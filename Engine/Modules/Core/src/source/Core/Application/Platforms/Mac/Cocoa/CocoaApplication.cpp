//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
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


namespace CE::Core {

static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
dispatch_semaphore_t _inFlightSemaphore = dispatch_semaphore_create(MAX_FRAMES_IN_FLIGHT);

// Action invoked by the "Quit" menu item (and its Cmd+Q shortcut). The item has no explicit target, so AppKit sends the
// action up the responder chain; metal-cpp installs this callback on NSObject, so it fires on the shared application.
// Route it through the engine's clean shutdown path instead of NSApplication::terminate.
static void OnQuitMenuItemSelected(void*, SEL, const NS::Object*) {
	CocoaApplication::StGet().Quit();
}

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
}

CocoaApplication::~CocoaApplication() {
	if (IsRunning())
		CocoaApplication::Quit();

	ShutdownInput();

	if (_loopThread.joinable()) {
		_loopThread.join();
	}

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
		EventDelegate<>::FromConstMethod<CocoaApplication, &CocoaApplication::_OnDidFinishLaunching>(this)
	);

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	if (not Types::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
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
		const auto errorMsg = "CocoaApplication::Init: Failed to create default render pipeline state. Error: " + std::string(pipelineError->localizedDescription()->utf8String());
		CE_CORE_ERROR(errorMsg);
		throw std::runtime_error(errorMsg);
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
		CE_CORE_ERROR("CocoaApplication::SetImGuiLayer: Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.");
		throw std::runtime_error("Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.");
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
		if (running && not _loopThread.joinable()) {
			_loopThread = std::thread([this] {
				while (_isRunning.load(std::memory_order_acquire)) {
					if (not _tickPending.exchange(true)) {
						dispatch_async_f(dispatch_get_main_queue(), this, [](void* context) {
							if (const auto app = static_cast<CocoaApplication*>(context)) {
								app->_tickPending.store(false, std::memory_order_release);
								if (app->IsRunning()) {
									app->Tick(app->GetDeltaTime());
								}
							}
						});
					}
				}
			});
		}
	}
	else {
		// VSync on: the CAMetalDisplayLink paces rendering. Pausing it stops frame delivery when the app is not running.
		_context->SetDisplayLinkPaused(not running);
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
		const auto error = "CocoaApplication::SetEventHubDispatcher: Context and window must be initialized before setting up the event hub dispatcher";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	assert(_context->metalContextEventDispatcher && "CocoaApplication::SetEventHubDispatcher: View event dispatcher must be initialized");

	// The hub needs the render view and window to convert native (bottom-left) mouse coordinates into engine space.
	eventHubDispatcher.SetSources(_context.get(), _window.get());

	using hub = CocoaEventHubDispatcher;

	// Native view input → hub. Left/right/other button variants all fold into the same button events (the engine keys off
	// the event's buttonNumber), mirroring how GLFW routes every button through one callback.
	auto& mouseEvents = _context->metalContextEventDispatcher->mouseEvents;
	auto& keyboardEvents = _context->metalContextEventDispatcher->keyboardEvents;

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

	keyboardEvents.keyDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyDownEvent>(&eventHubDispatcher));
	keyboardEvents.keyUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyUpEvent>(&eventHubDispatcher));

	// Native window lifecycle → hub.
	_window->cocoaWindowEventDispatcher.nsWindowLifecycleEvents.willCloseDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowWillCloseEvent>(&eventHubDispatcher)
	);

	// The render context owns the drawable-resize signal (backing pixels) → hub as a WindowResize.
	_context->metalContextEventDispatcher->onResizeDispatcher.Bind(
		EventDelegate<unsigned int, unsigned int>::FromMethod<hub, &hub::ReceiveWindowResizeEvent>(&eventHubDispatcher)
	);

	// The application fires its own lifecycle events (tick/update/render each frame, plus errors) into the hub.
	applicationEventHandler.applicationEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveAppErrorEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onTickDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppTickEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onUpdateDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppUpdateEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onRenderDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppRenderEvent>(&eventHubDispatcher));
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

void CocoaApplication::_BindContextDelegates() {
	assert(_context && "CocoaApplication::_BindViewCallbacks: Render context must be initialized before binding callbacks");

	// Drive a frame from the CAMetalDisplayLink. This is what paces rendering while VSync is enabled (the display link is
	// unpaused in SetRunning); with VSync off the dedicated tick loop drives Tick instead and the display link stays paused.
	// (Drawable resize is handled by the context itself and routed to the hub via its resize dispatcher.)
	_context->SetDrawDelegate(EventDelegate<MTK::View*>::FromMethod<CocoaApplication, &CocoaApplication::_OnDraw>(this));
}

void CocoaApplication::_OnDidFinishLaunching() const {
	// Promote the process to a regular UI app so it gets a Dock icon, can own the menu bar and hold a key window. This must
	// happen after launch: doing it here (rather than before run()) follows the standard Cocoa lifecycle.
	if (_appCocoa->activationPolicy() != NS::ActivationPolicyRegular) {
		if (not _appCocoa->setActivationPolicy(NS::ActivationPolicyRegular)) {
			CE_CORE_ERROR("CocoaApplication::_OnDidFinishLaunching: Failed to set activation policy for the application");
			throw std::runtime_error("Failed to set activation policy for the application");
		}
	}

	_CreateMenuBar();

	// Bring the app to the foreground and reveal the window now that the run loop is active.
	_appCocoa->activateIgnoringOtherApps(true);
	_window->Show();
}

void CocoaApplication::_CreateMenuBar() const {
	// AppKit expects the first item of the main menu to carry the application submenu (its own title is ignored, AppKit uses
	// the process name). A minimal app menu with a Quit item is enough to get the native menu bar and the Cmd+Q shortcut.
	const auto mainMenu = NS::Menu::alloc()->init();

	const auto appMenuItem = NS::MenuItem::alloc()->init();
	mainMenu->addItem(appMenuItem);

	const auto appMenu = NS::Menu::alloc()->init();

	const auto appName = NS::RunningApplication::currentApplication()->localizedName();
	const auto quitTitle = NS::String::string("Quit ", NS::UTF8StringEncoding)->stringByAppendingString(appName);

	const auto quitSelector = NS::MenuItem::registerActionCallback("celestialQuit", OnQuitMenuItemSelected);
	appMenu->addItem(quitTitle, quitSelector, NS::String::string("q", NS::UTF8StringEncoding));

	appMenuItem->setSubmenu(appMenu);

	_appCocoa->setMainMenu(mainMenu);

	// AppKit retains the menus once installed, so release the ownership taken by alloc/init. The strings above are
	// autoreleased (not owned), so they are left alone.
	appMenu->release();
	appMenuItem->release();
	mainMenu->release();
}

void CocoaApplication::_OnWindowClose(Events::WindowCloseEvent&) {
	Quit();
}

void CocoaApplication::_OnDraw(MTK::View*) {
	Tick(GetDeltaTime());
}

}
