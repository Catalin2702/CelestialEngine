//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include "Window/Platforms/Mac/MetalGlfwWindow.hpp"

#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/ImGui/ImGuiBridge.h"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <stdexcept>


namespace CE::Layers {

static int _st_imGuiMetalLayerCount = 0;

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer") {
	_renderSemaphore = dispatch_semaphore_create(_maxFramesInFlight);
}

ImGuiMetalLayer::~ImGuiMetalLayer() {
	_Shutdown();
}

void ImGuiMetalLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with Metal!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.1f FPS", ImGui::GetIO().Framerate);
	if (ImGui::Button("Click Me!")) {
		CE_CORE_INFO("Button clicked!");
	}
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiMetalLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
	case Events::EventType::MouseMoved:
		dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyPressed));
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
	dispatch_semaphore_wait(_renderSemaphore, DISPATCH_TIME_FOREVER);
	_currentFrameStarted = false;

	auto& io = ImGui::GetIO();
	const auto time = static_cast<float>(glfwGetTime());
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	_frameContext.autoreleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	int width, height;
	glfwGetFramebufferSize(_metalContext.glfwWindow, &width, &height);
	_metalContext.metalLayer->setDrawableSize(CGSizeMake(width, height));

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

	Bridge::ImGuiMetalNewFrame(_metalContext.renderPassDescriptor);
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	_currentFrameStarted = true;
}

void ImGuiMetalLayer::End() {
	if (not _currentFrameStarted)
		return;

	ImGui::Render();
	Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), _frameContext.commandBuffer, _frameContext.renderCommandEncoder);

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

void ImGuiMetalLayer::_Init() {
	IMGUI_CHECKVERSION();

	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	const auto& app = Core::Application::Get();

	_metalContext.window = dynamic_cast<Window::MetalGlfwWindow*>(app.GetWindow());

	if (not _metalContext.window) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a MetalGlfwWindow window!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a MetalGlfwWindow window!");
	}

	_metalContext.glfwWindow = static_cast<GLFWwindow*>(_metalContext.window->GetNativeWindow());
	if (not _metalContext.glfwWindow) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid GLFWwindow!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid GLFWwindow!");
	}

	_metalContext.metalDevice = _metalContext.window->GetDevice();
	if (not _metalContext.metalDevice) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::Device!");
	}

	_metalContext.commandQueue = _metalContext.window->GetCommandQueue();
	if (not _metalContext.commandQueue) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
	}

	_metalContext.metalLayer = _metalContext.window->GetMetalLayer();
	if (not _metalContext.metalLayer) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid CA::MetalLayer!");
	}

	_metalContext.renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	if (not _metalContext.renderPassDescriptor) {
		CE_CORE_ERROR("Failed to create MTL::RenderPassDescriptor!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to create MTL::RenderPassDescriptor!");
	}

	if (!ImGui_ImplGlfw_InitForOther(_metalContext.glfwWindow, false)) {
		CE_CORE_ERROR("Failed to initialize ImGui GLFW backend!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to initialize ImGui GLFW backend!");
	}

	_metalContext.metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
	if (const auto contentView = _metalContext.window->GetMetalWindow()->contentView()) {
		contentView->setLayer(_metalContext.metalLayer);
		contentView->setWantsLayer(true);
	} else {
		CE_CORE_ERROR("Failed to get content view from Metal window!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to get content view from Metal window!");
	}

	io.DisplaySize = ImVec2(static_cast<float>(_metalContext.window->GetWidth()), static_cast<float>(_metalContext.window->GetHeight()));

	float x, y;
	glfwGetWindowContentScale(_metalContext.glfwWindow, &x, &y);
	io.DisplayFramebufferScale = ImVec2(x, y);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		auto& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Bridge::ImGuiMetalInit(_metalContext.metalDevice);

	_initialized = true;
	_st_imGuiMetalLayerCount++;
}

void ImGuiMetalLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	_st_imGuiMetalLayerCount--;
	if (_st_imGuiMetalLayerCount > 0)
		return;

	_metalContext.renderPassDescriptor->release();

	Bridge::ImGuiMetalShutdown();
	ImGui_ImplGlfw_Shutdown();
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
	float x, y;
	glfwGetWindowContentScale(_metalContext.glfwWindow, &x, &y);

	io.DisplayFramebufferScale = ImVec2(x, y);

	return false;
}

}
