//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>


namespace CE::Core::Layers {

static int _st_imGuiOpenGlLayerCount = 0;

ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

ImGuiOpenGlLayer::~ImGuiOpenGlLayer() {
	_Shutdown();
}

void ImGuiOpenGlLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with OpenGL and GLFW!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("Application average delta time: %.5f s", _deltaTime);
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiOpenGlLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
	case Events::EventType::MouseMoved:
		dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnMouseMoved));
		break;
	case Events::EventType::MouseScrolled:
		dispatcher.Dispatch<Events::MouseScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnMouseScrolled));
		break;
	case Events::EventType::MouseButtonPressed:
		dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnMouseButtonPressed));
		break;
	case Events::EventType::MouseButtonReleased:
		dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnMouseButtonReleased));
		break;
	case Events::EventType::KeyPressed:
		dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnKeyPressed));
		break;
	case Events::EventType::KeyReleased:
		dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnKeyReleased));
		break;
	case Events::EventType::KeyTyped:
		dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnKeyTyped));
		break;
	case Events::EventType::WindowResize:
		dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiOpenGlLayer::_OnWindowResized));
		break;
	default:
		break;
	}
}

void ImGuiOpenGlLayer::Begin(const float deltaTime) {
	_currentFrameStarted = false;
	_deltaTime = deltaTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::GetIO().DeltaTime = _deltaTime > 0.0f ? _deltaTime : 1.0f / 60.0f;

	ImGui::NewFrame();

	_currentFrameStarted = true;
}

void ImGuiOpenGlLayer::End() {
	if (not _currentFrameStarted)
		return;

	ImGui::Render();

	const auto [width, height] = _window->get().GetFrameBufferSize();

	Render::Context::OpenGlContext::SetViewport(0, 0, width, height);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (const auto& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		_window->get().SetCurrentContext();
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

	const auto& app = Core::Application::GlfwApplication::StGet();

	_window = dynamic_cast<Window::GlfwWindow&>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an GlfwWindow window!");
		throw std::runtime_error("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an GlfwWindow window!");
	}

	_context = dynamic_cast<Render::Context::OpenGlContext&>(app.GetRenderContext());
	if (not _context) {
		CE_CORE_ERROR("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an OpenGlContext context!");
		throw std::runtime_error("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an OpenGlContext context!");
	}

	io.DisplaySize = ImVec2(_window->get().GetWindowWidth(),_window->get().GetWindowHeight());

	assert(_window->get().GetGlfwWindow() != nullptr);

	const auto [x, y] = _window->get().GetContentScale();

	io.DisplayFramebufferScale = ImVec2(x, y);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(_window->get().GetGlfwWindow(), false);

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

bool ImGuiOpenGlLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

bool ImGuiOpenGlLayer::_OnMouseScrolled(Events::MouseScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

bool ImGuiOpenGlLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, true);

	return false;
}

bool ImGuiOpenGlLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = KeyCode::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, false);

	return false;
}

bool ImGuiOpenGlLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, true);

	return false;
}

bool ImGuiOpenGlLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = KeyCode::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, false);

	return false;
}

bool ImGuiOpenGlLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const auto keycode = KeyCode::ToUInt(event.GetKeyCode());
	if (keycode < KeyCode::KeyboardCharsCode::A || keycode > KeyCode::KeyboardCharsCode::z)
		return false;

	ImGui::GetIO().AddInputCharacter(keycode);

	return false;
}

bool ImGuiOpenGlLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	const auto [xScale, yScale] = _window->get().GetContentScale();
	io.DisplayFramebufferScale = ImVec2(xScale, yScale);

	return false;
}

}
