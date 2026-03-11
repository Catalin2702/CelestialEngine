//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-11
//

#include "Window/Platforms/Universal/OpenGlWindow.hpp"

#include "Layers/ImGui/Platforms/Universal/ImGuiOpenGlLayer.hpp"

#include "Core/Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Tools/Log/Log.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


namespace CE::Layers {

static int _st_imGuiOpenGlLayerCount = 0;

ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

ImGuiOpenGlLayer::~ImGuiOpenGlLayer() {
	_Shutdown();
}

void ImGuiOpenGlLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	const auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with OpenGL!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	if (ImGui::Button("Click Me!")) {
		CE_CORE_INFO("Button clicked!");
	}
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiOpenGlLayer::Begin() {
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

void ImGuiOpenGlLayer::End() {
	if (not _currentFrameStarted)
		return;

	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(_window->GetWidth()), static_cast<float>(_window->GetHeight()));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiOpenGlLayer::_Init() {
	IMGUI_CHECKVERSION();

	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	const auto& app = Core::Application::Get();

	_window = dynamic_cast<Window::OpenGlWindow*>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires an OpenGlWindow window!");
		throw std::runtime_error("ImGuiOpenGlLayer requires an OpenGlWindow window!");
	}

	_glfwWindow = static_cast<GLFWwindow*>(_window->GetNativeWindow());
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires a valid GLFWwindow!");
		throw std::runtime_error("ImGuiOpenGlLayer requires a valid GLFWwindow!");
	}

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, false);

	ImGui_ImplOpenGL3_Init("#version 410");

	_initialized = true;
	_st_imGuiOpenGlLayerCount++;
}

void ImGuiOpenGlLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	_st_imGuiOpenGlLayerCount--;
	if (_st_imGuiOpenGlLayerCount > 0)
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

}
