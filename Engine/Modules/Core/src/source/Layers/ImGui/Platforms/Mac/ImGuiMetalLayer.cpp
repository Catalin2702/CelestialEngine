//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-06
//

#include "Window/Platforms/Mac/MetalWindow.hpp"

#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/ImGui/ImGuiBridge.h"
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

#include <stdexcept>


namespace CE::Layers {

/**
 * @brief ImGuiMetalLayer constructor implementation
 * @details Initializes the layer with the name "ImGuiMetalLayer"
 */
ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer") {}

/**
 * @brief Attaches the ImGui Metal layer to the application
 * @details Performs the following initialization:
 *			- Creates ImGui context and applies dark theme
 *			- Configures ImGui backend flags for mouse support
 *			- Caches Metal window and GLFW window pointers
 *			- Initializes ImGui_ImplGlfw backend
 *			- Initializes ImGui Metal rendering backend
 *			- Sets up custom dark theme colors
 *			Throws std::runtime_error if window is not a MetalWindow or if initialization fails.
 */
void ImGuiMetalLayer::OnAttach() {
	IMGUI_CHECKVERSION();
	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	const auto& app = Core::Application::Get();
	_window = dynamic_cast<Window::MetalWindow*>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a MetalWindow window!");
		throw std::runtime_error("ImGuiMetalLayer requires a MetalWindow window!");
	}

	// Cache dei puntatori per evitare lookup ripetuti ogni frame
	_glfwWindow = static_cast<GLFWwindow*>(_window->GetNativeWindow());
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid GLFWwindow!");
		throw std::runtime_error("ImGuiMetalLayer requires a valid GLFWwindow!");
	}
	_metalDevice = _window->GetDevice();
	if (not _metalDevice) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::Device!");
	}
	_commandQueue = _window->GetCommandQueue();
	if (not _commandQueue) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
	}
	_metalLayer = _window->GetMetalLayer();
	if (not _metalLayer) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
		throw std::runtime_error("ImGuiMetalLayer requires a valid CA::MetalLayer!");
	}

	ImGui_ImplGlfw_InitForOther(_glfwWindow, false);

	Bridge::ImGuiMetalInit(_metalDevice);
}

/**
 * @brief Detaches the ImGui Metal layer from the application
 * @details Shuts down ImGui backends (Metal and GLFW) and destroys ImGui context
 */
void ImGuiMetalLayer::OnDetach() {
	Bridge::ImGuiMetalShutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/**
 * @brief Updates the ImGui Metal layer every frame
 * @details Performs per-frame rendering:
 *			- Creates Metal command buffer and render pass descriptor
 *			- Starts new ImGui frame (Metal, GLFW, and ImGui)
 *			- Renders ImGui demo window
 *			- Finalizes ImGui rendering and encodes to Metal command buffer
 *			- Presents drawable to screen
 */
void ImGuiMetalLayer::OnUpdate() const {
	const auto time = static_cast<float>(glfwGetTime());

	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(_window->GetWidth()), static_cast<float>(_window->GetHeight()));
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	[[maybe_unused]] const auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

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

/**
 * @brief Dispatches events to appropriate handlers
 * @param event Reference to the event to process
 * @return bool True if event was handled
 * @details Uses EventDispatcher to route events to specific handler methods
 */
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

// Protected event handlers
/**
 * @brief Handles key press events
 * @param event Key pressed event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnKeyPressed(Events::KeyPressedEvent& event) const {
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

/**
 * @brief Handles key release events
 * @param event Key released event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnKeyReleased(Events::KeyReleasedEvent& event) const {
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

/**
 * @brief Handles key typed events (character input)
 * @param event Key typed event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnKeyTyped(Events::KeyTypedEvent& event) const {
	auto& io = ImGui::GetIO();
	if (const auto keycode = event.GetKeyCode(); keycode > 0 && keycode < 0x10000) {
		io.AddInputCharacter(static_cast<unsigned short>(keycode));
	}
	return false;
}

/**
 * @brief Handles mouse button press events
 * @param event Mouse button pressed event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.GetMouseButton(), true);

	return false;
}

/**
 * @brief Handles mouse button release events
 * @param event Mouse button released event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.GetMouseButton(), false);

	return false;
}

/**
 * @brief Handles mouse moved events
 * @param event Mouse moved event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

/**
 * @brief Handles mouse scroll events
 * @param event Mouse scrolled event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnMouseScrolled(Events::MouseScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

/**
 * @brief Handles window resize events
 * @param event Window resize event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiMetalLayer::OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	return false;
}

}
