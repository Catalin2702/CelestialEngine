//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"

#include "Core/Application/Application.hpp"
#include "Core/Hub/Events/I_EventHubDispatcher.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Types/Build/Build.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/ImGui/ImGui.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace CE::Core {

static int _st_imGuiOpenGlLayerCount = 0;

ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

ImGuiOpenGlLayer::~ImGuiOpenGlLayer() {
	// Explicit, though _subscriptions would release itself a moment later anyway: the dispatchers must stop being able
	// to call back into this layer before _Shutdown starts tearing its ImGui context down.
	UnsubscribeFromEventHub();

	_Shutdown();
}

void ImGuiOpenGlLayer::OnRender() const {
	if (not _currentFrameStarted) [[unlikely]]
		return;

	const auto& windowProps = Utility::Config::GetWindowProps();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	Utility::ImguiText("W-API: {}", windowProps.windowApi);
	Utility::ImguiText("G-API: {}", windowProps.graphicsApi);
	Utility::ImguiText("Build: {}", Types::GetCurrentBuildTypeString().c_str());
	Utility::ImguiText("FPS: {:.1f}", ImGui::GetIO().Framerate);
	Utility::ImguiText("Delta: {:.5f} s", _deltaTime);
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiOpenGlLayer::SubscribeToEventHub(I_EventHubDispatcher& eventHubDispatcher) {
	UnsubscribeFromEventHub();

	// Reached through the interface: the hub's subscribable channels are the same on every backend, so this layer
	// neither names one nor has to know which application it belongs to.
	_subscriptions[MouseMoved] = eventHubDispatcher.GetMouseEventHub().onMovedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseMoved>(this));
	_subscriptions[MouseDragged] = eventHubDispatcher.GetMouseEventHub().onDraggedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseDraggedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseDragged>(this));
	_subscriptions[MouseWheelScrolled] = eventHubDispatcher.GetMouseEventHub().onWheelScrolledMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseScrolled>(this));
	_subscriptions[MouseButtonPressed] = eventHubDispatcher.GetMouseEventHub().onButtonPressedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonPressed>(this));
	_subscriptions[MouseButtonReleased] = eventHubDispatcher.GetMouseEventHub().onButtonReleasedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonReleased>(this));
	_subscriptions[KeyboardKeyPressed] = eventHubDispatcher.GetKeyboardEventHub().onPressedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyPressed>(this));
	_subscriptions[KeyboardKeyReleased] = eventHubDispatcher.GetKeyboardEventHub().onReleasedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyReleased>(this));
	_subscriptions[KeyboardCharTyped] = eventHubDispatcher.GetKeyboardEventHub().onTypedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::KeyTypedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyTyped>(this));
	_subscriptions[ViewResize] = eventHubDispatcher.GetWindowEventHub().onResizeMulticastDispatcher.SubscribeScoped(EventDelegate<Events::WindowResizeEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnViewResized>(this));
}

void ImGuiOpenGlLayer::UnsubscribeFromEventHub() {
	// Assigning an empty array over the live one runs every token's Reset: no hub to reach for, and nothing to keep
	// in step with the enum by hand.
	_subscriptions = {};
}

void ImGuiOpenGlLayer::Begin(const f32 deltaTime) {
	_currentFrameStarted = false;
	_deltaTime = deltaTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::GetIO().DeltaTime = _deltaTime > 0.0f ? _deltaTime : 1.0f / 60.0f;

	ImGui::NewFrame();

	_currentFrameStarted = true;
}

void ImGuiOpenGlLayer::End() {
	if (not _currentFrameStarted) [[unlikely]]
		return;

	ImGui::Render();

	const auto [width, height] = _window->get().GetFrameSize();

	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiOpenGlLayer::_Init() {
	IMGUI_CHECKVERSION();

	try {
		const auto context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		Utility::SetImGuiIniFile(io);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// The application holds its window behind I_Window, but the ImGui GLFW backend wants the native handle, so this
		// layer - which is the GLFW one - asks for the concrete type. It is the only place that needs it.
		_window = dynamic_cast<GlfwWindow&>(Application::Get().GetWindow());

		// io.DisplaySize must be in the same (logical/screen) coordinate space as the mouse position
		// events fed in _OnMouseMoved/_OnMouseDragged - those come straight from GLFW's cursor
		// callback, which always reports screen coordinates, never framebuffer pixels. Using the
		// framebuffer size here (GetFrameSize()) instead of the window size desyncs the two on any
		// display where the OS content scale isn't 1:1 (e.g. Windows at 125%/150% scaling), which is
		// what caused clicks to land off from the visible cursor by a roughly constant offset.
		// DisplayFramebufferScale is the separate framebuffer/window ratio used to scale draw calls
		// for crisp rendering on such displays, and is unaffected by this.
		const auto [width, height] = _window->get().GetWindowSize();
		const auto contentScale = _window->get().GetContentScale();

		io.DisplaySize = ImVec2(static_cast<f32>(width), static_cast<f32>(height));
		io.DisplayFramebufferScale = ImVec2(contentScale, contentScale);

		assert(_window->get().GetGlfwWindow() != nullptr);

		ImGui_ImplGlfw_InitForOpenGL(_window->get().GetGlfwWindow(), false);

		ImGui_ImplOpenGL3_Init("#version 410");

		_initialized = true;
		_st_imGuiOpenGlLayerCount++;
	}
	catch (...) {
		_initialized = false;
		throw;
	}

}

void ImGuiOpenGlLayer::_Shutdown() {
	if (not _initialized) [[unlikely]]
		return;
	_initialized = false;

	_st_imGuiOpenGlLayerCount--;
	if (_st_imGuiOpenGlLayerCount > 0) [[unlikely]]
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiOpenGlLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());
}

void ImGuiOpenGlLayer::_OnMouseDragged(Events::MouseDraggedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());
}

void ImGuiOpenGlLayer::_OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
}

void ImGuiOpenGlLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT) [[unlikely]]
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, true);
}

void ImGuiOpenGlLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT) [[unlikely]]
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, false);
}

void ImGuiOpenGlLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None) [[unlikely]]
		return;

	ImGui::GetIO().AddKeyEvent(key, true);
}

void ImGuiOpenGlLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None) [[unlikely]]
		return;

	ImGui::GetIO().AddKeyEvent(key, false);
}

void ImGuiOpenGlLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const unsigned int codepoint = event.GetKeyCode();
	if (codepoint == 0) [[unlikely]]
		return;

	ImGui::GetIO().AddInputCharacter(codepoint);
}

void ImGuiOpenGlLayer::_OnViewResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<f32>(event.GetWidth()), static_cast<f32>(event.GetHeight()));

	// The scale can change without a resize (dragging onto a display with a different one), and it can change with a
	// resize too, so it is re-read here rather than cached at init.
	const auto contentScale = _window->get().GetContentScale();
	io.DisplayFramebufferScale = ImVec2(contentScale, contentScale);
}

}
