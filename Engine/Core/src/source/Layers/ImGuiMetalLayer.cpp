//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Window/Platforms/Mac/MetalViewport.hpp"

#include "Layers/ImGuiMetalLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/ImGui/MetalImGuiBridge.h"
#include "Tools/ImGui/ImGui.hpp"
#include "Tools/Log/Log.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CE::Layers {

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer") {}

void ImGuiMetalLayer::OnAttach() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	const auto& app = Core::Application::Get();
	_viewport = dynamic_cast<Window::MetalViewport*>(app.GetViewport());
	if (not _viewport) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a MetalViewport viewport!");
		exit(EXIT_FAILURE);
	}

	// Cache dei puntatori per evitare lookup ripetuti ogni frame
	_glfwWindow = _viewport->GetGLFWwindow();
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid GLFWwindow!");
		exit(EXIT_FAILURE);
	}
	_metalDevice = _viewport->GetDevice();
	if (not _metalDevice) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
		exit(EXIT_FAILURE);
	}
	_commandQueue = _viewport->GetCommandQueue();
	if (not _commandQueue) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
		exit(EXIT_FAILURE);
	}
	_metalLayer = _viewport->GetMetalLayer();
	if (not _metalLayer) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfw_InitForOther(_glfwWindow, false);

	Bridge::ImGuiMetalInit(_metalDevice);
}

void ImGuiMetalLayer::OnDetach() {
	Bridge::ImGuiMetalShutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiMetalLayer::OnUpdate() {
	const auto time = static_cast<float>(glfwGetTime());

	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(_viewport->GetWidth()), static_cast<float>(_viewport->GetHeight()));
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	const auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	int width, height;
	glfwGetFramebufferSize(_glfwWindow, &width, &height);
	_metalLayer->setDrawableSize(CGSizeMake(width, height));

	const auto drawable = _metalLayer->nextDrawable();
	if (not drawable) {
		CE_CORE_WARN("Failed to get drawable");
		return;
	}

	const auto commandBuffer = _commandQueue->commandBuffer();

	const auto renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	const auto colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
	colorAttachment->setTexture(drawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	const auto renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);

	Bridge::ImGuiMetalNewFrame(renderPassDescriptor);
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with Metal!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	if (ImGui::Button("Click Me!")) {
		CE_CORE_INFO("Button clicked!");
	}
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();

	Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);

	renderEncoder->endEncoding();
	commandBuffer->presentDrawable(drawable);
	commandBuffer->commit();

	renderPassDescriptor->release();
}

bool ImGuiMetalLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);

	switch (event.GetEventType()) {
		case Events::EventType::KeyPressed: {
			if (dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnKeyPressed))) return event.IsHandled();
			break;
		}
		case Events::EventType::KeyReleased: {
			if (dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnKeyReleased))) return event.IsHandled();
			break;
		}
		case Events::EventType::KeyTyped: {
			if (dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnKeyTyped))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseButtonPressed: {
			if (dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnMouseButtonPressed))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseButtonReleased: {
			if (dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnMouseButtonReleased))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseMoved: {
			if (dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnMouseMoved))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseScrolled: {
			if (dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnMouseScrolled))) return event.IsHandled();
			break;
		}
		case Events::EventType::WindowResize: {
			if (dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::OnWindowResized))) return event.IsHandled();
			break;
		}
		default:
			return false;
	}

	return false;
}

bool ImGuiMetalLayer::OnKeyPressed(Events::KeyPressedEvent& event) {
	auto& io = ImGui::GetIO();
	const ImGuiKey key = Tools::ImGui::GlfwKeyToImGuiKey(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	io.AddKeyEvent(key, true);

	if (key == ImGuiKey_LeftCtrl || key == ImGuiKey_RightCtrl) {
		io.AddKeyEvent(ImGuiMod_Ctrl, true);
		return false;
	}
	if (key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift) {
		io.AddKeyEvent(ImGuiMod_Shift, true);
		return false;
	}
	if (key == ImGuiKey_LeftAlt || key == ImGuiKey_RightAlt) {
		io.AddKeyEvent(ImGuiMod_Alt, true);
		return false;
	}
	if (key == ImGuiKey_LeftSuper || key == ImGuiKey_RightSuper) {
		io.AddKeyEvent(ImGuiMod_Super, true);
		return false;
	}

	return false;
}

bool ImGuiMetalLayer::OnKeyReleased(Events::KeyReleasedEvent& event) {
	auto& io = ImGui::GetIO();
	const ImGuiKey key = Tools::ImGui::GlfwKeyToImGuiKey(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	io.AddKeyEvent(key, false);

	if (key == ImGuiKey_LeftCtrl || key == ImGuiKey_RightCtrl) {
		io.AddKeyEvent(ImGuiMod_Ctrl, false);
		return false;
	}
	if (key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift) {
		io.AddKeyEvent(ImGuiMod_Shift, false);
		return false;
	}
	if (key == ImGuiKey_LeftAlt || key == ImGuiKey_RightAlt) {
		io.AddKeyEvent(ImGuiMod_Alt, false);
		return false;
	}
	if (key == ImGuiKey_LeftSuper || key == ImGuiKey_RightSuper) {
		io.AddKeyEvent(ImGuiMod_Super, false);
		return false;
	}

	return false;
}

bool ImGuiMetalLayer::OnKeyTyped(Events::KeyTypedEvent& event) {
	auto& io = ImGui::GetIO();
	if (const auto keycode = event.GetKeyCode(); keycode > 0 && keycode < 0x10000) {
		io.AddInputCharacter(static_cast<unsigned short>(keycode));
	}
	return false;
}

bool ImGuiMetalLayer::OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = true;

	return false;
}

bool ImGuiMetalLayer::OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = false;

	return false;
}

bool ImGuiMetalLayer::OnMouseMoved(Events::MouseMovedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MousePos = ImVec2(event.GetX(), event.GetY());

	return false;
}

bool ImGuiMetalLayer::OnMouseScrolled(Events::MouseScrolledEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseWheelH += event.GetXOffset();
	io.MouseWheel += event.GetYOffset();

	return false;
}

bool ImGuiMetalLayer::OnWindowResized(Events::WindowResizeEvent& event) {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	return false;
}

}
