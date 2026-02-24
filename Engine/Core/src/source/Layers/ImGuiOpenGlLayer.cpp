//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Core/Application.hpp"
#include "Layers/ImGuiOpenGlLayer.hpp"
#include "Window/Platforms/Universal/OpenGLViewport.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>
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

	const auto window = static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow());
	ImGui_ImplGlfw_InitForOpenGL(window, true);

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
	const auto window = dynamic_cast<Window::OpenGLViewport*>(Core::Application::Get().GetWindow());
	const auto time = static_cast<float>(glfwGetTime());

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = {static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight())};
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiOpenGlLayer::OnEvent(Events::Event&) {
}
}
