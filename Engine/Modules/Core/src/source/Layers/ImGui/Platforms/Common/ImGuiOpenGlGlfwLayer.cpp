//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Common
// File: ImGuiOpenGlGlfwLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#include "Window/Platforms/Common/GlfwWindow.hpp"

#include "Layers/ImGui/Platforms/Common/ImGuiOpenGlGlfwLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"

#include <glad/glad.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


namespace CE::Layers {

static int _st_imGuiOpenGlGlfwLayerCount = 0;

ImGuiOpenGlGlfwLayer::ImGuiOpenGlGlfwLayer(): I_ImGuiLayer("ImGuiOpenGlGlfwLayer") {}

ImGuiOpenGlGlfwLayer::~ImGuiOpenGlGlfwLayer() {
	_Shutdown();
}

void ImGuiOpenGlGlfwLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with OpenGL and GLFW!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiOpenGlGlfwLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
	case Events::EventType::MouseMoved:
		dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnMouseMoved));
		break;
	case Events::EventType::MouseScrolled:
		dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnMouseScrolled));
		break;
	case Events::EventType::MouseButtonPressed:
		dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnMouseButtonPressed));
		break;
	case Events::EventType::MouseButtonReleased:
		dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnMouseButtonReleased));
		break;
	case Events::EventType::KeyPressed:
		dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnKeyPressed));
		break;
	case Events::EventType::KeyReleased:
		dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnKeyReleased));
		break;
	case Events::EventType::KeyTyped:
		dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnKeyTyped));
		break;
	case Events::EventType::WindowResize:
		dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlGlfwLayer::_OnWindowResized));
		break;
	default:
		break;
	}
}

void ImGuiOpenGlGlfwLayer::Begin() {
	_currentFrameStarted = false;

	auto& io = ImGui::GetIO();
	const auto time = static_cast<float>(glfwGetTime());
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	_currentFrameStarted = true;
}

void ImGuiOpenGlGlfwLayer::End() {
	if (not _currentFrameStarted)
		return;

	ImGui::Render();

	int width, height;
	glfwGetFramebufferSize(_glfwWindow, &width, &height);

	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (const auto& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiOpenGlGlfwLayer::_Init() {
	IMGUI_CHECKVERSION();

	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	const auto& app = Core::Application::Get();

	_window = dynamic_cast<Window::GlfwWindow*>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiOpenGlGlfwLayer requires an GlfwWindow window!");
		throw std::runtime_error("ImGuiOpenGlGlfwLayer requires an GlfwWindow window!");
	}

	io.DisplaySize = ImVec2(static_cast<float>(_window->GetWidth()), static_cast<float>(_window->GetHeight()));

	_glfwWindow = static_cast<GLFWwindow*>(_window->GetNativeWindow());
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiOpenGlGlfwLayer requires a valid GLFWwindow!");
		throw std::runtime_error("ImGuiOpenGlGlfwLayer requires a valid GLFWwindow!");
	}

	float x, y;
	glfwGetWindowContentScale(_glfwWindow, &x, &y);
	io.DisplayFramebufferScale = ImVec2(x, y);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, false);

	ImGui_ImplOpenGL3_Init("#version 410");

	_initialized = true;
	_st_imGuiOpenGlGlfwLayerCount++;
}

void ImGuiOpenGlGlfwLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	_st_imGuiOpenGlGlfwLayerCount--;
	if (_st_imGuiOpenGlGlfwLayerCount > 0)
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool ImGuiOpenGlGlfwLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnMouseScrolled(Events::MouseScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, true);

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, false);

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, true);

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, false);

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const auto keycode = KeyCode::ToUInt(event.GetKeyCode());
	if (keycode < KeyCode::KeyboardCharsCode::A || keycode > KeyCode::KeyboardCharsCode::z)
		return false;

	ImGui::GetIO().AddInputCharacter(keycode);

	return false;
}

bool ImGuiOpenGlGlfwLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	float x, y;
	glfwGetWindowContentScale(_glfwWindow, &x, &y);
	io.DisplayFramebufferScale = ImVec2(x, y);

	return false;
}

}
