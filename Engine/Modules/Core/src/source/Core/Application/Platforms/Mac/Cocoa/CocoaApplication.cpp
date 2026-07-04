//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Define/Bind.hpp"

#include <Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp>
#include "Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Callback/Dispatcher.hpp"
#include "Utility/Config/Config.hpp"

#include <AppKit/AppKit.hpp>

#include <cassert>
#include <stdexcept>


namespace CE::Core::Application {

static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
dispatch_semaphore_t _inFlightSemaphore = dispatch_semaphore_create(MAX_FRAMES_IN_FLIGHT);

CocoaApplication::CocoaApplication() {
	assert(_stInstance == nullptr && "CocoaApplication::CocoaApplication: CocoaApplication already exists!");
	_stInstance = this;

	I_Application::SetRunning(false);

	_appCocoa = NS::RetainPtr(NS::Application::sharedApplication());
}

CocoaApplication::~CocoaApplication() {
	if (IsRunning())
		CocoaApplication::Quit();

	Input::ShutdownInput();

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

	I_Application::SetRunning(false);
}

void CocoaApplication::Tick(const float deltaTime) {
	for (const auto layer: _layerStack)
		layer->OnUpdate();

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();

		_imguiLayer->End();
	}
}

void CocoaApplication::DispatchEventToLayers(Events::I_Event& event) {
	if (event.IsHandled())
		return;

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(event);
		if (event.IsHandled())
			break;
	}
}

void CocoaApplication::Init() {
	assert(not _window && "CocoaApplication::Init: Window is already initialized!");
	assert(not _context && "CocoaApplication::Init: Renderer is already initialized!");

	_appCocoa->setDelegate(&_appDelegate);

	if (_appCocoa->activationPolicy() != NS::ActivationPolicyRegular) {
		if (not _appCocoa->setActivationPolicy(NS::ActivationPolicyRegular)) {
			CE_CORE_ERROR("CocoaApplication::Init: Failed to set activation policy for the application");
			throw std::runtime_error("Failed to set activation policy for the application");
		}
	}

	_appCocoa->activateIgnoringOtherApps(true);

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	if (not Types::Window::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_context = std::make_unique<Render::Context::MetalContext>();
	_context->Init();

	_window = std::make_unique<Window::CocoaWindow>();
	_window->Init();

	// Attach the render context's MetalKit view to the window and wire it into the engine.
	_window->SetContentView(_context->GetView());

	Input::InitInput(windowProps.windowApi);

	_BindWindowCallbacks();
	_SetWindowCallbacks();

	_BindViewCallbacks();
	_SetViewEventCallbacks();

	_window->GetReady(windowProps.VSync);

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

void CocoaApplication::SetImGuiLayer(Layers::I_Layer* imguiLayer) {
	if (not imguiLayer){
		CE_WARN("CocoaApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto metalLayer = dynamic_cast<Layers::ImGuiMetalLayer*>(imguiLayer)) {
		if (_imguiLayer) {
			PopOverlay(_imguiLayer);
			_imguiLayer = nullptr;
		}

		_imguiLayer = metalLayer;
		PushOverlay(_imguiLayer);
	}
	else {
		CE_CORE_ERROR("CocoaApplication::SetImGuiLayer: Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.");
		throw std::runtime_error("Provided ImGui layer is not compatible with MetalContext. Expected ImGuiMetalLayer or derived class.");
	}
}

void CocoaApplication::RemoveImGuiLayer() {
	if (not _imguiLayer)
		return;

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

	auto overlay = std::make_unique<Layers::ImGuiMetalLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
}

void CocoaApplication::_BindWindowCallbacks() const {
	assert(_window && "CocoaApplication::_BindWindowCallbacks: Window must be initialized before binding callbacks");
	assert(_context && "CocoaApplication::_BindWindowCallbacks: Render context must be initialized before binding callbacks");

	_window->SetContentScaleCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleContentSizeChange));
	_window->SetVSyncCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleVSyncChange));
}

void CocoaApplication::_SetWindowCallbacks() {
	_window->windowEventDispatcher->windowWillCloseDispatcher.Subscribe(NotificationDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnWindowWillClose>(this));
}

void CocoaApplication::_BindViewCallbacks() {
	assert(_context && "CocoaApplication::_BindViewCallbacks: Render context must be initialized before binding callbacks");

	// Keep the Metal drawable in lockstep with the view on resize, then translate it into a window resize event so layers
	// (e.g. ImGui) can update their display size. The view is paused and driven by the display link, so MetalKit does not
	// auto-resize the drawable for us: without this explicit update the drawable stays at its initial size and the layer
	// stretches it across the new bounds, distorting the aspect ratio. `size` is already expressed in backing pixels.
	_context->SetResizeCallback([this](MTK::View*, const CGSize size) {
		_context->HandleContentSizeChange({static_cast<float>(size.width), static_cast<float>(size.height)});

		const auto [width, height] = _window->GetFrameSize();
		Events::WindowResizeEvent event{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
		DispatchEventToLayers(event);
	});

	// Drive a frame from the CAMetalDisplayLink. This is what paces rendering while VSync is enabled (the display link is
	// unpaused in SetRunning); with VSync off the dedicated tick loop drives Tick instead and the display link stays paused.
	_context->SetDrawCallback([this](MTK::View*) {
		Tick(GetDeltaTime());
	});
}

void CocoaApplication::_SetViewEventCallbacks() {
	assert(_context && "CocoaApplication::_SetViewEventCallbacks: Render context must be initialized before setting callbacks");
	assert(_context->viewEventDispatcher && "CocoaApplication::_SetViewEventCallbacks: View event dispatcher must be initialized before setting callbacks");

	const auto eventDispatcher = _context->viewEventDispatcher.get();

	eventDispatcher->mouseDownDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDown>(this));
	eventDispatcher->rightMouseDownDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDown>(this));
	eventDispatcher->otherMouseDownDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDown>(this));

	eventDispatcher->mouseUpDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonUp>(this));
	eventDispatcher->rightMouseUpDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonUp>(this));
	eventDispatcher->otherMouseUpDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonUp>(this));

	eventDispatcher->mouseDraggedDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDragged>(this));
	eventDispatcher->rightMouseDraggedDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDragged>(this));
	eventDispatcher->otherMouseDraggedDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseButtonDragged>(this));

	eventDispatcher->mouseMovedDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnMouseMoved>(this));

	eventDispatcher->keyDownDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnKeyDown>(this));

	eventDispatcher->keyUpDispatcher.Subscribe(EventDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnKeyUp>(this));

	eventDispatcher->viewDidMoveToWindowDispatcher.Subscribe(VoidDelegate::FromMethod<CocoaApplication, &CocoaApplication::_OnViewDidMoveToWindow>(this));
}

void CocoaApplication::_OnWindowWillClose(const NS::Notification*) {
	Events::WindowCloseEvent windowCloseEvent{false};

	DispatchEventToLayers(windowCloseEvent);

	Quit();
}

void CocoaApplication::_OnMouseButtonDown(const NS::Event* event) {
	if (not event)
		return;

	Events::MouseButtonPressedEvent mouseButtonPressedEvent{KeyCode::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchEventToLayers(mouseButtonPressedEvent);
}

void CocoaApplication::_OnMouseButtonUp(const NS::Event* event) {
	if (not event)
		return;

	Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{KeyCode::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchEventToLayers(mouseButtonReleasedEvent);
}

void CocoaApplication::_OnMouseButtonDragged(const NS::Event* event) {
	if (not event)
		return;

	const auto buttonCode = event->buttonNumber();
	const auto [x, y] = _MouseLocationTopLeft(event);
	Events::MouseDraggedEvent mouseDraggedEvent{static_cast<KeyCode::MouseButtonCode>(buttonCode), x, y};
	DispatchEventToLayers(mouseDraggedEvent);
}

void CocoaApplication::_OnMouseMoved(const NS::Event* event) {
	if (not event)
		return;

	const auto [x, y] = _MouseLocationTopLeft(event);
	Events::MouseMovedEvent mouseMovedEvent{x, y};
	DispatchEventToLayers(mouseMovedEvent);
}

std::pair<float, float> CocoaApplication::_MouseLocationTopLeft(const NS::Event* event) const {
	if (not (event and _context and _context->GetView()))
		return {0.0f, 0.0f};

	const auto view = _context->GetView();

	// Cocoa reports the cursor in the window's coordinate system, whose origin is the BOTTOM-left with Y growing upward.
	// Convert it into the view's space, then flip Y so the engine (and ImGui) receive TOP-left origin coordinates with Y
	// growing downward. Without this flip the vertical axis is inverted (moving the mouse down moves the cursor up).
	const auto [x, y] = view->convertPointFromView(event->locationInWindow(), nullptr);
	const auto [frameWidth, frameHeight] = _window->GetFrameSize();

	return {static_cast<float>(x), frameHeight - static_cast<float>(y)};
}

void CocoaApplication::_OnKeyDown(const NS::Event* event) {
	if (not event)
		return;

	Events::KeyPressedEvent keyPressedEvent{KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode()), 0};
	DispatchEventToLayers(keyPressedEvent);

	if (const auto* characters = event->characters()) {
		if (const auto* utf8String = characters->utf8String(); utf8String and utf8String[0] != '\0') {
			Events::KeyTypedEvent keyTypedEvent{KeyCode::KeyboardCharsCodeFromChar(utf8String[0])};
			DispatchEventToLayers(keyTypedEvent);
		}
	}
}

void CocoaApplication::_OnKeyUp(const NS::Event* event) {
	if (not event)
		return;

	Events::KeyReleasedEvent e{KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode())};
	DispatchEventToLayers(e);
}

void CocoaApplication::_OnViewDidMoveToWindow() {
}

}
