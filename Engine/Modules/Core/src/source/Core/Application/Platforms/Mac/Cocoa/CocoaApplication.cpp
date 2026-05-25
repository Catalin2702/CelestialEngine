//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Define/Bind.hpp"

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"
#include "Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "MetalCpp/AppKit/AppKit.hpp"
#include "MetalCpp/QuartzCore/QuartzCore.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/EventHandlers/DisplayLinkEventHandler.hpp"

#include <cassert>
#include <stdexcept>


namespace CE::Core::Application {

CocoaApplication::CocoaApplication(): _displayLink(nullptr), _context(nullptr), _window(nullptr), _imguiLayer(nullptr) {
	assert(_stInstance == nullptr && "CocoaApplication::CocoaApplication: CocoaApplication already exists!");
	_stInstance = this;

	SetRunning(false);

	_appCocoa = NS::RetainPtr(NS::Application::sharedApplication());

	_appDelegate = std::make_unique<CocoaApplicationDelegate>();
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

	_context.reset();
	_window.reset();

	_appDelegate.reset();
	_appCocoa.reset();
}

void CocoaApplication::Run() {
	_appCocoa->setDelegate(_appDelegate.get());

	if (_appCocoa->activationPolicy() != NS::ActivationPolicyRegular) {
		if (not _appCocoa->setActivationPolicy(NS::ActivationPolicyRegular)) {
			CE_CORE_ERROR("CocoaApplication::Run: Failed to set activation policy for the application");
			throw std::runtime_error("Failed to set activation policy for the application");
		}
	}

	_appCocoa->activateIgnoringOtherApps(true);

	_appCocoa->run();
}

void CocoaApplication::Quit() {
	StopDisplayLink();

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
	SetRunning(false);
}

void CocoaApplication::Tick(const float deltaTime) {
	for (const auto layer: _layerStack)
		layer->OnUpdate();

	_imguiLayer->Begin(deltaTime);

	for (const auto layer: _layerStack)
		if (const auto renderLayer = dynamic_cast<Layers::I_RenderLayer*>(layer))
			renderLayer->OnRender();

	_imguiLayer->End();
}

void CocoaApplication::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher eventDispatcher{event};

	switch (event.GetEventType()) {
		case Events::EventType::WindowClose: {
			eventDispatcher.Dispatch<Events::WindowCloseEvent>([this](const Events::WindowCloseEvent& e) {
				Quit();
				e.Consume();
				return true;
			});
			break;
		}
		default:
			break;
	}

	if (event.IsHandled())
		return;

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(event);
		if (event.IsHandled())
			break;
	}
}

void CocoaApplication::Init(const Types::Window::WindowProps& windowProps) {
	_InitWindow(windowProps);
	_InitRenderer(windowProps.graphicsApi);

	_SetWindowCallbacks();

	_window->GetReady(windowProps.VSync);
}

void CocoaApplication::StartDisplayLink() {
	assert(IsRunning() && "CocoaApplication::StartDisplayLink: Application must be running to start display link");
	assert(_window && "CocoaApplication::StartDisplayLink: Window must be initialized before starting display link");
	assert(_context && "CocoaApplication::StartDisplayLink: Renderer must be initialized before starting display link");
	assert(_imguiLayer && "CocoaApplication::StartDisplayLink: ImGui layer must be initialized before starting display link");

	if (_window->IsVSync()) {
		assert(_displayLink && "CocoaApplication::StartDisplayLink: Display link is already running!");

		_displayLink = NS::TransferPtr(CA::DisplayLink::alloc()->init());
		_displayLinkEventHandler = std::make_unique<Apple::Types::DisplayLinkEventHandler>();
		_displayLinkEventHandler->OnTick([this]() {
			if (not IsRunning())
				return;
			Tick(GetDeltaTime());
		});

		_lastFrameTime = Clock::now(); // Reset to avoid a large deltaTime on the first frame
		_displayLink->start();
	}
	else {
		_loopThread = std::thread([this] {
			while (_isRunning.load(std::memory_order_acquire)) {
				if (not _tickPending.exchange(true)) {
					dispatch_async_f(dispatch_get_main_queue(), this, &CocoaApplication::_StAsyncTickCallback);
				}
			}
		});
	}
}

void CocoaApplication::StopDisplayLink() {
	if (not _displayLink)
		return;

	_displayLink.reset();
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

void CocoaApplication::_InitWindow(const Types::Window::WindowProps& windowProps) {
	assert(not _window && "CocoaApplication::InitWindow: Window is already initialized!");

	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_window = std::make_unique<Window::CocoaWindow>(windowProps);
	_window->SetEventCallback(BIND_FN_ONE_PARAM(CocoaApplication::OnEvent));

	Input::InitInput(windowProps.windowApi);
}

void CocoaApplication::_InitRenderer(Types::Render::GraphicsApi) {
	assert(not _context && "CocoaApplication::InitRenderer: Renderer is already initialized!");
	assert(_window && "CocoaApplication::InitRenderer: Window must be initialized before initializing renderer");

	_context = std::make_unique<Render::Context::MetalContext>();
	_context->props.pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
	_context->Init();
}

void CocoaApplication::InitImGuiLayer(Types::Render::GraphicsApi) {
	assert(_window && "CocoaApplication::InitImGuiLayer: Window must be initialized before initializing ImGui layer");
	assert(_context && "CocoaApplication::InitImGuiLayer: Renderer must be initialized before initializing ImGui layer");
	assert(not _imguiLayer && "CocoaApplication::InitImGuiLayer: ImGui layer is already initialized!");

	auto overlay = std::make_unique<Layers::ImGuiMetalLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
}

Window::I_Window& CocoaApplication::GetWindow() const {
	return *_window;
}

Window::CocoaWindow& CocoaApplication::GetCocoaWindow() const {
	return *_window;
}

Render::Context::MetalContext& CocoaApplication::GetMetalContext() const {
	return *_context;
}

Render::Context::I_Context& CocoaApplication::GetRenderContext() const {
	return *_context;
}

void CocoaApplication::_StAsyncTickCallback(void* userData) {
	if (const auto app = static_cast<CocoaApplication*>(userData)) {
		app->_tickPending.store(false);
		if (app->_isRunning.load(std::memory_order_acquire)) {
			app->Tick(app->GetDeltaTime());
		}
	}
}

void CocoaApplication::_SetWindowCallbacks() const {
	assert(_window && "CocoaApplication::SetWindowCallbacks: Window must be initialized before setting callbacks");
	assert(_context && "CocoaApplication::SetWindowCallbacks: Render context must be initialized before setting callbacks");

	_window->SetContentScaleCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleContentSizeChange));
	_window->SetVSyncCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleVSyncChange));
}

}
