//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"
#include "CoreAnimation/DisplayLink/DisplayLink.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#include "Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include <AppKit/AppKit.hpp>

#include <cassert>
#include <stdexcept>


namespace CE::Core::Application {

CocoaApplication::CocoaApplication(): _displayLink(nullptr), _context(nullptr), _window(nullptr), _imguiLayer(nullptr), _lastFrameTime(Clock::now()) {
	assert(_stInstance == nullptr && "CocoaApplication already exists!");
	_stInstance = this;

	SetRunning(false);

	_appCocoa = NS::RetainPtr(NS::Application::sharedApplication());

	_appDelegate = std::make_unique<CocoaApplicationDelegate>();
}

CocoaApplication::~CocoaApplication() {
	_appCocoa.reset();
	_appDelegate.reset();
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

	NS::Event* dummyEvent = NS::Event::otherEventWithType(
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
}

void CocoaApplication::Tick(const float) {
	for (const auto layer: _layerStack)
		layer->OnUpdate();

	_imguiLayer->Begin();

	for (const auto layer: _layerStack)
		if (const auto renderLayer = dynamic_cast<Layers::I_RenderLayer*>(layer))
			renderLayer->OnRender();

	_imguiLayer->End();

	_context->SwapBuffers();
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
}

void CocoaApplication::Init(const Types::Window::WindowProps& windowProps) {
	InitWindow(windowProps);
	InitRenderer(windowProps.graphicsApi);
	InitImGuiLayer(windowProps.graphicsApi);

	_SetWindowCallbacks();

	_window->GetReady();
}

void CocoaApplication::_StDisplayLinkCallback(void* userData) {
	if (not userData)
		return;

	const auto app = static_cast<CocoaApplication*>(userData);
	if (not app->IsRunning())
		return;

	const auto currentTime = Clock::now();
	const auto deltaTime = std::chrono::duration<float>(currentTime - app->_lastFrameTime).count();
	app->_lastFrameTime = currentTime;

	app->Tick(deltaTime);
}

void CocoaApplication::StartDisplayLink() {
	if (_displayLink)
		return;

	_displayLink = NS::TransferPtr(CA::DisplayLink::alloc()->init());
	_displayLink->setCallback(&CocoaApplication::_StDisplayLinkCallback, this);

	_lastFrameTime = Clock::now(); // Reset per evitare un grande deltaTime al primo frame
	_displayLink->start();
}

void CocoaApplication::StopDisplayLink() {
	if (not _displayLink)
		return;

	_displayLink.reset();
}

void CocoaApplication::InitWindow(const Types::Window::WindowProps& windowProps) {
	assert(not _window && "CocoaApplication::InitWindow: Window is already initialized!");

	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_window = std::make_unique<Window::CocoaWindow>(windowProps);
	_window->SetEventCallback(BIND_FN_ONE_PARAM(CocoaApplication::OnEvent));

	Input::InitInput(windowProps.windowApi);
}

void CocoaApplication::InitRenderer(Types::Render::GraphicsApi) {
	assert(not _context && "CocoaApplication::InitRenderer: Renderer is already initialized!");
	assert(_window && "CocoaApplication::InitRenderer: Window must be initialized before initializing renderer");

	_context = std::make_unique<Render::Context::MetalContext>(Render::Context::MetalContextProps{
		.window = _window.get(),
		.pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm
	});
	_context->Init();
}

void CocoaApplication::InitImGuiLayer(Types::Render::GraphicsApi) {
	auto overlay = std::make_unique<Layers::ImGuiMetalLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
}

Window::I_Window& CocoaApplication::GetWindow() const {
	return *_window;
}

Render::Context::I_Context& CocoaApplication::GetRenderContext() const {
	return *_context;
}

void CocoaApplication::_SetWindowCallbacks() const {
	assert(_window && "CocoaApplication::SetWindowCallbacks: Window must be initialized before setting callbacks");
	assert(_context && "CocoaApplication::SetWindowCallbacks: Render context must be initialized before setting callbacks");

	_window->SetContentScaleCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleContentSizeChange));
	_window->SetVSyncCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleVSyncChange));
}

}
