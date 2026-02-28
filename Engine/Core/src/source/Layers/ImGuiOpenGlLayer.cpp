//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Window/Platforms/Universal/OpenGLViewport.hpp"

#include "Layers/ImGuiOpenGlLayer.hpp"

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


namespace CE::Layers {


ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

void ImGuiOpenGlLayer::OnAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

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

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, false);

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

bool ImGuiOpenGlLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseMoved));
	dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseScrolled));
	dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseButtonPressed));
	dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnMouseButtonReleased));
	dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyPressed));
	dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyReleased));
	dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnKeyTyped));
	dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnWindowResized));
	dispatcher.Dispatch<Events::WindowCloseEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnWindowClosed));
	dispatcher.Dispatch<Events::AppTickEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnAppTick));
	dispatcher.Dispatch<Events::AppUpdateEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnAppUpdate));
	dispatcher.Dispatch<Events::AppRenderEvent>(BIND_EVENT_FN_ONE_PARAM(ImGuiOpenGlLayer::OnAppRender));

	return event.IsHandled();
}

bool ImGuiOpenGlLayer::OnMouseMoved(Events::MouseMovedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnMouseScrolled(Events::MouseScrolledEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnMouseButtonPressed(Events::MouseButtonPressedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnMouseButtonReleased(Events::MouseButtonReleasedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnKeyPressed(Events::KeyPressedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnKeyReleased(Events::KeyReleasedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnKeyTyped(Events::KeyTypedEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnWindowResized(Events::WindowResizeEvent& event) {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, static_cast<int>(event.GetWidth()), static_cast<int>(event.GetHeight()));
	return false;
}

bool ImGuiOpenGlLayer::OnWindowClosed(Events::WindowCloseEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnAppTick(Events::AppTickEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnAppUpdate(Events::AppUpdateEvent&) {
	return false;
}

bool ImGuiOpenGlLayer::OnAppRender(Events::AppRenderEvent&) {
	return false;
}
}
