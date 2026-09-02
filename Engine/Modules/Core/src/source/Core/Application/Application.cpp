//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: Application.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Application/Application.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"
#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"
#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Surface/I_OpenGlSurface.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"
#include "Core/Render/Renderer/I_Renderer.hpp"
#include "Core/Render/Renderer/DrawCommand.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/RunLoop/I_RunLoop.hpp"
#include "Core/Window/I_Platform.hpp"
#include "Core/Window/I_Window.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Utility.hpp"

#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <utility>
#include <string>


namespace CE::Core {

namespace {

const Types::WindowProps& Props() { return Utility::Config::GetWindowProps(); }

// The bundle puts the resources one level up from the executable on macOS, beside it everywhere else.
constexpr auto OpenGlShadersDirectory = CE_PLATFORM_MACOS ? "../Resources/Shaders/OpenGL/" : "Resources/Shaders/OpenGL/";

}

std::atomic<Application*> Application::_instance = nullptr;

Application::Application():
	_dispatcher(I_EventHubDispatcher::MakeEventHubDispatcher(Props().windowApi)),
	_platform(I_Platform::MakePlatform(Props().windowApi)),
	_window(I_Window::MakeWindow(*_platform)),
	// _graphicDevice(I_GraphicDevice::MakeDevice(Props().graphicsApi)),
	// _swapchain(I_Swapchain::MakeSwapchain(*_window, Props().graphicsApi)),
	_runLoop(I_RunLoop::MakeRunLoop(*_platform))
{
	assert(_instance == nullptr && "Application::Application: An application already exists!");

	// Published before the init steps because they reach back through Get(); a throw from any of them has to clear it
	// by hand, since the destructor - which is what normally resets it - never runs for an object whose constructor
	// did not complete.
	_instance = this;

	try {
		_InitWindow();
		_InitRenderer();
		Application::Init();
	}
	catch (...) {
		_instance = nullptr;
		throw;
	}
}

Application::~Application() {
	// A safety net, not the normal path: Start() ends the application when its loop returns. This covers an
	// application that was built and never started, or one whose Start() unwound.
	End();

	_instance = nullptr;
}

void Application::PushLayer(const std::shared_ptr<I_Layer>& layer) {
	_layerStack.PushLayer(layer);
}

void Application::PushOverlay(const std::shared_ptr<I_Layer>& overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::PopLayer(const std::shared_ptr<I_Layer>& layer) {
	_layerStack.PopLayer(layer);
}

void Application::PopOverlay(const std::shared_ptr<I_Layer>& overlay) {
	_layerStack.PopOverlay(overlay);
}

void Application::ReplaceLayer(const std::shared_ptr<I_Layer>& oldLayer, const std::shared_ptr<I_Layer>& newLayer) {
	_layerStack.ReplaceLayer(oldLayer, newLayer);
}

Application& Application::Get() {
	assert(_instance != nullptr && "Application::Get: no application exists - did you forget to create one?");
	return *_instance;
}

const Application& Application::GetConst() {
	return Get();
}

void Application::Start() {
	SetRunning(true);

	// Reset the clock last, so the first frame's delta measures a frame and not the whole of initialisation.
	_ResetDeltaTime();

	// Blocking: the loop owns the thread from here and calls _OnFrame until Stop(). Nothing below runs until it ends.
	_runLoop->Start();

	End();
}

void Application::End() {
	// Reachable twice - once when Start() returns, once from the destructor - so it has to be idempotent. The guard is
	// a flag of its own and not _runLoop->IsRunning(): by the time End() runs the loop has already stopped, whether it
	// was Stop() from a close event or Start() simply returning, so keying on the loop would skip the real teardown
	// and leave every subscription and the ImGui layer alive.
	if (std::exchange(_ended, true))
		return;

	// Pause before Stop so a display-driven loop tears its pacing source down before the loop itself goes away.
	_runLoop->Pause();
	_runLoop->Stop();

	_UnsubscribeFromEventHubDispatcher();
	Input::Shutdown();

	// Detaches every remaining layer while the window - and on OpenGL its context - is still alive.
	_layerStack.Clear();
}

void Application::Tick(const f32 deltaTime) {
	Events::AppTickEvent tickEvent{deltaTime};
	_dispatcher->DispatchAppTickEvent(tickEvent);

	for (const auto& layer: _layerStack)
		layer->OnUpdate();

	Events::AppUpdateEvent updateEvent{};
	_dispatcher->DispatchAppUpdateEvent(updateEvent);

	// A false here is a minimised or zero-sized window: skip the frame, never the event pump below, or the window
	// could never be restored.
	if (_renderer->BeginFrame()) {
		_RenderFrame(deltaTime);
		_renderer->EndFrame();
	}

	// Clear this frame's input transitions BEFORE pumping: the events drained below belong to the next frame, so they
	// have to land on a clean transition state.
	Input::EndFrame();
	_platform->PollEvents();
}

void Application::Run() const {
	_runLoop->Run();
}

void Application::Pause() const {
	_runLoop->Pause();
}

void Application::Init() {
	_runLoop->SetFrameDelegate(EventDelegate<>::FromMethod<Application, &Application::_OnFrame>(this));
	_runLoop->SetDidStartDelegate(EventDelegate<>::FromConstMethod<Application, &Application::_OnLoopStarted>(this));

	_renderer->SetVSync(Props().VSync);
}

void Application::InitImguiLayer() {
	const auto imguiLayer = std::make_shared<ImGuiOpenGlLayer>();
	SetImGuiLayer(imguiLayer);
}

void Application::SetRunning(const bool running) const {
	if (not running) {
		_runLoop->Stop();
		return;
	}

	// With VSync on the display sets the cadence, so the limiter is told the refresh rate rather than the configured
	// one: asking for more frames than the display can show only burns the CPU.
	const auto& windowProps = Props();
	_runLoop->SetTargetFrameRate(windowProps.VSync ? _window->GetRefreshRate() : windowProps.refreshRate);

	_runLoop->Run();
}

void Application::SetImGuiLayer(const std::shared_ptr<I_ImGuiLayer>& imguiLayer) {
	if (not imguiLayer) [[unlikely]] {
		CE_CORE_WARN("Application::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto oldLayer = _imguiLayer.lock())
		ReplaceLayer(oldLayer, imguiLayer);
	else
		PushOverlay(imguiLayer);

	imguiLayer->SubscribeToEventHub();
	_imguiLayer = imguiLayer;
}

void Application::RemoveImGuiLayer() {
	const auto imguiLayer = _imguiLayer.lock();
	if (not imguiLayer)
		return;

	// Unsubscribe before the layer leaves the stack: its delegates point at it, and PopOverlay may drop the last
	// owning reference.
	imguiLayer->UnsubscribeFromEventHub();
	PopOverlay(imguiLayer);

	_imguiLayer.reset();
}

void Application::_SubscribeToEventHubDispatcher() {
	auto& windowEventHub = _dispatcher->GetWindowEventHub();
	auto& [onChangeVSyncDispatcher] = _dispatcher->GetRenderContextEventHub();

	_eventHubHandlers[_WindowClose] = windowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromConstMethod<Application, &Application::_OnWindowClose>(this)
	);
	_eventHubHandlers[_WindowResize] = windowEventHub.onResizeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowResizeEvent&>::FromConstMethod<Application, &Application::_OnWindowResize>(this)
	);
	_eventHubHandlers[_VSyncChange] = onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromConstMethod<Application, &Application::_OnVSyncChange>(this)
	);
}

void Application::_UnsubscribeFromEventHubDispatcher() {
	_dispatcher->GetWindowEventHub().onCloseMulticastDispatcher.Unsubscribe(_eventHubHandlers[_WindowClose]);
	_dispatcher->GetWindowEventHub().onResizeMulticastDispatcher.Unsubscribe(_eventHubHandlers[_WindowResize]);
	_dispatcher->GetRenderContextEventHub().onChangeVSyncDispatcher.Unsubscribe(_eventHubHandlers[_VSyncChange]);

	_eventHubHandlers = {};
}

void Application::_SetEventHubDispatcher() const {
	// Each backend wires its own raw callbacks: the translation shapes differ, and only the backend knows its own.
	_platform->ConnectToEventHub(*_dispatcher);
	_window->ConnectToEventHub(*_dispatcher);
}

void Application::_ResetDeltaTime() const {
	_lastFrameTime.store(Clock::now());
}

void Application::_InitWindow() {
	// Input must exist before the subscriptions below: the state it owns has to be the hub's first subscriber, so it
	// has already updated when every other handler runs.
	Input::Init();

	_SetEventHubDispatcher();
	_SubscribeToEventHubDispatcher();
}

void Application::_InitRenderer() {
	// On OpenGL every graphics call applies to whichever context is current on this thread, and nothing else makes it
	// current. The other backends have no such notion, which is why this is asked of the surface and not of the window.
	if (auto* const surface = dynamic_cast<I_OpenGlSurface*>(_window.get()))
		surface->MakeContextCurrent();

	_renderer = I_Renderer::MakeRenderer(*_window, Props().graphicsApi);
	_renderer->SetClearColor(glm::vec4{
		0.1_f32, 0.1_f32, 0.1_f32, 1.0_f32
	});

	_CreateRenderResources();
}

void Application::_OnFrame() {
	const auto now = std::chrono::steady_clock::now();
	const auto deltaTime = std::chrono::duration<f32>(now - _lastFrameTime.load()).count();
	_lastFrameTime.store(now);

	Tick(deltaTime);
}

void Application::_OnLoopStarted() const {
	// Revealed from inside the loop rather than from the constructor: AppKit will not give a usable window before its
	// run loop is up, and showing one early is the difference between a window and a beach ball.
	_window->Show();
}

void Application::_RenderFrame(const f32 deltaTime) {
	_renderer->BeginPass();

	if (_pipeline and _vertexBuffer and _indexBuffer) [[likely]] {
		DrawCommand command{};
		command.pipelineState = _pipeline.get();
		command.vertexBuffer = _vertexBuffer.get();
		command.indexBuffer = _indexBuffer.get();

		_renderer->Submit(command);
	}

	_renderer->EndPass();

	// The overlay draws on top of the pass above, which is why it is not inside it.
	if (const auto imguiLayer = _imguiLayer.lock()) [[likely]] {
		imguiLayer->Begin(deltaTime);

		for (const auto& layer: _layerStack)
			layer->OnRender();

		Events::AppRenderEvent renderEvent{};
		_dispatcher->DispatchAppRenderEvent(renderEvent);

		imguiLayer->End();
	}
}

void Application::_OnWindowClose(const Events::WindowCloseEvent& event) const {
	// Stop, not End: this runs inside a frame, and the loop has to finish it before unwinding. Start() calls End()
	// once Start() returns.
	_runLoop->Stop();

	event.Consume();
}

void Application::_OnWindowResize(const Events::WindowResizeEvent&) const {
	const auto [width, height] = _window->GetFrameSize();
	_renderer->OnResize(width, height);
}

void Application::_OnVSyncChange(const Events::VSyncEvent& event) const {
	// The pacing target follows the swap behaviour: the display's rate while it waits for the display, the configured
	// one while it runs free.
	const auto& windowProps = Props();
	_runLoop->SetTargetFrameRate(event.GetState() ? _window->GetRefreshRate() : windowProps.refreshRate);

	event.Consume();
}

void Application::_CreateRenderResources() {
	auto& graphicDevice = _renderer->GetGraphicDevice();
	auto& swapchain = _renderer->GetSwapchain();
	constexpr std::array vertices {
		// Front face
		-0.55f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 0
		 0.55f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		 0.55f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
		-0.55f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3

		// Lateral face
		 0.55f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 4
		 0.70f, -0.60f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 5
		 0.70f,  0.90f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6
		 0.55f,  0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7

		// Upper face
		 0.55f,  0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 8
		 0.70f,  0.90f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 9
		-0.40f,  0.90f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 10
		-0.55f,  0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 11
	};

	constexpr std::array indices {
		0_u32, 1_u32, 2_u32, 2_u32, 3_u32, 0_u32,		// Front face
		4_u32, 5_u32, 6_u32, 6_u32, 7_u32, 4_u32,		// Lateral face
		8_u32, 9_u32, 10_u32, 10_u32, 11_u32, 8_u32		// Upper face
	};

	_vertexBuffer = graphicDevice.CreateVertexBuffer(vertices, BufferLayout{
		{ShaderDataType::Float3, "inputPosition"},
		{ShaderDataType::Float4, "inputColor"}
	});
	_indexBuffer = graphicDevice.CreateIndexBuffer(indices);

	// The descriptor borrows its strings for the duration of the call only, so these have to outlive it - which is why
	// the contents are pulled out into named locals rather than passed inline.
	const auto vertexSource = Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Vertex.glsl").GetContentString();
	const auto fragmentSource = Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Fragment.glsl").GetContentString();

	PipelineDescriptor pipelineDescriptor{};
	pipelineDescriptor.vertexShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Vertex, .source = vertexSource, .debugName = "Vertex"
	});
	pipelineDescriptor.fragmentShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Fragment, .source = fragmentSource, .debugName = "Fragment"
	});

	// Asked rather than assumed: the descriptor defaults to BGRA8Unorm, GLFW only ever gives RGBA8Unorm, and every
	// backend but OpenGL rejects a pipeline whose format disagrees with the swapchain it draws into.
	pipelineDescriptor.formats.colors[0] = swapchain.GetColorFormat();
	pipelineDescriptor.formats.colorCount = 1;

	_pipeline = graphicDevice.CreatePipelineState(pipelineDescriptor);
}

}
