//
// Module: Layers
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-24
// Updated: 2026-02-28
//

#include "Window/Platforms/Universal/OpenGlViewport.hpp"

#include "Layers/ImGuiOpenGlLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/ImGui/ImGui.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


namespace CE::Layers {

/**
 * @brief ImGuiOpenGlLayer constructor implementation
 * @details Initializes the layer with the name "ImGuiOpenGlLayer"
 */
ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

/**
 * @brief Attaches the ImGui OpenGL layer to the application
 * @details Performs the following initialization:
 *          - Creates ImGui context and applies dark theme
 *          - Configures ImGui backend flags for mouse support
 *          - Caches OpenGL viewport and GLFW window pointers
 *          - Initializes ImGui_ImplGlfw backend
 *          - Initializes ImGui OpenGL3 rendering backend
 *          - Sets up custom dark theme colors
 *          Exits with error if viewport is not an OpenGLViewport or if initialization fails
 */
void ImGuiOpenGlLayer::OnAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	const auto& app = Core::Application::Get();
	_viewport = dynamic_cast<Window::OpenGlViewport*>(app.GetViewport());
	if (not _viewport) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires an OpenGlViewport viewport!");
		exit(EXIT_FAILURE);
	}
	_glfwWindow = _viewport->GetGLFWwindow();
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires a valid GLFWwindow!");
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, false);

	ImGui_ImplOpenGL3_Init("#version 410");
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
}

/**
 * @brief Detaches the ImGui OpenGL layer from the application
 * @details Shuts down ImGui backends (OpenGL3 and GLFW) and destroys ImGui context
 */
void ImGuiOpenGlLayer::OnDetach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/**
 * @brief Updates the ImGui OpenGL layer every frame
 * @details Performs per-frame rendering:
 *          - Starts new ImGui frame (OpenGL3, GLFW, and ImGui)
 *          - Renders ImGui demo window
 *          - Finalizes ImGui rendering and renders to OpenGL
 */
void ImGuiOpenGlLayer::OnUpdate() {
	const auto time = static_cast<float>(glfwGetTime());

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = {static_cast<float>(_viewport->GetWidth()), static_cast<float>(_viewport->GetHeight())};
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	// Clear the buffer before rendering
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
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
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * @brief Dispatches events to appropriate handlers
 * @param event Reference to the event to process
 * @return bool True if event was handled
 * @details Uses EventDispatcher to route events to specific handler methods
 */
bool ImGuiOpenGlLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);

	switch (event.GetEventType()) {
		case Events::EventType::KeyPressed: {
			if (dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyPressed))) return event.IsHandled();
			break;
		}
		case Events::EventType::KeyReleased: {
			if (dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyReleased))) return event.IsHandled();
			break;
		}
		case Events::EventType::KeyTyped: {
			if (dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyTyped))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseButtonPressed: {
			if (dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseButtonPressed))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseButtonReleased: {
			if (dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseButtonReleased))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseMoved: {
			if (dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseMoved))) return event.IsHandled();
			break;
		}
		case Events::EventType::MouseScrolled: {
			if (dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseScrolled))) return event.IsHandled();
			break;
		}
		case Events::EventType::WindowResize: {
			if (dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::OnWindowResized))) return event.IsHandled();
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
bool ImGuiOpenGlLayer::OnKeyPressed(Events::KeyPressedEvent& event) {
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
bool ImGuiOpenGlLayer::OnKeyReleased(Events::KeyReleasedEvent& event) {
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
bool ImGuiOpenGlLayer::OnKeyTyped(Events::KeyTypedEvent& event) {
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
bool ImGuiOpenGlLayer::OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = true;

	return false;
}

/**
 * @brief Handles mouse button release events
 * @param event Mouse button released event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiOpenGlLayer::OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = false;

	return false;
}

/**
 * @brief Handles mouse moved events
 * @param event Mouse moved event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiOpenGlLayer::OnMouseMoved(Events::MouseMovedEvent& event) {
	auto& io = ImGui::GetIO();
	io.MousePos = ImVec2(event.GetX(), event.GetY());

	return false;
}

/**
 * @brief Handles mouse scroll events
 * @param event Mouse scrolled event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiOpenGlLayer::OnMouseScrolled(Events::MouseScrolledEvent& event) {
	auto& io = ImGui::GetIO();
	io.MouseWheelH += event.GetXOffset();
	io.MouseWheel += event.GetYOffset();

	return false;
}

/**
 * @brief Handles window resize events
 * @param event Window resize event
 * @return bool Always returns false to allow event propagation
 */
bool ImGuiOpenGlLayer::OnWindowResized(Events::WindowResizeEvent& event) {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, static_cast<int>(event.GetWidth()), static_cast<int>(event.GetHeight()));
	return false;
}

}
