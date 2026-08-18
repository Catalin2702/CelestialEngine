//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"

#include "Core/Application/Platforms/Mac/Cocoa/MacMenuBar.hpp"
#include "Core/Input/Input.hpp"
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>

#include <cassert>
#include <format>
#include <stdexcept>
#include <string>


namespace CE::Core {

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void LogError(Events::ErrorEvent& appErrorEvent) {
	CE_CORE_ERROR(appErrorEvent);
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

	// Starts empty (count 0): the tick loop posts a frame, then waits here until the main thread signals completion.
	_tickSemaphore = dispatch_semaphore_create(0);
}

CocoaApplication::~CocoaApplication() {
	if (IsRunning())
		CocoaApplication::Quit();

	Input::Shutdown();

	_StopTickLoop();

	_layerStack.Clear();
	_imguiLayer = nullptr;

	// Reset the window before the context: the window retains the context-owned view (as its content view) and that
	// view references the context-owned event dispatcher, so the window must let go of the view first.
	_window.reset();
	_context.reset();

	_appCocoa.reset();

	if (_tickSemaphore) {
		dispatch_release(_tickSemaphore);
		_tickSemaphore = nullptr;
	}
}

void CocoaApplication::Start() {
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

	// Clear per-frame input transitions (just pressed/released, scroll deltas) now that every layer had a chance to
	// query them. Native events pumped by the run loop between this tick and the next fill the next frame's transitions.
	Input::EndFrame();
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

	const auto& windowProps = Utility::Config::StGetWindowProps();

	if (not Types::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		const auto error = std::format("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {}, Window API: {}", windowProps.graphicsApi, windowProps.windowApi);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_context = std::make_unique<MetalContext>();
	_context->Init();

	_window = std::make_unique<CocoaWindow>();
	_window->Init();

	// Attach the render context's MetalKit view to the window and wire it into the engine.
	_window->SetContentView(_context->GetView());

	Input::Init();

	SetEventHubDispatcher();
	SubscribeToHubDispatcher();

	_BindContextDelegates();

	_context->SetVSync(windowProps.VSync);

	const auto [vertexFunction, fragmentFunction] = _context->GetShaderLibrary().GetShaderProgram("vertexMain", "fragmentMain");

	const auto renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
	renderPipelineDescriptor->setVertexFunction(vertexFunction);
	renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
	renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(_context->props.pixelFormat);

	NS::Error* pipelineError = nullptr;
	defaultRenderPipelineState = _context->GetDevice()->newRenderPipelineState(renderPipelineDescriptor, &pipelineError);
	if (not defaultRenderPipelineState) {
		const auto error = std::format("CocoaApplication::Init: Failed to create default render pipeline state. Error: {}", std::string(pipelineError->localizedDescription()->utf8String()));
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

	assert(_context && "CocoaApplication::SetRunning: Render context must be initialized");

	if (running)
		_Run();
	else
		_Pause();
}

void CocoaApplication::_StartTickLoop() {
	if (_loopThread.joinable())
		return;

	// Drain any permit left over from a previous run (the stop signal or a late tick block) so the loop starts balanced.
	while (dispatch_semaphore_wait(_tickSemaphore, DISPATCH_TIME_NOW) == 0) {}

	_loopThreadRunning.store(true, std::memory_order_release);
	_loopThread = std::thread([this] {
		while (_loopThreadRunning.load(std::memory_order_acquire)) {
			// Pace against the main thread instead of busy-spinning: post exactly one Tick, then block until it completes.
			dispatch_async_f(dispatch_get_main_queue(), this, [](void* context) {
				if (const auto app = static_cast<CocoaApplication*>(context)) {
					// Drop the frame if the app stopped or the loop was torn down (e.g. VSync was switched on) between
					// the dispatch and now, so no stale Tick calls nextDrawable() once a display link exists.
					if (app->IsRunning() and app->_loopThreadRunning.load(std::memory_order_acquire))
						app->Tick(app->GetDeltaTime());
					// Release the loop thread to schedule the next frame.
					dispatch_semaphore_signal(app->_tickSemaphore);
				}
			});

			dispatch_semaphore_wait(_tickSemaphore, DISPATCH_TIME_FOREVER);
		}
	});
}

void CocoaApplication::_StopTickLoop() {
	_loopThreadRunning.store(false, std::memory_order_release);

	// Signal before joining: this runs on the main thread while the loop thread may be blocked waiting for a main-queue tick
	// block that can no longer run (the main thread is stuck in join). Waking the loop thread here avoids that deadlock.
	dispatch_semaphore_signal(_tickSemaphore);

	if (_loopThread.joinable())
		_loopThread.join();
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
	auto& [keyDownDispatcher, keyUpDispatcher, flagsChangedDispatcher] = _context->metalContextEventDispatcher.keyboardEvents;
	keyDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyDownEvent>(&eventHubDispatcher));
	keyUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyUpEvent>(&eventHubDispatcher));
	// Modifier keys (Shift/Ctrl/Alt/Cmd) never arrive as keyDown/keyUp: AppKit reports them via flagsChanged.
	flagsChangedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveFlagsChangedEvent>(&eventHubDispatcher));
#pragma endregion

#pragma region WindowEvents
	_window->cocoaWindowEventDispatcher.nsWindowLifecycleEvents.willCloseDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowWillCloseEvent>(&eventHubDispatcher)
	);
	// Focus changes feed the hub so input state can reset held keys when the window stops receiving events.
	_window->cocoaWindowEventDispatcher.nsWindowFocusEvents.didBecomeKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowDidBecomeKeyEvent>(&eventHubDispatcher)
	);
	_window->cocoaWindowEventDispatcher.nsWindowFocusEvents.didResignKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowDidResignKeyEvent>(&eventHubDispatcher)
	);
#pragma endregion

#pragma region RenderContextEvents
	_context->metalContextEventDispatcher.metalContextLifeCycleEvents.onResizeDispatcher.Bind(
		EventDelegate<double, double>::FromMethod<hub, &hub::ReceiveContextResizeViewEvent>(&eventHubDispatcher)
	);
	_context->metalContextEventDispatcher.metalContextLifeCycleEvents.onVSyncChangedDispatcher.Bind(
		EventDelegate<bool>::FromMethod<hub, &hub::ReceiveContextChangeVSyncEvent>(&eventHubDispatcher)
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
	using app = CocoaApplication;

	// Input state MUST subscribe first: it has to be up to date before any other subscriber (ImGui, layers) handles
	// the same event. Requires Input::Init to have run (it has: Init calls it right before this method).
	Input::SubscribeToHub(
		eventHubDispatcher.cocoaKeyboardEventHub,
		eventHubDispatcher.cocoaMouseEventHub,
		eventHubDispatcher.cocoaWindowEventHub
	);

	_eventHubHandlers[AppError] = eventHubDispatcher.cocoaApplicationEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);

	_eventHubHandlers[VSyncChange] = eventHubDispatcher.metalRenderContextEventHub.onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromMethod<app, &app::_OnVSyncChange>(this)
	);

	_eventHubHandlers[WindowClose] = eventHubDispatcher.cocoaWindowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<app, &app::_OnWindowClose>(this)
	);
	_eventHubHandlers[WindowError] = eventHubDispatcher.cocoaWindowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);
}

void CocoaApplication::UnsubscribeFromDispatcher() {
	Input::UnsubscribeFromHub(
		eventHubDispatcher.cocoaKeyboardEventHub,
		eventHubDispatcher.cocoaMouseEventHub,
		eventHubDispatcher.cocoaWindowEventHub
	);

	eventHubDispatcher.cocoaApplicationEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[AppError]);

	eventHubDispatcher.metalRenderContextEventHub.onChangeVSyncDispatcher.Unsubscribe(_eventHubHandlers[VSyncChange]);

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

	StGet().GetMetalContext().SetVSync(windowProps.VSync);
}

void CocoaApplication::StOnMiniaturizeCallback(void*, SEL, const NS::Object*) {
	StGet().GetCocoaWindow().Miniaturize();
}

void CocoaApplication::StOnDeminiaturizeCallback(void*, SEL, const NS::Object*) {
	StGet().GetCocoaWindow().Deminiaturize();
}

void CocoaApplication::StOnToggleFullscreenCallback(void*, SEL, const NS::Object*) {
	StGet().GetCocoaWindow().ToggleFullScreen();
}

void CocoaApplication::_Run() {
	if (Utility::Config::StGetWindowProps().VSync) {
		// VSync on: the CAMetalDisplayLink paces rendering. Tear down the tick loop first so no stale frame calls
		// CAMetalLayer::nextDrawable() once a display link exists for the layer, then build/resume the link.
		_StopTickLoop();
		_context->SetDisplayLinkPaused(false);
	}
	else {
		_StartTickLoop();
	}
}

void CocoaApplication::_Pause() {
	if (_context->IsVSyncEnabled())
		_context->SetDisplayLinkPaused(true);
	else
		_StopTickLoop();
}

void CocoaApplication::_BindContextDelegates() {
	assert(_context && "CocoaApplication::_BindViewCallbacks: Render context must be initialized before binding callbacks");

	// Drive a frame from the CAMetalDisplayLink. This is what paces rendering while VSync is enabled (the display link is
	// unpaused in SetRunning(); with VSync off the dedicated tick loop drives Tick instead and the display link stays paused.
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
	const auto mainMenu = CreateMenuBar<CocoaApplication>();
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

void CocoaApplication::_OnVSyncChange(Events::VSyncEvent&) {
	_Pause();
	if (IsRunning())
		_Run();
}

void CocoaApplication::_OnDraw(MTK::View*) {
	Tick(GetDeltaTime());
}

}
