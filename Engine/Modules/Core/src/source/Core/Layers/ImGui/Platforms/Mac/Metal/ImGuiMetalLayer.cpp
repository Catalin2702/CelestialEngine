//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"

#include "Apple/Bridge/ImGui/ImGuiBridge.h"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Build/Build.hpp"

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <imgui.h>

namespace CE::Core {

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer"), _context(std::nullopt), _window(std::nullopt) {
}

ImGuiMetalLayer::~ImGuiMetalLayer() {
	// Ensure _Shutdown is called if OnDetach was not called
	_Shutdown();
}

void ImGuiMetalLayer::OnRender() const {
	if (not _currentFrameStarted)
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

void ImGuiMetalLayer::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher dispatcher(event);
	switch (event.GetEventType()) {
		case Events::EventType::MouseMoved:
			dispatcher.Dispatch<Events::MouseMovedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseMoved));
			break;
		case Events::EventType::MouseScrolled:
			dispatcher.Dispatch<Events::MouseWheelScrolledEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseScrolled));
			break;
		case Events::EventType::MouseButtonPressed:
			dispatcher.Dispatch<Events::MouseButtonPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseButtonPressed));
			break;
		case Events::EventType::MouseButtonReleased:
			dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnMouseButtonReleased));
			break;
		case Events::EventType::KeyPressed:
			dispatcher.Dispatch<Events::KeyPressedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyPressed));
			break;
		case Events::EventType::KeyReleased:
			dispatcher.Dispatch<Events::KeyReleasedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyReleased));
			break;
		case Events::EventType::KeyTyped:
			dispatcher.Dispatch<Events::KeyTypedEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnKeyTyped));
			break;
		case Events::EventType::WindowResize:
			dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_FN_ONE_PARAM(ImGuiMetalLayer::_OnWindowResized));
			break;
		default:
			break;
	}
}

void ImGuiMetalLayer::Begin(const float deltaTime) {
	_renderSemaphore.acquire();
	_currentFrameStarted = false;
	_deltaTime = deltaTime;

	_frameContext.drawable = _context->get().AcquireDrawable();
	if (not _frameContext.drawable) {
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
	if (not _currentFrameStarted)
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
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		const auto& app = CocoaApplication::StGet();

		_window = app.GetCocoaWindow();

		_context = app.GetMetalContext();

		Native::ImGuiMetalInit(_context->get().GetDevice());

		// Input is delivered exclusively through the engine event system: the MetalContext view dispatcher translates
		// native events and they reach this layer via OnEvent(). The ImGui OSX platform backend is deliberately NOT
		// initialized, because it installs its own event monitor that would capture the same input a second time.
		// As a consequence this layer owns the platform-side state ImGui still needs: the display size (set here and
		// refreshed in _OnWindowResized) and the delta time (set in Begin()).
		if (const auto view = _context->get().GetView()) {
			const auto [width, height] = _window->get().GetFrameSize();
			const auto scale = static_cast<float>(view->layer() ? view->layer()->contentsScale() : 1.0);

			io.DisplaySize = ImVec2(width, height);
			io.DisplayFramebufferScale = ImVec2(scale, scale);
		}

		_initialized = true;
	}
	catch (...) {
		_initialized = false;
		throw;
	}
}

void ImGuiMetalLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	Native::ImGuiMetalShutdown();
	ImGui::DestroyContext();
}

bool ImGuiMetalLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(event.GetX(), event.GetY());

	return false;
}

bool ImGuiMetalLayer::_OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

	return false;
}

bool ImGuiMetalLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, true);

	return false;
}

bool ImGuiMetalLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const {
	const auto button = Types::ImGuiKeyFromMouseButton(event.GetMouseButton());
	if (button >= ImGuiMouseButton_COUNT)
		return false;

	ImGui::GetIO().AddMouseButtonEvent(button, false);

	return false;
}

bool ImGuiMetalLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, true);

	return false;
}

bool ImGuiMetalLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const {
	const auto key = Types::ImGuiKeyFromKeyboard(event.GetKeyCode());
	if (key == ImGuiKey_None)
		return false;

	ImGui::GetIO().AddKeyEvent(key, false);

	return false;
}

bool ImGuiMetalLayer::_OnKeyTyped(Events::KeyTypedEvent& event) const {
	const unsigned int codepoint = event.GetKeyCode();
	if (codepoint == 0)
		return false;

	ImGui::GetIO().AddInputCharacter(codepoint);

	return false;
}

bool ImGuiMetalLayer::_OnWindowResized(Events::WindowResizeEvent& event) const {
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

	const auto scale = static_cast<float>(_context->get().GetView()->layer()->contentsScale());
	io.DisplayFramebufferScale = ImVec2(scale, scale);

	return false;
}

}
