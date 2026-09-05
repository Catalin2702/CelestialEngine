//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"

#include "Apple/Bridge/ImGui/ImGuiBridge.h"
#include "Core/Application/Application.hpp"
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "Core/Render/Renderer/I_Renderer.hpp"
#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Tools.hpp"
#include "Types/Build/Build.hpp"
#include "Utility/ImGui/ImGui.hpp"

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <imgui.h>


namespace CE::Core {

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer"), _window(std::nullopt) {
}

ImGuiMetalLayer::~ImGuiMetalLayer() {
	// Drop hub subscriptions first so the dispatchers never call back into a half-destroyed layer.
	UnsubscribeFromEventHub();

	// Ensure _Shutdown is called if OnDetach was not called
	_Shutdown();
}

void ImGuiMetalLayer::OnRender() const {
	if (not _currentFrameStarted) [[unlikely]]
		return;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("Hello from ImGui with Metal and Cocoa on macOS!");
	ImGui::Text("Build type: %s", Types::GetCurrentBuildTypeString().c_str());
	ImGui::Text("Application average: %.5f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("Application average delta time: %.5f s", _deltaTime);
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiMetalLayer::SubscribeToEventHub() {
	if (_eventHub) [[unlikely]]
		UnsubscribeFromEventHub();

	// The hub the application owns, named concretely: the Metal-only view-resize channel lives on the derived type.
	auto* const hub = dynamic_cast<CocoaEventHubDispatcher*>(&Application::Get().GetEventHubDispatcher());
	if (not hub) [[unlikely]] {
		constexpr auto error = "ImGuiMetalLayer::SubscribeToEventHub: the Metal ImGui layer needs a Cocoa event hub!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
	_eventHub = *hub;

	_eventHubHandles[MouseMoved] = _eventHub->get().mouseEventHub.onMovedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseMoved>(this));
	_eventHubHandles[MouseDragged] = _eventHub->get().mouseEventHub.onDraggedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseDraggedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseDragged>(this));
	_eventHubHandles[MouseWheelScrolled] = _eventHub->get().mouseEventHub.onWheelScrolledMulticastDispatcher.Subscribe(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseScrolled>(this));
	_eventHubHandles[MouseButtonPressed] = _eventHub->get().mouseEventHub.onButtonPressedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseButtonPressed>(this));
	_eventHubHandles[MouseButtonRelease] = _eventHub->get().mouseEventHub.onButtonReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseButtonReleased>(this));

	_eventHubHandles[KeyboardKeyPressed] = _eventHub->get().keyboardEventHub.onPressedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyPressed>(this));
	_eventHubHandles[KeyboardKeyReleased] = _eventHub->get().keyboardEventHub.onReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyReleased>(this));
	_eventHubHandles[KeyboardCharTyped] = _eventHub->get().keyboardEventHub.onTypedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyTypedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyTyped>(this));

	_eventHubHandles[ViewResize] = _eventHub->get().renderContextEventHub.onResizeViewDispatcher.Subscribe(EventDelegate<Events::ViewResizeEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnViewResized>(this));
}

void ImGuiMetalLayer::UnsubscribeFromEventHub() {
	if (not _eventHub) [[unlikely]]
		return;

	_eventHub->get().mouseEventHub.onMovedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseMoved]);
	_eventHub->get().mouseEventHub.onDraggedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseDragged]);
	_eventHub->get().mouseEventHub.onWheelScrolledMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseWheelScrolled]);
	_eventHub->get().mouseEventHub.onButtonPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonPressed]);
	_eventHub->get().mouseEventHub.onButtonReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonRelease]);

	_eventHub->get().keyboardEventHub.onPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyPressed]);
	_eventHub->get().keyboardEventHub.onReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyReleased]);
	_eventHub->get().keyboardEventHub.onTypedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardCharTyped]);

	_eventHub->get().renderContextEventHub.onResizeViewDispatcher.Unsubscribe(_eventHubHandles[ViewResize]);

	_eventHub = std::nullopt;
	_eventHubHandles = {};
}

void ImGuiMetalLayer::Begin(const f32 deltaTime) {
	_renderSemaphore.acquire();
	_currentFrameStarted = false;
	_deltaTime = deltaTime;

	// The frame's drawable, not a new one: the swapchain acquired it in BeginFrame and will present it in EndFrame.
	// Pulling a second one here - which is what this used to do - would either starve the layer or put two different
	// back buffers on screen for the same frame.
	_frameContext.drawable = _swapchain->GetCurrentDrawable();
	if (not _frameContext.drawable) [[unlikely]] {
		// No drawable means the renderer skipped this frame; the overlay skips it too, and says nothing - the
		// swapchain has already reported it once.
		_renderSemaphore.release();
		return;
	}

	// The frame's buffer, shared with the renderer's passes rather than one of our own: the overlay is another pass of
	// the same frame, and Metal only orders a pass that loads against a pass that wrote within one command buffer.
	_frameContext.commandBuffer = _graphicDevice->GetFrameCommandBuffer();

	const auto renderPassDescriptor = NS::RetainPtr(MTL::RenderPassDescriptor::renderPassDescriptor());

	const auto colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setTexture(_frameContext.drawable->texture());

	// Load, never clear: this is a second pass over a back buffer the scene has already been drawn into.
	colorAttachment->setLoadAction(MTL::LoadActionLoad);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	_frameContext.renderCommandEncoder = _frameContext.commandBuffer->renderCommandEncoder(renderPassDescriptor.get());

	Native::ImGuiMetalNewFrame(renderPassDescriptor.get());

	ImGui::GetIO().DeltaTime = _deltaTime > 0.0f ? _deltaTime : 1.0f / 60.0f;

	ImGui::NewFrame();
	_currentFrameStarted = true;
}

void ImGuiMetalLayer::End() {
	if (not _currentFrameStarted) [[unlikely]]
		return;

	ImGui::Render();
	Native::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), _frameContext.commandBuffer, _frameContext.renderCommandEncoder);

	_frameContext.renderCommandEncoder->endEncoding();

	// Still on this command buffer, and still needed: it paces ImGui's own ring of vertex buffers, which the backend
	// reuses once the GPU is done with them. It has nothing to do with presentation.
	_frameContext.commandBuffer->addCompletedHandler([this](...) {
		_renderSemaphore.release();
	});

	// Neither presented nor committed: the buffer is the frame's, and the swapchain commits it with the present once
	// every pass has closed.
	_frameContext.commandBuffer = nullptr;
}

void ImGuiMetalLayer::_Init() {
	IMGUI_CHECKVERSION();

	try {
		const auto context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		ImGui::StyleColorsDark();

		auto& io = ImGui::GetIO();
		Utility::SetImGuiIniFile(io);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		auto& app = Application::Get();

		auto* const window = dynamic_cast<CocoaWindow*>(&app.GetWindow());
		if (not window) [[unlikely]] {
			constexpr auto error = "ImGuiMetalLayer::_Init: the Metal ImGui layer needs a Cocoa window!";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		_window = *window;

		// Checked, not asserted, and once: the renderer is built before any layer is attached, but a mismatched pair
		// would otherwise show up as a crash inside the ImGui backend rather than as a sentence.
		auto& renderer = app.GetRenderer();
		if (renderer.GetGraphicApi() != Types::GraphicsApi::Metal) [[unlikely]] {
			constexpr auto error = "ImGuiMetalLayer::_Init: the Metal ImGui layer needs a Metal renderer!";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		_graphicDevice = &static_cast<MetalGraphicDevice&>(renderer.GetGraphicDevice());
		_swapchain = &static_cast<MetalSwapchain&>(renderer.GetSwapchain());

		// The backend builds its font atlas and its buffer pool on this device, and never touches the engine's RHI -
		// which is why ImGui needs no I_Texture and no 16-bit index buffer to run here.
		Native::ImGuiMetalInit(_graphicDevice->GetDevice());

		const auto [width, height] = _window->get().GetFrameSize();
		const auto scale = _window->get().GetContentScale();
		const auto safeScale = scale > 0.0f ? scale : 1.0f;

		// Points, with the scale declared separately - the same pair _OnViewResized computes. Setting pixels here and
		// points there left the two disagreeing by the content scale until the first resize arrived, and ImGui derives
		// its render target size from DisplaySize * DisplayFramebufferScale: twice the drawable, so every scissor
		// rectangle landed off the part of the buffer that is actually shown.
		io.DisplaySize = ImVec2(static_cast<f32>(width) / safeScale, static_cast<f32>(height) / safeScale);
		io.DisplayFramebufferScale = ImVec2(safeScale, safeScale);

		_initialized = true;
	}
	catch (...) {
		_initialized = false;
		throw;
	}
}

void ImGuiMetalLayer::_Shutdown() {
	if (not _initialized) [[unlikely]]
		return;
	_initialized = false;

	Native::ImGuiMetalShutdown();
	ImGui::DestroyContext();
}

void ImGuiMetalLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());
}

void ImGuiMetalLayer::_OnMouseDragged(Events::MouseDraggedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());
}

void ImGuiMetalLayer::_OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
}

void ImGuiMetalLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT) [[unlikely]]
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, true);
}

void ImGuiMetalLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT) [[unlikely]]
		return;

	ImGui::GetIO().AddMouseButtonEvent(button, false);
}

void ImGuiMetalLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None) [[unlikely]]
		return;

	ImGui::GetIO().AddKeyEvent(key, true);
}

void ImGuiMetalLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None) [[unlikely]]
		return;

	ImGui::GetIO().AddKeyEvent(key, false);
}

void ImGuiMetalLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const unsigned int codepoint = event.GetKeyCode();
	if (codepoint == 0) [[unlikely]]
		return;

	ImGui::GetIO().AddInputCharacter(codepoint);
}

void ImGuiMetalLayer::_OnViewResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();

	// The event reports the resize in backing pixels (the drawable size). ImGui expects DisplaySize in logical points and
	// applies DisplayFramebufferScale on top, so feeding pixels here would f64-count the scale and render the UI zoomed in
	// with its right/bottom pushed off the drawable. Convert back to points.
	const auto scale = _window ? _window->get().GetContentScale() : 1.0f;
	const auto safeScale = scale > 0.0f ? scale : 1.0f;

	io.DisplaySize = ImVec2(static_cast<f32>(event.GetWidth()) / safeScale, static_cast<f32>(event.GetHeight()) / safeScale);
	io.DisplayFramebufferScale = ImVec2(safeScale, safeScale);
}

}
