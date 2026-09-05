//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: Application.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Application/Application.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"
#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"
#if CE_PLATFORM_MACOS
	#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
	#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#endif
#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Surface/Common/OpenGl/I_OpenGlSurface.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"
#include "Core/Render/Renderer/I_Renderer.hpp"
#include "Core/Render/Renderer/DrawCommand.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/RunLoop/I_RunLoop.hpp"
#include "Core/Window/I_Platform.hpp"
#include "Core/Window/I_Window.hpp"
#include "Tools/Tools.hpp"
#include "Utility/Utility.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
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

		// The window, the renderer and the rest of the setup wait for the platform to say it has a usable window -
		// see _OnPlatformReady. On GLFW that is immediate; on Cocoa it is after NSApplication has finished launching,
		// which cannot happen while this constructor is still running.
		_platform->onReadyDispatcher.Bind(EventDelegate<>::FromMethod<Application, &Application::_OnPlatformReady>(this));
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
	// Brings the windowing backend up and fires onReadyDispatcher, which is what actually creates the window and the
	// renderer. Here rather than in the constructor because AppKit reaches that point only once it has finished
	// launching, and everything below needs a window that exists.
	_platform->Prepare();

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

	SetVSync(Props().VSync);
}

void Application::InitImguiLayer() {
	// Deferred, not built: an ImGui layer is attached as soon as it is created, and attaching it runs its _Init, which
	// asks the renderer for a device. The entry point calls this between constructing the application and starting it,
	// so at this point there is no renderer yet - the same wait everything else moved to.
	if (not _renderer) {
		_imguiLayerRequested = true;
		return;
	}

	_MakeImGuiLayer();
}

void Application::_MakeImGuiLayer() {
	// One layer per backend, and the choice belongs here rather than to a factory of its own: each one wants the
	// concrete window and the concrete device, so a layer that does not match the API cannot be built at all - the
	// OpenGL one throws std::bad_cast the moment it asks the window for a GlfwWindow.
	switch (Props().graphicsApi) {
		case Types::GraphicsApi::OpenGL:
			SetImGuiLayer(std::make_shared<ImGuiOpenGlLayer>());
			return;

#if CE_PLATFORM_MACOS
		case Types::GraphicsApi::Metal:
			SetImGuiLayer(std::make_shared<ImGuiMetalLayer>());
			return;
#endif

		default:
			CE_CORE_WARN("Application::InitImguiLayer: no ImGui layer for {} yet; the application runs without one.", Props().graphicsApi);
	}
}

void Application::SetRunning(const bool running) const {
	if (not running) {
		_runLoop->Stop();
		return;
	}

	_runLoop->SetTargetFrameRate(_TargetFrameRate(Props().VSync));
	_ApplyPresentPacing(Props().VSync);

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

void Application::_OnPlatformReady() {
	// A no-op wherever the window was already usable at construction, which is every backend but Cocoa.
	_window->Init();

	_InitRenderer();

	// Before the ImGui layer, because it binds the run loop's delegates and pushes the configured VSync into the
	// renderer that _InitRenderer has just built.
	Init();

	if (std::exchange(_imguiLayerRequested, false))
		_MakeImGuiLayer();
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
	// The window reports its first size while it is being built, which on Cocoa is before the renderer exists: the
	// view is laid out the moment it becomes the content view, and that is a resize like any other. Nothing is lost by
	// ignoring it - the swapchain reads the window's size on its first acquire anyway.
	if (not _renderer)
		return;

	const auto [width, height] = _window->GetFrameSize();
	_renderer->OnResize(width, height);

	// A geometry change is also how a window arrives on a different screen, and the presentation pacing is measured
	// against that screen's refresh. Nothing else would notice: the configured rate has not changed, only what the
	// display can do with it.
	_ApplyPresentPacing(_renderer->GetSwapchain().IsVSyncEnabled());
}

void Application::_OnVSyncChange(const Events::VSyncEvent& event) const {
	_runLoop->SetTargetFrameRate(_TargetFrameRate(event.GetState()));
	_ApplyPresentPacing(event.GetState());

	event.Consume();
}

void Application::_ApplyPresentPacing([[maybe_unused]] const bool vsync) const {
#if CE_PLATFORM_MACOS
	if (_renderer->GetGraphicApi() != Types::GraphicsApi::Metal)
		return;

	auto& swapchain = static_cast<MetalSwapchain&>(_renderer->GetSwapchain());

	// With VSync on the acquire already blocks until the display is ready, so holding frames back on top of that
	// would only be a second pacer. Uncapped is a choice too: asking for no limit is asking to present the moment the
	// GPU is done, tearing included.
	const auto configuredRate = Props().refreshRate;
	if (vsync or configuredRate == 0) {
		swapchain.SetMinimumPresentInterval(0.0_f32);
		return;
	}

	// The longer of the two intervals, and the display's is the half that matters: presenting more often than the
	// panel can scan is the definition of tearing, so a configured 240 on a 120 Hz screen has to become 120. Read
	// rather than remembered, and recomputed whenever the window reports a new geometry - which is what makes this
	// follow the window onto another monitor, and follow a refresh rate changed under it.
	const auto displayRate = _window->GetRefreshRate();

	const auto configuredInterval = 1.0_f32 / static_cast<f32>(configuredRate);
	const auto displayInterval = displayRate != 0 ? 1.0_f32 / static_cast<f32>(displayRate) : 0.0_f32;

	swapchain.SetMinimumPresentInterval(std::max(configuredInterval, displayInterval));
#endif
}

u32 Application::_TargetFrameRate(const bool vsync) const {
	// Zero - uncapped - is the right answer while VSync is on, and it is not a contradiction: the presentation call
	// already blocks until the display is ready, on every backend. glfwSwapBuffers waits for the swap interval,
	// CAMetalLayer::nextDrawable waits for the compositor to free a buffer. Sleeping on top of that would mean two
	// pacers, with the software one deciding - and the software one samples the refresh rate once, so it goes on
	// asking for 120 after the window has been dragged onto a 60 Hz display. Letting the display pace is what
	// CAMetalDisplayLink used to give for free, and it follows the window from screen to screen on its own.
	if (vsync)
		return 0;

	// Free-running: the only limit is the one the configuration asked for, and 0 there means genuinely uncapped.
	return Props().refreshRate;
}

void Application::SetVSync(const bool enabled) const {
	_renderer->SetVSync(enabled);

	// Announced rather than applied here: the run loop's pacing, and any layer showing the frame rate, hang off this
	// channel. It is the one the render context used to fire, and it had no one left to fire it.
	Events::VSyncEvent vsyncEvent{enabled};
	_dispatcher->DispatchRenderContextChangeVSyncEvent(vsyncEvent);
}

void Application::_CreateRenderResources() {
	auto& graphicDevice = _renderer->GetGraphicDevice();
	const auto& swapchain = _renderer->GetSwapchain();
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

	// Named, because it is needed twice: the buffer uses it to compute its stride, and the pipeline needs it to build
	// the vertex descriptor Metal and DirectX 12 compile into the pipeline. OpenGL only reads it in the first place,
	// which is why leaving it off the pipeline goes unnoticed there and draws nothing at all on Metal.
	const BufferLayout vertexLayout{
		{Types::ShaderDataType::Float3, "inputPosition"},
		{Types::ShaderDataType::Float4, "inputColor"}
	};

	_vertexBuffer = graphicDevice.CreateVertexBuffer(vertices, vertexLayout);
	_indexBuffer = graphicDevice.CreateIndexBuffer(indices);

	// The two halves of ShaderModuleDescriptor, picked here because only the caller knows which backend it is talking
	// to: OpenGL compiles GLSL loaded from the bundle and always enters at main, Metal looks a function up by name in
	// the .metallib CMake already compiled and never reads the source.
	const auto isOpenGl = graphicDevice.GetGraphicApi() == Types::GraphicsApi::OpenGL;

	// The descriptor borrows its strings for the duration of the call only, so these have to outlive it - which is why
	// the contents are pulled out into named locals rather than passed inline.
	const auto vertexSource = isOpenGl
		? Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Vertex.glsl").GetContentString()
		: std::string{};
	const auto fragmentSource = isOpenGl
		? Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Fragment.glsl").GetContentString()
		: std::string{};

	PipelineDescriptor pipelineDescriptor{};
	pipelineDescriptor.vertexLayout = vertexLayout;
	pipelineDescriptor.vertexShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Vertex,
		.source = vertexSource,
		.entryPoint = isOpenGl ? "main" : "vertexMain",
		.debugName = "Vertex"
	});
	pipelineDescriptor.fragmentShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Fragment,
		.source = fragmentSource,
		.entryPoint = isOpenGl ? "main" : "fragmentMain",
		.debugName = "Fragment"
	});

	// Asked rather than assumed: the descriptor defaults to BGRA8Unorm, GLFW only ever gives RGBA8Unorm, and every
	// backend but OpenGL rejects a pipeline whose format disagrees with the swapchain it draws into.
	pipelineDescriptor.formats.colors[0] = swapchain.GetColorFormat();
	pipelineDescriptor.formats.colorCount = 1;

	_pipeline = graphicDevice.CreatePipelineState(pipelineDescriptor);
}

}
