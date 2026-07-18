//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-18
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"

#include "Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>

#include <cassert>
#include <stdexcept>
#include <string>


namespace CE::Core {

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void LogError(Events::ErrorEvent& appErrorEvent) {
	CE_CORE_ERROR(appErrorEvent);
}

static NS::Menu* CreateMenuBar() {
	const auto mainMenu = NS::Menu::alloc()->init();

	const auto appName = NS::RunningApplication::currentApplication()->localizedName();
	const auto appNameString = std::string(appName->cString(NS::UTF8StringEncoding));

#pragma region AppMenu
	const auto appMenuItem = NS::MenuItem::alloc()->init();
	const auto appMenu = NS::Menu::alloc()->init();

#pragma region SectionQuit
	const auto quitTitle = NS::String::string("Quit ", NS::UTF8StringEncoding)->stringByAppendingString(appName);
	const auto quitSelector = NS::MenuItem::registerActionCallback((appNameString + "Quit").c_str(), CocoaApplication::StOnQuitMenuCallback);
	appMenu->addItem(quitTitle, quitSelector, NS::String::string("q", NS::UTF8StringEncoding));
#pragma endregion

	appMenuItem->setSubmenu(appMenu);
	mainMenu->addItem(appMenuItem);

	appMenu->release();
	appMenuItem->release();
#pragma endregion

#pragma region WindowMenu
	const auto windowMenuItem = NS::MenuItem::alloc()->init();
	const auto windowMenu = NS::Menu::alloc()->init(NS::String::string("Window", NS::UTF8StringEncoding));

#pragma region SectionVSync
	const auto vsyncTitle = NS::String::string("Toggle VSync", NS::UTF8StringEncoding);
	const auto vsyncSelector = NS::MenuItem::registerActionCallback((appNameString + "ToggleVSync").c_str(), CocoaApplication::StOnToggleVSyncCallback);
	windowMenu->addItem(vsyncTitle, vsyncSelector, NS::String::string("", NS::UTF8StringEncoding));
#pragma endregion

	windowMenuItem->setSubmenu(windowMenu);
	mainMenu->addItem(windowMenuItem);

	windowMenu->release();
	windowMenuItem->release();
#pragma endregion

	return mainMenu->autorelease();
}

void CocoaApplicationEventHandler::DispatchErrorEvent(const int errorCode, const char* description) const {
	applicationEvents.onErrorDispatcher.Dispatch(errorCode, description);
}

void CocoaApplicationEventHandler::DispatchTickEvent() const {
	applicationEvents.onTickDispatcher.Dispatch();
}

void CocoaApplicationEventHandler::DispatchUpdateEvent() const {
	applicationEvents.onUpdateDispatcher.Dispatch();
}

void CocoaApplicationEventHandler::DispatchRenderEvent() const {
	applicationEvents.onRenderDispatcher.Dispatch();
}

CocoaApplication::CocoaApplication() {
	assert(_stInstance == nullptr && "CocoaApplication::CocoaApplication: CocoaApplication already exists!");
	_stInstance = this;

	I_Application::SetRunning(false);

	_appCocoa = NS::RetainPtr(NS::Application::sharedApplication());
}

CocoaApplication::~CocoaApplication() {
	if (IsRunning())
		CocoaApplication::Quit();

	ShutdownInput();

	_StopTickLoop();

	_layerStack.Clear();
	_imguiLayer = nullptr;

	// Reset the window before the context: the window retains the context-owned view (as its content view) and that
	// view references the context-owned event dispatcher, so the window must let go of the view first.
	_window.reset();
	_context.reset();

	_appCocoa.reset();
}

void CocoaApplication::Run() {
	ResetDeltaTime();
	SetRunning(true);

	_appCocoa->run();
}

void CocoaApplication::Quit() {
	_appCocoa->stop(nullptr);

	const auto dummyEvent = NS::Event::otherEventWithType(
	NS::EventTypeAppKitDefined,
		CGPointZero,
		0,
		0.0,
		0,
		nullptr,
		0,
		0,
		0
	);
	_appCocoa->postEvent(dummyEvent, true);

	UnsubscribeFromDispatcher();

	I_Application::SetRunning(false);
}

void CocoaApplication::Tick(const float deltaTime) {
	applicationEventHandler.DispatchTickEvent();

	for (const auto layer: _layerStack)
		layer->OnUpdate();
	applicationEventHandler.DispatchUpdateEvent();

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();
		applicationEventHandler.DispatchRenderEvent();

		_imguiLayer->End();
	}
}

void CocoaApplication::Init() {
	assert(not _window && "CocoaApplication::Init: Window is already initialized!");
	assert(not _context && "CocoaApplication::Init: Renderer is already initialized!");

	_appCocoa->setDelegate(&_appDelegate);

	// App-level launch setup (activation policy, menu bar, activation, window reveal) is deferred to the run loop via the
	// applicationDidFinishLaunching hook, the canonical Cocoa place for it.
	_appDelegate.SetApplicationDidFinishLaunchingDelegate(
		EventDelegate<NS::Notification*>::FromConstMethod<CocoaApplication, &CocoaApplication::_OnDidFinishLaunching>(this)
	);

	_appDelegate.SetApplicationWillFinishLaunchingDelegate(
		EventDelegate<NS::Notification*>::FromConstMethod<CocoaApplication, &CocoaApplication::_OnWillFinishLaunching>(this)
	);

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	if (not Types::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		const auto error = fmt::format("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {}, Window API: {}", windowProps.graphicsApi, windowProps.windowApi);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_context = std::make_unique<MetalContext>();
	_context->Init();

	_window = std::make_unique<CocoaWindow>();
	_window->Init();

	// Attach the render context's MetalKit view to the window and wire it into the engine.
	_window->SetContentView(_context->GetView());

	InitInput(windowProps.windowApi);

	SetEventHubDispatcher();
	SubscribeToHubDispatcher();

	_BindContextDelegates();

	_context->SetVSync(windowProps.VSync);

	const auto [vertexFunction, fragmentFunction] = _context->GetShaderLibrary()->GetShaderProgram("vertexMain", "fragmentMain");

	const auto renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
	renderPipelineDescriptor->setVertexFunction(vertexFunction);
	renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
	renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(_context->props.pixelFormat);

	NS::Error* pipelineError = nullptr;
	defaultRenderPipelineState = _context->GetDevice()->newRenderPipelineState(renderPipelineDescriptor, &pipelineError);
	if (not defaultRenderPipelineState) {
		const auto error = fmt::format("CocoaApplication::Init: Failed to create default render pipeline state. Error: {}", std::string(pipelineError->localizedDescription()->utf8String()));
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	renderPipelineDescriptor->release();
}

void CocoaApplication::SetImGuiLayer(I_Layer* imguiLayer) {
	if (not imguiLayer){
		CE_WARN("CocoaApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto metalLayer = dynamic_cast<ImGuiMetalLayer*>(imguiLayer)) {
		if (_imguiLayer) {
			_imguiLayer->UnsubscribeFromEventHub();
			PopOverlay(_imguiLayer);
			_imguiLayer = nullptr;
		}

		_imguiLayer = metalLayer;
		PushOverlay(_imguiLayer);
		_imguiLayer->SubscribeToEventHub();
	}
	else {
		constexpr auto error = "CocoaApplication::SetImGuiLayer: Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

void CocoaApplication::RemoveImGuiLayer() {
	if (not _imguiLayer)
		return;

	_imguiLayer->UnsubscribeFromEventHub();
	PopOverlay(_imguiLayer);
	_imguiLayer = nullptr;
}

void CocoaApplication::SetRunning(const bool running) {
	I_Application::SetRunning(running);

	if (not Utility::Config::Config::StGetWindowProps().VSync) {
		// VSync off: a dedicated thread paces frames by dispatching Tick onto the main queue.
		if (running)
			_StartTickLoop();
		else
			_StopTickLoop();
	}
	else {
		// VSync on: the CAMetalDisplayLink paces rendering. Pausing it stops frame delivery when the app is not running.
		_context->SetDisplayLinkPaused(not running);
	}
}

void CocoaApplication::_StartTickLoop() {
	if (_loopThread.joinable())
		return;

	_loopThreadRunning.store(true, std::memory_order_release);
	_loopThread = std::thread([this] {
		while (_loopThreadRunning.load(std::memory_order_acquire)) {
			if (not _tickPending.exchange(true)) {
				dispatch_async_f(dispatch_get_main_queue(), this, [](void* context) {
					if (const auto app = static_cast<CocoaApplication*>(context)) {
						app->_tickPending.store(false, std::memory_order_release);
						// Drop the frame if the app stopped or the loop was torn down (e.g. VSync was switched on) between
						// the dispatch and now, so no stale Tick calls nextDrawable() once a display link exists.
						if (app->IsRunning() and app->_loopThreadRunning.load(std::memory_order_acquire))
							app->Tick(app->GetDeltaTime());
					}
				});
			}
		}
	});
}

void CocoaApplication::_StopTickLoop() {
	_loopThreadRunning.store(false, std::memory_order_release);
	if (_loopThread.joinable())
		_loopThread.join();

	// Clear the in-flight flag so a later restart is not stuck thinking a tick is still pending.
	_tickPending.store(false, std::memory_order_release);
}

void CocoaApplication::_ApplyVSync(const bool enabled) {
	assert(_context && "CocoaApplication::_ApplyVSync: Render context must be initialized");

	// Runs on the main thread (menu action). Nothing to do if the layer is already in the requested mode.
	if (_context->IsVSyncEnabled() == enabled)
		return;

	if (enabled) {
		// Tick loop → display link: stop the loop first so no stale frame calls CAMetalLayer::nextDrawable() once the
		// display link exists, then let the context build the link and resume it if the app is running.
		_StopTickLoop();
		_context->SetVSync(true);
		if (IsRunning())
			_context->SetDisplayLinkPaused(false);
	}
	else {
		// Display link → tick loop: the context tears down the display link (re-enabling nextDrawable), then the tick loop
		// takes over pacing while the app is running.
		_context->SetVSync(false);
		if (IsRunning())
			_StartTickLoop();
	}
}

void CocoaApplication::InitImGuiLayer() {
	assert(_window && "CocoaApplication::InitImGuiLayer: Window must be initialized before initializing ImGui layer");
	assert(_context && "CocoaApplication::InitImGuiLayer: Renderer must be initialized before initializing ImGui layer");
	assert(not _imguiLayer && "CocoaApplication::InitImGuiLayer: ImGui layer is already initialized!");

	auto overlay = std::make_unique<ImGuiMetalLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
	_imguiLayer->SubscribeToEventHub();
}

void CocoaApplication::SetEventHubDispatcher() {
	if (not (_context and _window)) {
		constexpr auto error = "CocoaApplication::SetEventHubDispatcher: Context and window must be initialized before setting up the event hub dispatcher";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// The hub needs the render view and window to convert native (bottom-left) mouse coordinates into engine space.
	eventHubDispatcher.SetSources(_context.get(), _window.get());

	using hub = CocoaEventHubDispatcher;

#pragma region MouseEvents
	auto& mouseEvents = _context->metalContextEventDispatcher.mouseEvents;


	mouseEvents.mouseDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonDownEvent>(&eventHubDispatcher));
	mouseEvents.rightMouseDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonDownEvent>(&eventHubDispatcher));
	mouseEvents.otherMouseDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonDownEvent>(&eventHubDispatcher));

	mouseEvents.mouseUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonUpEvent>(&eventHubDispatcher));
	mouseEvents.rightMouseUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonUpEvent>(&eventHubDispatcher));
	mouseEvents.otherMouseUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseButtonUpEvent>(&eventHubDispatcher));

	mouseEvents.mouseDraggedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseDraggedEvent>(&eventHubDispatcher));
	mouseEvents.rightMouseDraggedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseDraggedEvent>(&eventHubDispatcher));
	mouseEvents.otherMouseDraggedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseDraggedEvent>(&eventHubDispatcher));

	mouseEvents.mouseMovedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveMouseMovedEvent>(&eventHubDispatcher));
	mouseEvents.scrollWheelDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveScrollWheelEvent>(&eventHubDispatcher));
#pragma endregion

#pragma region KeyboardEvents
	auto& keyboardEvents = _context->metalContextEventDispatcher.keyboardEvents;
	keyboardEvents.keyDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyDownEvent>(&eventHubDispatcher));
	keyboardEvents.keyUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyUpEvent>(&eventHubDispatcher));
#pragma endregion

#pragma region WindowEvents
	_window->cocoaWindowEventDispatcher.nsWindowLifecycleEvents.willCloseDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowWillCloseEvent>(&eventHubDispatcher)
	);
#pragma endregion

#pragma region RenderContextEvents
	_context->metalContextEventDispatcher.metalContextLifeCycleEvents.onResizeDispatcher.Bind(
		EventDelegate<unsigned int, unsigned int>::FromMethod<hub, &hub::ReceiveWindowResizeEvent>(&eventHubDispatcher)
	);
#pragma endregion

#pragma region ApplicationEvents
	applicationEventHandler.applicationEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveAppErrorEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onTickDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppTickEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onUpdateDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppUpdateEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onRenderDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppRenderEvent>(&eventHubDispatcher));
#pragma endregion
}

void CocoaApplication::SubscribeToHubDispatcher() {
	_eventHubHandlers[AppError] = eventHubDispatcher.cocoaApplicationEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);

	_eventHubHandlers[WindowClose] = eventHubDispatcher.cocoaWindowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<CocoaApplication, &CocoaApplication::_OnWindowClose>(this)
	);
	_eventHubHandlers[WindowError] = eventHubDispatcher.cocoaWindowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);
}

void CocoaApplication::UnsubscribeFromDispatcher() {
	eventHubDispatcher.cocoaApplicationEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[AppError]);

	eventHubDispatcher.cocoaWindowEventHub.onCloseMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowClose]);
	eventHubDispatcher.cocoaWindowEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowError]);
}

void CocoaApplication::StOnQuitMenuCallback(void*, SEL, const NS::Object*) {
	StGet().Quit();
}

void CocoaApplication::StOnToggleVSyncCallback(void*, SEL, const NS::Object*) {
	Types::WindowProps windowProps = Utility::Config::StGetWindowProps();
	windowProps.VSync = !windowProps.VSync;
	Utility::Config::StSetWindowProps(windowProps);

	// Menu actions fire on the main thread, so it is safe to swap the frame-pacing mechanism here.
	StGet()._ApplyVSync(windowProps.VSync);
}

void CocoaApplication::_BindContextDelegates() {
	assert(_context && "CocoaApplication::_BindViewCallbacks: Render context must be initialized before binding callbacks");

	// Drive a frame from the CAMetalDisplayLink. This is what paces rendering while VSync is enabled (the display link is
	// unpaused in SetRunning); with VSync off the dedicated tick loop drives Tick instead and the display link stays paused.
	// (Drawable resize is handled by the context itself and routed to the hub via its resize dispatcher.)
	_context->SetDrawDelegate(EventDelegate<MTK::View*>::FromMethod<CocoaApplication, &CocoaApplication::_OnDraw>(this));
}

void CocoaApplication::_OnDidFinishLaunching(NS::Notification*) const {
	// Bring the app to the foreground and reveal the window now that the run loop is active.
	_appCocoa->activateIgnoringOtherApps(true);
	_window->Show();
}

void CocoaApplication::_OnWillFinishLaunching(NS::Notification*) const {
	if (not _appCocoa) {
		constexpr auto error = "CocoaApplication::_OnWillFinishLaunching: The cocoa application is not initialized";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
	const auto mainMenu = CreateMenuBar();
	_appCocoa->setMainMenu(mainMenu);

	if (_appCocoa->activationPolicy() != NS::ActivationPolicyRegular) {
		if (not _appCocoa->setActivationPolicy(NS::ActivationPolicyRegular)) {
			constexpr auto error = "CocoaApplication::_OnDidFinishLaunching: Failed to set activation policy for the application";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
	}
}

void CocoaApplication::_OnWindowClose(Events::WindowCloseEvent&) {
	Quit();
}

void CocoaApplication::_OnDraw(MTK::View*) {
	Tick(GetDeltaTime());
}

}
