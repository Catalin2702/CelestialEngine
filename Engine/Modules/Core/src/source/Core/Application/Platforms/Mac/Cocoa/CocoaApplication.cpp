//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-05-29
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Define/Bind.hpp"

#include "Apple/MetalCpp/AppKit/AppKit.hpp"
#include "Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Config/Config.hpp"

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

	_context.reset();
	_window.reset();

	_appCocoa.reset();
}

void CocoaApplication::Run() {
	_appCocoa->setDelegate(&_appDelegate);

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

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();

		_imguiLayer->End();
	}
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

void CocoaApplication::Init() {
	assert(not _window && "CocoaApplication::Init: Window is already initialized!");
	assert(not _context && "CocoaApplication::Init: Renderer is already initialized!");

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	if (not Types::Window::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("CocoaApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_context = std::make_unique<Render::Context::MetalContext>();
	_context->Init();

	_window = std::make_unique<Window::CocoaWindow>();
	_window->SetEventCallback(BIND_FN_ONE_PARAM(CocoaApplication::OnEvent));
	_window->Init(_context->GetDevice());

	if (const auto view = _window->GetViewController()->view()) {
		_context->SetView(view);
		view->setPaused(true);
		view->setEnableSetNeedsDisplay(false);
		view->setDelegate(&_renderViewDelegate);
	}

	Input::InitInput(windowProps.windowApi);

	_SetWindowCallbacks();

	_window->GetReady(windowProps.VSync);
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
		if (const auto view = _context->GetView()) {
			view->setPaused(not running);
		}
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

void CocoaApplication::_SetWindowCallbacks() const {
	assert(_window && "CocoaApplication::SetWindowCallbacks: Window must be initialized before setting callbacks");
	assert(_context && "CocoaApplication::SetWindowCallbacks: Render context must be initialized before setting callbacks");

	_window->SetContentScaleCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleContentSizeChange));
	_window->SetVSyncCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::MetalContext::HandleVSyncChange));
}

}
