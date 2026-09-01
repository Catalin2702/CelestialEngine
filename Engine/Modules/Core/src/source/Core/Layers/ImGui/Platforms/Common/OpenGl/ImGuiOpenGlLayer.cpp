//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"
#include "Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"
#include "Utility/ImGui/ImGui.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace CE::Core {

static int _st_imGuiOpenGlLayerCount = 0;

ImGuiOpenGlLayer::ImGuiOpenGlLayer(): I_ImGuiLayer("ImGuiOpenGlLayer") {}

ImGuiOpenGlLayer::~ImGuiOpenGlLayer() {
	// Drop hub subscriptions first so the dispatchers never call back into a half-destroyed layer.
	UnsubscribeFromEventHub();

	_Shutdown();
}

void ImGuiOpenGlLayer::OnRender() const {
	if (not _currentFrameStarted) [[unlikely]]
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
	if (_eventHub) [[unlikely]]
		UnsubscribeFromEventHub();

	_eventHub = dynamic_cast<GlfwApplication&>(I_Application::StGet()).eventHubDispatcher;

	_eventHubHandles[MouseMoved] = _eventHub->get().mouseEventHub.onMovedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseMoved>(this));
	_eventHubHandles[MouseDragged] = _eventHub->get().mouseEventHub.onDraggedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseDraggedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseDragged>(this));
	_eventHubHandles[MouseWheelScrolled] = _eventHub->get().mouseEventHub.onWheelScrolledMulticastDispatcher.Subscribe(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseScrolled>(this));
	_eventHubHandles[MouseButtonPressed] = _eventHub->get().mouseEventHub.onButtonPressedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonPressed>(this));
	_eventHubHandles[MouseButtonReleased] = _eventHub->get().mouseEventHub.onButtonReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnMouseButtonReleased>(this));

	_eventHubHandles[KeyboardKeyPressed] = _eventHub->get().keyboardEventHub.onPressedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyPressed>(this));
	_eventHubHandles[KeyboardKeyReleased] = _eventHub->get().keyboardEventHub.onReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyReleased>(this));
	_eventHubHandles[KeyboardCharTyped] = _eventHub->get().keyboardEventHub.onTypedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyTypedEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnKeyTyped>(this));

	_eventHubHandles[ViewResize] = _eventHub->get().windowEventHub.onResizeMulticastDispatcher.Subscribe(EventDelegate<Events::WindowResizeEvent&>::FromConstMethod<ImGuiOpenGlLayer, &ImGuiOpenGlLayer::_OnViewResized>(this));
}

void ImGuiOpenGlLayer::UnsubscribeFromEventHub() {
	if (not _eventHub) [[unlikely]]
		return;

	_eventHub->get().mouseEventHub.onMovedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseMoved]);
	_eventHub->get().mouseEventHub.onDraggedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseDragged]);
	_eventHub->get().mouseEventHub.onWheelScrolledMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseWheelScrolled]);
	_eventHub->get().mouseEventHub.onButtonPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonPressed]);
	_eventHub->get().mouseEventHub.onButtonReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonReleased]);

	_eventHub->get().keyboardEventHub.onPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyPressed]);
	_eventHub->get().keyboardEventHub.onReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyReleased]);
	_eventHub->get().keyboardEventHub.onTypedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyPressed]);

	_eventHub->get().windowEventHub.onResizeMulticastDispatcher.Unsubscribe(_eventHubHandles[ViewResize]);

	_eventHub = std::nullopt;
	_eventHubHandles = {};
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

	OpenGlContext::SetViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

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

		// The window and the context are owned by value by GlfwApplication and exposed with their concrete types, so the
		// dynamic_cast through the I_Window / I_Context interfaces (which only hand out const references) is gone: there is
		// nothing left to check at runtime either, a reference cannot be null and the type is fixed at compile time.
		auto& app = GlfwApplication::StGet();

		_window = app.GetGlfwWindow();
		_context = app.GetOpenGlContext();

		// io.DisplaySize must be in the same (logical/screen) coordinate space as the mouse position
		// events fed in _OnMouseMoved/_OnMouseDragged - those come straight from GLFW's cursor
		// callback, which always reports screen coordinates, never framebuffer pixels. Using the
		// framebuffer size here (GetFrameSize()) instead of the window size desyncs the two on any
		// display where the OS content scale isn't 1:1 (e.g. Windows at 125%/150% scaling), which is
		// what caused clicks to land off from the visible cursor by a roughly constant offset.
		// DisplayFramebufferScale is the separate framebuffer/window ratio used to scale draw calls
		// for crisp rendering on such displays, and is unaffected by this.
		const auto [width, height] = _window->get().GetWindowSize();
		const auto [xScale, yScale] = _context->get().GetContentScale();

		io.DisplaySize = ImVec2(width,height);
		io.DisplayFramebufferScale = ImVec2(xScale, yScale);

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

	const auto [xScale, yScale] = _context->get().GetContentScale();
	io.DisplayFramebufferScale = ImVec2(xScale, yScale);
}

}
