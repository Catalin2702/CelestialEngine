//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
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

// The context and the window are members held by value, so they are built by the initializer list, before the constructor
// body ever runs - which is where the graphics/window API compatibility check used to live. It moves here so it still runs
// before any of them exists: the helper validates the configured pair and only then builds the context (the first of the
// two, see the member declaration order). Mirrors GlfwApplication::CreateValidatedWindow.
static MetalContext CreateValidatedContext() {
	if (const auto& windowProps = Utility::Config::StGetWindowProps(); not Types::IsGraphicsApiCompatible(windowProps.graphicsApi, windowProps.windowApi)) [[unlikely]] {
		const auto error = std::format("CocoaApplication::CocoaApplication: Incompatible graphics API and window API specified in window properties. Graphics API: {}, Window API: {}", windowProps.graphicsApi, windowProps.windowApi);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	return MetalContext{};
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

// The shared NSApplication is retained first (declaration order puts it ahead of the context and the window): everything
// AppKit-side built below assumes it exists.
CocoaApplication::CocoaApplication():
	_appCocoa(NS::RetainPtr(NS::Application::sharedApplication())),
	_context(CreateValidatedContext()),
	_tickSemaphore(dispatch_semaphore_create(0)) {
	assert(_stInstance == nullptr && "CocoaApplication::CocoaApplication: CocoaApplication already exists!");
	_stInstance = this;

	// The singleton is published before the init steps because they reach back through StGet, so a throw from any of them
	// has to clear it by hand: the destructor - which is what normally resets it - never runs for an object whose
	// constructor did not complete, and the stale pointer would fail the assert above on the next application.
	try {
		// Reversed compared to GlfwApplication, which brings the window up first: here the window takes the context-owned
		// MetalKit view as its content view, so the context has to exist before the window is initialized.
		CocoaApplication::_InitRenderer();
		CocoaApplication::_InitWindow();

		CocoaApplication::Init();
	}
	catch (...) {
		_stInstance = nullptr;
		throw;
	}
}

CocoaApplication::~CocoaApplication() {
	if (IsRunning()) [[likely]]
		CocoaApplication::Quit();

	Input::Shutdown();

	_StopTickLoop();

	_layerStack.Clear();
	_imguiLayer.reset();

	// The window is destroyed before the context (reverse declaration order, see the members in the header): the window
	// retains the context-owned view as its content view, and that view references the context-owned event dispatcher.
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

void CocoaApplication::Tick(const f32 deltaTime) {
	applicationEventHandler.DispatchTickEvent();

	for (const auto& layer: _layerStack)
		layer->OnUpdate();
	applicationEventHandler.DispatchUpdateEvent();

	if (const auto imguiLayer = _imguiLayer.lock()) [[likely]] {
		imguiLayer->Begin(deltaTime);

		for (const auto& layer: _layerStack)
			layer->OnRender();
		applicationEventHandler.DispatchRenderEvent();

		imguiLayer->End();
	}

	// Clear per-frame input transitions (just pressed/released, scroll deltas) now that every layer had a chance to
	// query them. Native events pumped by the run loop between this tick and the next fill the next frame's transitions.
	Input::EndFrame();
}

void CocoaApplication::Init() {
	I_Application::SetRunning(false);

	_appCocoa->setDelegate(&_appDelegate);

	// App-level launch setup (activation policy, menu bar, activation, window reveal) is deferred to the run loop via the
	// applicationDidFinishLaunching hook, the canonical Cocoa place for it.
	_appDelegate.SetApplicationDidFinishLaunchingDelegate(
		EventDelegate<NS::Notification*>::FromMethod<CocoaApplication, &CocoaApplication::_OnDidFinishLaunching>(this)
	);

	_appDelegate.SetApplicationWillFinishLaunchingDelegate(
		EventDelegate<NS::Notification*>::FromConstMethod<CocoaApplication, &CocoaApplication::_OnWillFinishLaunching>(this)
	);

	// Last: SetVSync fires the context's VSync dispatcher, and _OnVSyncChange (which reconciles the frame pacing with it)
	// only reaches this application once _InitWindow has wired the hub and subscribed the app-level handlers.
	_context.SetVSync(Utility::Config::StGetWindowProps().VSync);
}

void CocoaApplication::_InitWindow() {
	// The context and the window are members held by value: they exist from construction, so this only brings the window
	// up and attaches the render context's MetalKit view to it.
	_window.Init();
	_window.SetContentView(_context.GetView());

	// Input must exist before SubscribeToHubDispatcher: the input state it owns is the hub's first subscriber.
	Input::Init();

	SetEventHubDispatcher();
	SubscribeToHubDispatcher();
}

void CocoaApplication::_InitRenderer() {
	_context.Init();

	_BindContextDelegates();

	const auto [vertexFunction, fragmentFunction] = _context.GetShaderLibrary().GetShaderProgram("vertexMain", "fragmentMain");

	const auto renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
	renderPipelineDescriptor->setVertexFunction(vertexFunction);
	renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
	renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(_context.props.pixelFormat);

	NS::Error* pipelineError = nullptr;
	defaultRenderPipelineState = _context.GetDevice()->newRenderPipelineState(renderPipelineDescriptor, &pipelineError);
	if (not defaultRenderPipelineState) [[unlikely]] {
		const auto error = std::format("CocoaApplication::_InitRenderer: Failed to create default render pipeline state. Error: {}", std::string(pipelineError->localizedDescription()->utf8String()));
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	renderPipelineDescriptor->release();
}

void CocoaApplication::SetImGuiLayer(const std::shared_ptr<I_Layer>& imguiLayer) {
	if (not imguiLayer) [[unlikely]] {
		CE_WARN("CocoaApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto metalLayer = std::dynamic_pointer_cast<ImGuiMetalLayer>(imguiLayer)) [[likely]] {
		if (const auto oldLayer = _imguiLayer.lock()) [[likely]] {
			ReplaceLayer(oldLayer, metalLayer);
		}
		else {
			PushOverlay(metalLayer);
		}

		metalLayer->SubscribeToEventHub();
		_imguiLayer = metalLayer;
	}
	else {
		constexpr auto error = "CocoaApplication::SetImGuiLayer: Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

void CocoaApplication::RemoveImGuiLayer() {
	// The lock keeps the layer alive across PopOverlay: the stack holds the only owning reference, so erasing the entry
	// would otherwise destroy it while OnDetach is still running. Its destructor drops the hub subscriptions.
	const auto imguiLayer = _imguiLayer.lock();
	if (not imguiLayer) [[unlikely]]
		return;

	PopOverlay(imguiLayer);
	_imguiLayer.reset();
}

void CocoaApplication::SetRunning(const bool running) {
	I_Application::SetRunning(running);


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
				if (const auto app = static_cast<CocoaApplication*>(context)) [[likely]] {
					// Drop the frame if the app stopped or the loop was torn down (e.g. VSync was switched on) between
					// the dispatch and now, so no stale Tick calls nextDrawable() once a display link exists.
					if (app->IsRunning() and app->_loopThreadRunning.load(std::memory_order_acquire)) [[likely]]
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

	if (_loopThread.joinable()) [[likely]]
		_loopThread.join();
}

void CocoaApplication::InitImGuiLayer() {
	assert(_imguiLayer.expired() && "CocoaApplication::InitImGuiLayer: ImGui layer is already initialized!");

	const auto imguiLayer = std::make_shared<ImGuiMetalLayer>();
	PushOverlay(imguiLayer);
	imguiLayer->SubscribeToEventHub();
	_imguiLayer = imguiLayer;
}

void CocoaApplication::SetEventHubDispatcher() {
	// The hub needs the render view and window to convert native (bottom-left) mouse coordinates into engine space.
	eventHubDispatcher.SetSources(&_context, &_window);

	using hub = CocoaEventHubDispatcher;

#pragma region MouseEvents
	auto& mouseEvents = _context.metalContextEventDispatcher.mouseEvents;


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
	auto& [keyDownDispatcher, keyUpDispatcher, flagsChangedDispatcher] = _context.metalContextEventDispatcher.keyboardEvents;
	keyDownDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyDownEvent>(&eventHubDispatcher));
	keyUpDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveKeyUpEvent>(&eventHubDispatcher));
	// Modifier keys (Shift/Ctrl/Alt/Cmd) never arrive as keyDown/keyUp: AppKit reports them via flagsChanged.
	flagsChangedDispatcher.Bind(EventDelegate<const NS::Event*>::FromMethod<hub, &hub::ReceiveFlagsChangedEvent>(&eventHubDispatcher));
#pragma endregion

#pragma region WindowEvents
	_window.cocoaWindowEventDispatcher.nsWindowLifecycleEvents.willCloseDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowWillCloseEvent>(&eventHubDispatcher)
	);
	// Focus changes feed the hub so input state can reset held keys when the window stops receiving events.
	_window.cocoaWindowEventDispatcher.nsWindowFocusEvents.didBecomeKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowDidBecomeKeyEvent>(&eventHubDispatcher)
	);
	_window.cocoaWindowEventDispatcher.nsWindowFocusEvents.didResignKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<hub, &hub::ReceiveWindowDidResignKeyEvent>(&eventHubDispatcher)
	);
#pragma endregion

#pragma region RenderContextEvents
	_context.metalContextEventDispatcher.metalContextLifeCycleEvents.onResizeDispatcher.Bind(
		EventDelegate<f64, f64>::FromMethod<hub, &hub::ReceiveContextResizeViewEvent>(&eventHubDispatcher)
	);
	_context.metalContextEventDispatcher.metalContextLifeCycleEvents.onVSyncChangedDispatcher.Bind(
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
	// the same event. Requires Input::Init to have run (it has: _InitWindow calls it right before this method).
	Input::SubscribeToHub(
		eventHubDispatcher.keyboardEventHub,
		eventHubDispatcher.mouseEventHub,
		eventHubDispatcher.windowEventHub
	);

	_eventHubHandlers[AppError] = eventHubDispatcher.applicationEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);

	_eventHubHandlers[VSyncChange] = eventHubDispatcher.renderContextEventHub.onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromMethod<app, &app::_OnVSyncChange>(this)
	);

	_eventHubHandlers[WindowClose] = eventHubDispatcher.windowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<app, &app::_OnWindowClose>(this)
	);
	_eventHubHandlers[WindowError] = eventHubDispatcher.windowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);
}

void CocoaApplication::UnsubscribeFromDispatcher() {
	Input::UnsubscribeFromHub(
		eventHubDispatcher.keyboardEventHub,
		eventHubDispatcher.mouseEventHub,
		eventHubDispatcher.windowEventHub
	);

	eventHubDispatcher.applicationEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[AppError]);

	eventHubDispatcher.renderContextEventHub.onChangeVSyncDispatcher.Unsubscribe(_eventHubHandlers[VSyncChange]);

	eventHubDispatcher.windowEventHub.onCloseMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowClose]);
	eventHubDispatcher.windowEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowError]);
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
		_context.SetDisplayLinkPaused(false);
	}
	else {
		_StartTickLoop();
	}
}

void CocoaApplication::_Pause() {
	if (_context.IsVSyncEnabled())
		_context.SetDisplayLinkPaused(true);
	else
		_StopTickLoop();
}

void CocoaApplication::_BindContextDelegates() {
	// Drive a frame from the CAMetalDisplayLink. This is what paces rendering while VSync is enabled (the display link is
	// unpaused in SetRunning(); with VSync off the dedicated tick loop drives Tick instead and the display link stays paused.
	// (Drawable resize is handled by the context itself and routed to the hub via its resize dispatcher.)
	_context.SetDrawDelegate(EventDelegate<MTK::View*>::FromMethod<CocoaApplication, &CocoaApplication::_OnDraw>(this));
}

void CocoaApplication::_OnDidFinishLaunching(NS::Notification*) {
	// Bring the app to the foreground and reveal the window now that the run loop is active.
	_appCocoa->activateIgnoringOtherApps(true);
	_window.Show();
}

void CocoaApplication::_OnWillFinishLaunching(NS::Notification*) const {
	if (not _appCocoa) [[unlikely]] {
		constexpr auto error = "CocoaApplication::_OnWillFinishLaunching: The cocoa application is not initialized";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
	const auto mainMenu = CreateMenuBar<CocoaApplication>();
	_appCocoa->setMainMenu(mainMenu);

	if (_appCocoa->activationPolicy() != NS::ActivationPolicyRegular) [[likely]] {
		if (not _appCocoa->setActivationPolicy(NS::ActivationPolicyRegular)) [[unlikely]] {
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
	if (IsRunning()) [[likely]]
		_Run();
}

void CocoaApplication::_OnDraw(MTK::View*) {
	Tick(GetDeltaTime());
}

}
