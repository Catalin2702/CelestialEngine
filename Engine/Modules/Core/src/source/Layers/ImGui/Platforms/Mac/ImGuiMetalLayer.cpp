//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Bridge/ImGui/ImGuiBridge.h"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"
#include "Utility/Time.hpp"
#include "Window/Platforms/Mac/CocoaWindow.hpp"

#include <imgui.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CE::Layers {

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer") {}

ImGuiMetalLayer::~ImGuiMetalLayer() {
	// Ensure _Shutdown is called if OnDetach was not called
	_Shutdown();
}

void ImGuiMetalLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with Metal and Cocoa on macOS!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiMetalLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
		case Events::EventType::MouseMoved:
			dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseMoved));
			break;
		case Events::EventType::MouseScrolled:
			dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseScrolled));
			break;
		case Events::EventType::MouseButtonPressed:
			dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseButtonPressed));
			break;
		case Events::EventType::MouseButtonReleased:
			dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseButtonReleased));
			break;
		case Events::EventType::KeyPressed:
			dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyPressed));
			break;
		case Events::EventType::KeyReleased:
			dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyReleased));
			break;
		case Events::EventType::KeyTyped:
			dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyTyped));
			break;
		case Events::EventType::WindowResize:
			dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnWindowResized));
			break;
		default:
			break;
	}
}

void ImGuiMetalLayer::Begin() {
	_renderSemaphore.acquire();
	_currentFrameStarted = false;

	auto& io = ImGui::GetIO();
	const auto time = Apple::Utility::GetTime();
	io.DeltaTime = _time > 0.0 ? static_cast<float>(time - _time) : 1.0f / 60.0f;
	_time = time;

	const auto scale = _metalContext.window->GetMetalWindow()->backingScaleFactor();
	const auto [width, height] = _metalContext.window->GetSize();
	_metalContext.metalLayer->setDrawableSize({
		static_cast<float>(width) * scale,
		static_cast<float>(height) * scale
	});

	_frameContext.drawable = NS::TransferPtr(_metalContext.metalLayer->nextDrawable());
	if (not _frameContext.drawable) {
		CE_CORE_WARN("Failed to get drawable");
		return;
	}

	_frameContext.commandBuffer = NS::TransferPtr(_metalContext.commandQueue->commandBuffer());

	const auto colorAttachment = _metalContext.renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
	colorAttachment->setTexture(_frameContext.drawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	_frameContext.renderCommandEncoder = _frameContext.commandBuffer->renderCommandEncoder(_metalContext.renderPassDescriptor);

	Apple::Bridge::ImGuiMetalNewFrame(_metalContext.renderPassDescriptor);
	// Apple::Bridge::ImGuiOSXNewFrame(_metalContext.window->GetContentView());

	ImGui::NewFrame();
	_currentFrameStarted = true;
}

void ImGuiMetalLayer::End() {
	if (not _currentFrameStarted)
		return;

	ImGui::Render();
	Apple::Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), _frameContext.commandBuffer.get(), _frameContext.renderCommandEncoder);

	if (const auto& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	_frameContext.renderCommandEncoder->endEncoding();

	_frameContext.commandBuffer->addCompletedHandler([this](...) {
		_renderSemaphore.release();
	});

	_frameContext.commandBuffer->presentDrawable(_frameContext.drawable.get());
	_frameContext.commandBuffer->commit();
}

void ImGuiMetalLayer::_Init() {
	// IMGUI_CHECKVERSION();
	//
	// try {
	// 	const auto context = ImGui::CreateContext();
	// 	ImGui::SetCurrentContext(context);
	// 	ImGui::StyleColorsDark();
	//
	// 	auto& io = ImGui::GetIO();
	// 	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// 	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// 	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Temporarily disabled to debug
	//
	// 	const auto& app = Core::Application::Get();
	//
	// 	_metalContext.window = dynamic_cast<Window::CocoaWindow*>(app.GetWindow());
	// 	if (not _metalContext.window) {
	// 		CE_CORE_ERROR("ImGuiMetalLayer requires a CocoaWindow window!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("ImGuiMetalLayer requires a CocoaWindow window!");
	// 	}
	//
	// 	_metalContext.metalDevice = _metalContext.window->GetDevice();
	// 	if (not _metalContext.metalDevice) {
	// 		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::Device!");
	// 	}
	//
	// 	_metalContext.commandQueue = _metalContext.window->GetCommandQueue();
	// 	if (not _metalContext.commandQueue) {
	// 		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
	// 	}
	//
	// 	_metalContext.metalLayer = _metalContext.window->GetMetalLayer();
	// 	if (not _metalContext.metalLayer) {
	// 		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("ImGuiMetalLayer requires a valid CA::MetalLayer!");
	// 	}
	//
	// 	_metalContext.renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	// 	if (not _metalContext.renderPassDescriptor) {
	// 		CE_CORE_ERROR("Failed to create MTL::RenderPassDescriptor!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("Failed to create MTL::RenderPassDescriptor!");
	// 	}
	//
	// 	Apple::Bridge::ImGuiMetalInit(_metalContext.metalDevice);
	//
	// 	// Initialize OSX backend for platform handling
	// 	if (const auto contentView = _metalContext.window->GetContentView()) {
	// 		if (not Apple::Bridge::ImGuiOSXInit(contentView)) {
	// 			CE_CORE_ERROR("Failed to initialize ImGui OSX backend!");
	// 			ImGui::DestroyContext(context);
	// 			throw std::runtime_error("Failed to initialize ImGui OSX backend!");
	// 		}
	// 	}
	// 	else {
	// 		CE_CORE_ERROR("Failed to get content view for ImGui OSX backend!");
	// 		ImGui::DestroyContext(context);
	// 		throw std::runtime_error("Failed to get content view for ImGui OSX backend!");
	// 	}
	//
	// 	_initialized = true;
	// }
	// catch (...) {
	// 	_initialized = false;
	// 	throw;
	// }
}

void ImGuiMetalLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	if (_metalContext.renderPassDescriptor) {
		_metalContext.renderPassDescriptor->release();
		_metalContext.renderPassDescriptor = nullptr;
	}

	Apple::Bridge::ImGuiMetalShutdown();
	Apple::Bridge::ImGuiOSXShutdown();
	ImGui::DestroyContext();
}

bool ImGuiMetalLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

bool ImGuiMetalLayer::_OnMouseScrolled(Events::MouseScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

bool ImGuiMetalLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, true);

	return false;
}

bool ImGuiMetalLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, false);

	return false;
}

bool ImGuiMetalLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, true);

	return false;
}

bool ImGuiMetalLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, false);

	return false;
}

bool ImGuiMetalLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const auto keycode = KeyCode::ToUInt(event.GetKeyCode());
	if (keycode < KeyCode::KeyboardCharsCode::A || keycode > KeyCode::KeyboardCharsCode::z)
		return false;

	ImGui::GetIO().AddInputCharacter(keycode);

	return false;
}

bool ImGuiMetalLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	const auto [xScale, yScale] = _metalContext.window->GetContentScale();
	io.DisplayFramebufferScale = ImVec2(xScale, yScale);

	return false;
}

}
