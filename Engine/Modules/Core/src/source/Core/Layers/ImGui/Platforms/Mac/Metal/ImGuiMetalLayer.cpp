//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"

#include "Apple/Bridge/ImGui/ImGuiBridge.h"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Core/MainHub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/ImGui/ImGui.hpp"
#include "Types/Build/Build.hpp"

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <imgui.h>


namespace CE::Core {

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer"), _context(std::nullopt), _window(std::nullopt) {
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

	_frameContext.renderCommandEncoder->setRenderPipelineState(CocoaApplication::StGet().defaultRenderPipelineState);

	_frameContext.renderCommandEncoder->drawPrimitives(
		MTL::PrimitiveType::PrimitiveTypeTriangle,
		static_cast<NS::UInteger>(0),
		3
	);
}

void ImGuiMetalLayer::SubscribeToEventHub() {
	if (_eventHub) [[unlikely]]
		UnsubscribeFromEventHub();

	_eventHub = dynamic_cast<CocoaApplication&>(I_Application::StGet()).eventHubDispatcher;

	_eventHubHandles[MouseMoved] = _eventHub->get().cocoaMouseEventHub.onMovedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseMoved>(this));
	_eventHubHandles[MouseDragged] = _eventHub->get().cocoaMouseEventHub.onDraggedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseDraggedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseDragged>(this));
	_eventHubHandles[MouseWheelScrolled] = _eventHub->get().cocoaMouseEventHub.onWheelScrolledMulticastDispatcher.Subscribe(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseScrolled>(this));
	_eventHubHandles[MouseButtonPressed] = _eventHub->get().cocoaMouseEventHub.onButtonPressedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseButtonPressed>(this));
	_eventHubHandles[MouseButtonRelease] = _eventHub->get().cocoaMouseEventHub.onButtonReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnMouseButtonReleased>(this));

	_eventHubHandles[KeyboardKeyPressed] = _eventHub->get().cocoaKeyboardEventHub.onPressedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyPressed>(this));
	_eventHubHandles[KeyboardKeyReleased] = _eventHub->get().cocoaKeyboardEventHub.onReleasedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyReleased>(this));
	_eventHubHandles[KeyboardCharTyped] = _eventHub->get().cocoaKeyboardEventHub.onTypedMulticastDispatcher.Subscribe(EventDelegate<Events::KeyTypedEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnKeyTyped>(this));

	_eventHubHandles[ViewResize] = _eventHub->get().metalRenderContextEventHub.onResizeViewDispatcher.Subscribe(EventDelegate<Events::ViewResizeEvent&>::FromConstMethod<ImGuiMetalLayer, &ImGuiMetalLayer::_OnViewResized>(this));
}

void ImGuiMetalLayer::UnsubscribeFromEventHub() {
	if (not _eventHub) [[unlikely]]
		return;

	_eventHub->get().cocoaMouseEventHub.onMovedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseMoved]);
	_eventHub->get().cocoaMouseEventHub.onDraggedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseDragged]);
	_eventHub->get().cocoaMouseEventHub.onWheelScrolledMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseWheelScrolled]);
	_eventHub->get().cocoaMouseEventHub.onButtonPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonPressed]);
	_eventHub->get().cocoaMouseEventHub.onButtonReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[MouseButtonRelease]);

	_eventHub->get().cocoaKeyboardEventHub.onPressedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyPressed]);
	_eventHub->get().cocoaKeyboardEventHub.onReleasedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardKeyReleased]);
	_eventHub->get().cocoaKeyboardEventHub.onTypedMulticastDispatcher.Unsubscribe(_eventHubHandles[KeyboardCharTyped]);

	_eventHub->get().metalRenderContextEventHub.onResizeViewDispatcher.Unsubscribe(_eventHubHandles[ViewResize]);

	_eventHub = std::nullopt;
	_eventHubHandles = {};
}

void ImGuiMetalLayer::Begin(const float deltaTime) {
	_renderSemaphore.acquire();
	_currentFrameStarted = false;
	_deltaTime = deltaTime;

	_frameContext.drawable = _context->get().AcquireDrawable();
	if (not _frameContext.drawable) [[unlikely]] {
		CE_CORE_WARN("Failed to get drawable");
		_renderSemaphore.release();
		return;
	}

	_frameContext.commandBuffer = _context->get().GetCommandQueue()->commandBuffer();

	const auto renderPassDescriptor = NS::RetainPtr(MTL::RenderPassDescriptor::renderPassDescriptor());

	const auto colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 0));
	colorAttachment->setTexture(_frameContext.drawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
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

	_frameContext.commandBuffer->addCompletedHandler([this](...) {
		_renderSemaphore.release();
	});

	_frameContext.commandBuffer->presentDrawable(_frameContext.drawable);
	_frameContext.commandBuffer->commit();
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

		// Non-const: the window and the context are owned by value by CocoaApplication, so the const getters hand out const
		// references, which the (non-const) cached reference_wrappers below cannot bind to.
		auto& app = CocoaApplication::StGet();

		_window = app.GetCocoaWindow();

		_context = app.GetMetalContext();

		Native::ImGuiMetalInit(_context->get().GetDevice());

		const auto [width, height] = _window->get().GetFrameSize();
		const auto [xScale, yScale] = _context->get().GetContentScale();

		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(xScale, yScale);

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

	// The context reports the resize in backing pixels (the drawable size). ImGui expects DisplaySize in logical points and
	// applies DisplayFramebufferScale on top, so feeding pixels here would double-count the scale and render the UI zoomed in
	// with its right/bottom pushed off the drawable. Convert back to points.
	const auto scale = static_cast<float>(_context->get().GetView()->layer()->contentsScale());
	const auto safeScale = scale > 0.0f ? scale : 1.0f;

	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()) / safeScale, static_cast<float>(event.GetHeight()) / safeScale);
	io.DisplayFramebufferScale = ImVec2(safeScale, safeScale);
}

}
