//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiMetalLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-11
//

#include "Window/Platforms/Mac/MetalWindow.hpp"

#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"

#include "Core/Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "MetalBridge/ImGui/ImGuiBridge.h"
#include "Tools/Log/Log.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <stdexcept>


namespace CE::Layers {

static int _st_imGuiMetalLayerCount = 0;

ImGuiMetalLayer::ImGuiMetalLayer(): I_ImGuiLayer("ImGuiMetalLayer") {}

ImGuiMetalLayer::~ImGuiMetalLayer() {
	_Shutdown();
}

void ImGuiMetalLayer::Begin() {
	_currentFrameStarted = false;

	auto& io = ImGui::GetIO();
	const auto time = static_cast<float>(glfwGetTime());
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	_currentAutoreleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	int width, height;
	glfwGetFramebufferSize(_glfwWindow, &width, &height);
	_metalLayer->setDrawableSize(CGSizeMake(width, height));

	_currentDrawable = _metalLayer->nextDrawable();
	if (not _currentDrawable) {
		CE_CORE_WARN("Failed to get drawable");
		return;
	}

	_currentCommandBuffer = _commandQueue->commandBuffer();
	_currentRenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();

	const auto colorAttachment = _currentRenderPassDescriptor->colorAttachments()->object(0);
	colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
	colorAttachment->setTexture(_currentDrawable->texture());
	colorAttachment->setLoadAction(MTL::LoadActionClear);
	colorAttachment->setStoreAction(MTL::StoreActionStore);

	_currentRenderCommandEncoder = _currentCommandBuffer->renderCommandEncoder(_currentRenderPassDescriptor);

	Bridge::ImGuiMetalNewFrame(_currentRenderPassDescriptor);
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	_currentFrameStarted = true;
}

void ImGuiMetalLayer::OnRender() const {
	if (not _currentFrameStarted)
		return;

	const auto& io = ImGui::GetIO();
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
}

void ImGuiMetalLayer::End() {
	if (not _currentFrameStarted)
		return;

	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(_window->GetWidth()), static_cast<float>(_window->GetHeight()));

	ImGui::Render();
	Bridge::ImGuiMetalRenderDrawData(ImGui::GetDrawData(), _currentCommandBuffer, _currentRenderCommandEncoder);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	_currentRenderCommandEncoder->endEncoding();
	_currentCommandBuffer->presentDrawable(_currentDrawable);
	_currentCommandBuffer->commit();
	_currentRenderPassDescriptor->release();

	_currentAutoreleasePool.reset();
}

void ImGuiMetalLayer::_Init() {
	IMGUI_CHECKVERSION();

	const auto context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	const auto& app = Core::Application::Get();

	_window = dynamic_cast<Window::MetalWindow*>(app.GetWindow());
	if (not _window) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a MetalWindow window!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a MetalWindow window!");
	}

	_glfwWindow = static_cast<GLFWwindow*>(_window->GetNativeWindow());
	if (not _glfwWindow) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid GLFWwindow!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid GLFWwindow!");
	}

	_metalDevice = _window->GetDevice();
	if (not _metalDevice) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::Device!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::Device!");
	}

	_commandQueue = _window->GetCommandQueue();
	if (not _commandQueue) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid MTL::CommandQueue!");
	}

	_metalLayer = _window->GetMetalLayer();
	if (not _metalLayer) {
		CE_CORE_ERROR("ImGuiMetalLayer requires a valid CA::MetalLayer!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("ImGuiMetalLayer requires a valid CA::MetalLayer!");
	}
	_metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

	if (!ImGui_ImplGlfw_InitForOther(_glfwWindow, false)) {
		CE_CORE_ERROR("Failed to initialize ImGui GLFW backend!");
		ImGui::DestroyContext(context);
		throw std::runtime_error("Failed to initialize ImGui GLFW backend!");
	}

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Bridge::ImGuiMetalInit(_metalDevice);

	_initialized = true;
	_st_imGuiMetalLayerCount++;
}

void ImGuiMetalLayer::_Shutdown() {
	if (not _initialized)
		return;
	_initialized = false;

	_st_imGuiMetalLayerCount--;
	if (_st_imGuiMetalLayerCount > 0)
		return;

	Bridge::ImGuiMetalShutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

}
