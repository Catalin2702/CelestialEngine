//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalCocoaLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#include "Layers/ImGui/Platforms/Mac/ImGuiMetalCocoaLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/ImGui/ImGuiBridge.h"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"
#include "Utility/Time.hpp"
#include "Window/Platforms/Mac/MetalCocoaWindow.hpp"

#include <imgui.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CE::Layers {

static int _st_imGuiMetalCocoaLayerCount = 0;

ImGuiMetalCocoaLayer::ImGuiMetalCocoaLayer(): I_ImGuiLayer("ImGuiMetalCocoaLayer") {
	_renderSemaphore = dispatch_semaphore_create(_maxFramesInFlight);
}

ImGuiMetalCocoaLayer::~ImGuiMetalCocoaLayer() {
	dispatch_release(_renderSemaphore);
	_renderSemaphore = nullptr;
	_Shutdown();
}

void ImGuiMetalCocoaLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with Metal and Cocoa on macOS!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.1f FPS", ImGui::GetIO().Framerate);
	if (ImGui::Button("Click Me!")) {
		CE_CORE_INFO("Button clicked!");
	}
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiMetalCocoaLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
		case Events::EventType::MouseMoved:
			dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnMouseMoved));
			break;
		case Events::EventType::MouseScrolled:
			dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnMouseScrolled));
			break;
		case Events::EventType::MouseButtonPressed:
			dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnMouseButtonPressed));
			break;
		case Events::EventType::MouseButtonReleased:
			dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnMouseButtonReleased));
			break;
		case Events::EventType::KeyPressed:
			dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnKeyPressed));
			break;
		case Events::EventType::KeyReleased:
			dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnKeyReleased));
			break;
		case Events::EventType::KeyTyped:
			dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnKeyTyped));
			break;
		case Events::EventType::WindowResize:
			dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiMetalCocoaLayer::_OnWindowResized));
			break;
		default:
			break;
	}
}

void ImGuiMetalCocoaLayer::Begin() {
	dispatch_semaphore_wait(_renderSemaphore, DISPATCH_TIME_FOREVER);
	_currentFrameStarted = false;

	auto& io = ImGui::GetIO();
	const auto time = Apple::Utility::GetTime();
	io.DeltaTime = _time > 0.0 ? static_cast<float>(time - _time) : 1.0f / 60.0f;
	_time = time;

	_frameContext.autoreleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	const auto scale = _metalContext.window->GetMetalWindow()->backingScaleFactor();
	const auto [width, height] = _metalContext.window->GetSize();
	_metalContext.metalLayer->setDrawableSize({
		static_cast<float>(width) * scale,
		static_cast<float>(height) * scale
	});

	_frameContext.drawable = _metalContext.metalLayer->nextDrawable();
	if (not _frameContext.drawable) {
		CE_CORE_WARN("Failed to get drawable");
		return;
	}

	_frameContext.commandBuffer = _metalContext.commandQueue->commandBuffer();

	const auto colorAttachment = _metalContext.renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
	colorAttachment->setTexture(_frameContext.drawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	_frameContext.renderCommandEncoder = _frameContext.commandBuffer->renderCommandEncoder(_metalContext.renderPassDescriptor);

	Apple::Bridge::ImGuiMetalNewFrame(_metalContext.renderPassDescriptor);
	Apple::Bridge::ImGuiOSXNewFrame(_metalContext.window->GetContentView());

	ImGui::NewFrame();
	_currentFrameStarted = true;
}

void ImGuiMetalCocoaLayer::End() {
	if (not _currentFrameStarted)
		return;

	ImGui::Render();
	Apple::Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), _frameContext.commandBuffer, _frameContext.renderCommandEncoder);

	if (const auto& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	_frameContext.renderCommandEncoder->endEncoding();

	_frameContext.commandBuffer->addCompletedHandler([this](...) {
		dispatch_semaphore_signal(_renderSemaphore);
	});

	_frameContext.commandBuffer->presentDrawable(_frameContext.drawable);
	_frameContext.commandBuffer->commit();

	_frameContext.autoreleasePool.reset();
}

void ImGuiMetalCocoaLayer::_Init() {
	IMGUI_CHECKVERSION();
	
	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Temporarily disabled to debug

	const auto& app = Core::Application::Get();

	_metalContext.window = dynamic_cast<Window::MetalCocoaWindow*>(app.GetWindow());
	if (not _metalContext.window) {
		CE_CORE_ERROR("ImGuiMetalCocoaLayer requires a MetalCocoaWindow window!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalCocoaLayer requires a MetalCocoaWindow window!");
	}

	_metalContext.metalDevice = _metalContext.window->GetDevice();
	if (not _metalContext.metalDevice) {
		CE_CORE_ERROR("ImGuiMetalCocoaLayer requires a valid MTL::Device!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalCocoaLayer requires a valid MTL::Device!");
	}

	_metalContext.commandQueue = _metalContext.window->GetCommandQueue();
	if (not _metalContext.commandQueue) {
		CE_CORE_ERROR("ImGuiMetalCocoaLayer requires a valid MTL::CommandQueue!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalCocoaLayer requires a valid MTL::CommandQueue!");
	}

	_metalContext.metalLayer = _metalContext.window->GetMetalLayer();
	if (not _metalContext.metalLayer) {
		CE_CORE_ERROR("ImGuiMetalCocoaLayer requires a valid CA::MetalLayer!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalCocoaLayer requires a valid CA::MetalLayer!");
	}

	_metalContext.renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	if (not _metalContext.renderPassDescriptor) {
		CE_CORE_ERROR("Failed to create MTL::RenderPassDescriptor!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to create MTL::RenderPassDescriptor!");
	}

	Apple::Bridge::ImGuiMetalInit(_metalContext.metalDevice);

	// Initialize OSX backend for platform handling
	if (const auto contentView = _metalContext.window->GetContentView()) {
		if (not Apple::Bridge::ImGuiOSXInit(contentView)) {
			CE_CORE_ERROR("Failed to initialize ImGui OSX backend!");
			ImGui::DestroyContext(context);
			throw std::runtime_error("Failed to initialize ImGui OSX backend!");
		}
		CE_CORE_INFO("ImGui OSX backend initialized successfully");
	}
	else {
		CE_CORE_ERROR("Failed to get content view for ImGui OSX backend!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to get content view for ImGui OSX backend!");
	}
}

void ImGuiMetalCocoaLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	_metalContext.renderPassDescriptor->release();

	_st_imGuiMetalCocoaLayerCount--;
	if (_st_imGuiMetalCocoaLayerCount > 0)
		return;

	Apple::Bridge::ImGuiMetalShutdown();
	Apple::Bridge::ImGuiOSXShutdown();
	ImGui::DestroyContext();
}

bool ImGuiMetalCocoaLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

bool ImGuiMetalCocoaLayer::_OnMouseScrolled(Events::MouseScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

bool ImGuiMetalCocoaLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, true);

	return false;
}

bool ImGuiMetalCocoaLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, false);

	return false;
}

bool ImGuiMetalCocoaLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, true);

	return false;
}

bool ImGuiMetalCocoaLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, false);

	return false;
}

bool ImGuiMetalCocoaLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const auto keycode = KeyCode::ToUInt(event.GetKeyCode());
	if (keycode < KeyCode::KeyboardCharsCode::A || keycode > KeyCode::KeyboardCharsCode::z)
		return false;

	ImGui::GetIO().AddInputCharacter(keycode);

	return false;
}

bool ImGuiMetalCocoaLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	const CGFloat frameBufferScale = _metalContext.window->GetMetalWindow()->backingScaleFactor();

	io.DisplayFramebufferScale = ImVec2(static_cast<float>(frameBufferScale), static_cast<float>(frameBufferScale));

	return false;
}

}
