//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Window/Platforms/Universal/OpenGLViewport.hpp"
#include "Core/Application.hpp"
#include "Layers/ImGuiOpenGlLayer.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


namespace CE::Layers {


ImGuiOpenGlLayer::ImGuiOpenGlLayer(): Layer("ImGuiOpenGlLayer") {}


void ImGuiOpenGlLayer::OnAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	io.AddKeyEvent(ImGuiKey_Tab, true);
	io.AddKeyEvent(ImGuiKey_LeftArrow, true);
	io.AddKeyEvent(ImGuiKey_RightArrow, true);
	io.AddKeyEvent(ImGuiKey_UpArrow, true);
	io.AddKeyEvent(ImGuiKey_DownArrow, true);
	io.AddKeyEvent(ImGuiKey_PageUp, true);
	io.AddKeyEvent(ImGuiKey_PageDown, true);
	io.AddKeyEvent(ImGuiKey_Home, true);
	io.AddKeyEvent(ImGuiKey_End, true);
	io.AddKeyEvent(ImGuiKey_Insert, true);
	io.AddKeyEvent(ImGuiKey_Delete, true);
	io.AddKeyEvent(ImGuiKey_Backspace, true);
	io.AddKeyEvent(ImGuiKey_Space, true);
	io.AddKeyEvent(ImGuiKey_Enter, true);
	io.AddKeyEvent(ImGuiKey_Escape, true);
	io.AddKeyEvent(ImGuiKey_KeypadEnter, true);
	io.AddKeyEvent(ImGuiKey_A, true);
	io.AddKeyEvent(ImGuiKey_C, true);
	io.AddKeyEvent(ImGuiKey_V, true);
	io.AddKeyEvent(ImGuiKey_X, true);
	io.AddKeyEvent(ImGuiKey_Y, true);
	io.AddKeyEvent(ImGuiKey_Z, true);

	const auto& app = Core::Application::Get();
	_viewport = dynamic_cast<Window::OpenGLViewport*>(app.GetViewport());
	if (not _viewport) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires an OpenGLViewport viewport!");
		exit(EXIT_FAILURE);
	}
	_glfwWindow = _viewport->GetGLFWwindow();
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiOpenGlLayer requires a valid GLFWwindow!");
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, true);

	ImGui_ImplOpenGL3_Init("#version 410");
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
}

void ImGuiOpenGlLayer::OnDetach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

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

void ImGuiOpenGlLayer::OnEvent(Events::Event&) {
}
}
