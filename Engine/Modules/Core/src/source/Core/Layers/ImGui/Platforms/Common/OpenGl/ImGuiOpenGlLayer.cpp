//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/MainHub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"
#include "Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

namespace CE::Core {

static int _st_imGuiOpenGlLayerCount = 0;

ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

ImGuiOpenGlLayer::~ImGuiOpenGlLayer() {
	// Drop hub subscriptions first so the dispatchers never call back into a half-destroyed layer.
	UnsubscribeFromEventHub();

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

void ImGuiOpenGlLayer::SubscribeToEventHub() {
	if (_eventHub)
		UnsubscribeFromEventHub();

	_eventHub = dynamic_cast<GlfwApplication&>(I_Application::StGet()).eventHubDispatcher;

	_eventHubHandles[0] = _eventHub->get().glfwMouseEventHub.onMovedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseMoved>(this));
	_eventHubHandles[1] = _eventHub->get().glfwMouseEventHub.onWheelScrolledMulticastDispatcher.Subscribe(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseScrolled>(this));
	_eventHubHandles[2] = _eventHub->get().glfwMouseEventHub.onButtonPressedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonPressed>(this));
	_eventHubHandles[3] = _eventHub->get().glfwMouseEventHub.onButtonReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonReleased>(this));
	_eventHubHandles[4] = _eventHub->get().glfwKeyboardEventHub.onPressedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyPressed>(this));
	_eventHubHandles[5] = _eventHub->get().glfwKeyboardEventHub.onReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyReleased>(this));
	_eventHubHandles[6] = _eventHub->get().glfwKeyboardEventHub.onTypedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyTypedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyTyped>(this));
	_eventHubHandles[7] = _eventHub->get().glfwWindowEventHub.onResizeMulticastDispatcher.Subscribe(EventDelegate<Events::WindowResizeEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnWindowResized>(this));
}

void ImGuiOpenGlLayer::UnsubscribeFromEventHub() {
	if (not _eventHub)
		return;

	_eventHub->get().glfwMouseEventHub.onMovedMulticastDispatcher.Unsubscribe(_eventHubHandles[0]);
	_eventHub->get().glfwMouseEventHub.onWheelScrolledMulticastDispatcher.Unsubscribe(_eventHubHandles[1]);
	_eventHub->get().glfwMouseEventHub.onButtonPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[2]);
	_eventHub->get().glfwMouseEventHub.onButtonReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[3]);
	_eventHub->get().glfwKeyboardEventHub.onPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[4]);
	_eventHub->get().glfwKeyboardEventHub.onReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[5]);
	_eventHub->get().glfwKeyboardEventHub.onTypedMulticastDispatcher.Unsubscribe(_eventHubHandles[6]);
	_eventHub->get().glfwWindowEventHub.onResizeMulticastDispatcher.Unsubscribe(_eventHubHandles[7]);

	_eventHub = std::nullopt;
	_eventHubHandles = {};
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

	const auto [width, height] = _window->get().GetFrameSize();

	OpenGlContext::SetViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

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
	// Multi-viewport (detaching ImGui windows into separate OS windows) is intentionally NOT enabled: on macOS a viewport
	// window dragged out while the main window is fullscreen escapes into a new Space and makes GLFW abort in
	// _glfwInputWindowContentScale (assert xscale > 0). Keeping windows inside the main window avoids it, and matches the
	// Metal layer which never enabled viewports either.
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	const auto& app = GlfwApplication::StGet();

	_window = dynamic_cast<GlfwWindow&>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an GlfwWindow window!");
		throw std::runtime_error("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an GlfwWindow window!");
	}

	_context = dynamic_cast<OpenGlContext&>(app.GetRenderContext());
	if (not _context) {
		CE_CORE_ERROR("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an OpenGlContext context!");
		throw std::runtime_error("ImGuiOpenGlLayer::_Init: ImGuiOpenGlLayer requires an OpenGlContext context!");
	}

	const auto [width, height] = _window->get().GetFrameSize();

	io.DisplaySize = ImVec2(width,height);

	assert(_window->get().GetGlfwWindow() != nullptr);

	// const auto [x, y] = _window->get().GetContentScale();
	//
	// io.DisplayFramebufferScale = ImVec2(x, y);

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

void ImGuiOpenGlLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());
}

void ImGuiOpenGlLayer::_OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
}

void ImGuiOpenGlLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, true);
}

void ImGuiOpenGlLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, false);
}

void ImGuiOpenGlLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return;

	ImGui::GetIO().AddKeyEvent(key, true);
}

void ImGuiOpenGlLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return;

	ImGui::GetIO().AddKeyEvent(key, false);
}

void ImGuiOpenGlLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const unsigned int codepoint = event.GetKeyCode();
	if (codepoint == 0)
		return;

	ImGui::GetIO().AddInputCharacter(codepoint);
}

void ImGuiOpenGlLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	// const auto [xScale, yScale] = _window->get().GetContentScale();
	// io.DisplayFramebufferScale = ImVec2(xScale, yScale);
}

}
