//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Layers/ImGuiMetalLayer.hpp"
#include "Core/Application.hpp"
#include "Window/Platforms/Mac/MetalViewport.hpp"
#include "MetalBridge/ImGui/MetalImGuiBridge.h"
#include "Tools/Log/Log.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CE::Layers {

ImGuiMetalLayer::ImGuiMetalLayer(): Layer("ImGuiMetalLayer") {}

void ImGuiMetalLayer::OnAttach() {
	IMGUI_CHECKVERSION();
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
	_viewport = dynamic_cast<Window::MetalViewport*>(app.GetViewport());
	if (not _viewport) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a MetalViewport viewport!");
		exit(EXIT_FAILURE);
	}

	// Cache dei puntatori per evitare lookup ripetuti ogni frame
	_glfwWindow = _viewport->GetGLFWwindow();
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid GLFWwindow!");
		exit(EXIT_FAILURE);
	}
	_metalDevice = _viewport->GetDevice();
	if (not _metalDevice) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
		exit(EXIT_FAILURE);
	}
	_commandQueue = _viewport->GetCommandQueue();
	if (not _commandQueue) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
		exit(EXIT_FAILURE);
	}
	_metalLayer = _viewport->GetMetalLayer();
	if (not _metalLayer) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, true);

	Bridge::ImGuiMetalInit(_metalDevice);
}

void ImGuiMetalLayer::OnDetach() {
	Bridge::ImGuiMetalShutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiMetalLayer::OnUpdate() {
	const auto time = static_cast<float>(glfwGetTime());

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(_viewport->GetWidth()), static_cast<float>(_viewport->GetHeight()));
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	const auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	int width, height;
	glfwGetFramebufferSize(_glfwWindow, &width, &height);
	_metalLayer->setDrawableSize(CGSizeMake(width, height));

	const auto drawable = _metalLayer->nextDrawable();
	if (not drawable) {
		CE_CORE_WARN("Failed to get drawable");
		return;
	}

	const auto commandBuffer = _commandQueue->commandBuffer();

	const auto renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
	const auto colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 0));
	colorAttachment->setTexture(drawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	const auto renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);

	Bridge::ImGuiMetalNewFrame(renderPassDescriptor);
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();

	Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);

	renderEncoder->endEncoding();
	commandBuffer->presentDrawable(drawable);
	commandBuffer->commit();

	renderPassDescriptor->release();
}

void ImGuiMetalLayer::OnEvent(Events::Event&) {}

}
