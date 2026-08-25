//
// Module: CelestialEngine/Engine/Modules/Core/Application/Platforms/Common/Glfw
// File: GlfwApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Input/Input.hpp"
#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"
#include "Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/FileSystem/File.hpp"
#include "Utility/FileSystem/FileSystem.hpp"

#include <glad/glad.h>

#include <cassert>
#include <chrono>
#include <format>
#include <string>
#include <thread>

// On macOS GLFW hosts a real NSApplication, so the GLFW backend reuses the shared native menu bar built by CreateMenuBar.
#if CE_PLATFORM_MACOS
#include "Core/Application/Platforms/Mac/Cocoa/MacMenuBar.hpp"

#include <AppKit/AppKit.hpp>
#endif


namespace CE::Core {

// On macOS, CE_App ships as a bundle (Contents/MacOS/CE_App next to Contents/Resources/), so
// Resources/ is one level above the executable. On Windows/Linux, Resources/ is copied as a
// direct sibling of the executable instead (see App/CMakeLists.txt), so no "../" is needed.
constexpr auto OpenGlShadersDirectory = CE_PLATFORM_MACOS ? "../Resources/Shaders/OpenGL/": "Resources/Shaders/OpenGL/";

static unsigned int _st_TargetFPS = 0;

// Only macOS has a native menu bar, and NS::Application / CreateMenuBar only exist there. An `if constexpr` at the call site
// would not be enough: the discarded branch is still name-looked-up and type-checked, so the preprocessor guard has to sit
// inside this shim (a no-op elsewhere) instead of around the call.
static void InstallNativeMenuBar() {
#if CE_PLATFORM_MACOS
	// GLFW already created the shared NSApplication during glfwInit; give it the same menu bar the Cocoa backend uses.
	NS::Application::sharedApplication()->setMainMenu(CreateMenuBar<GlfwApplication>());
#endif
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void LogError(Events::ErrorEvent& appErrorEvent) {
	CE_CORE_ERROR(appErrorEvent);
}

void GlfwApplicationEventHandler::DispatchErrorEvent(const int errorCode, const char* description) const {
	applicationEvents.onErrorDispatcher.Dispatch(errorCode, description);
}

void GlfwApplicationEventHandler::DispatchTickEvent() const {
	applicationEvents.onTickDispatcher.Dispatch();
}

void GlfwApplicationEventHandler::DispatchUpdateEvent() const {
	applicationEvents.onUpdateDispatcher.Dispatch();
}

void GlfwApplicationEventHandler::DispatchRenderEvent() const {
	applicationEvents.onRenderDispatcher.Dispatch();
}

GlfwApplication::GlfwApplication(): _context(nullptr), _window(nullptr), _imguiLayer(nullptr) {
	assert(_stInstance == nullptr && "GlfwApplication::GlfwApplication: GlfwApplication already exists!");
	_stInstance = this;

	I_Application::SetRunning(false);

	GlfwApplication::_InitWindow();
	GlfwApplication::_InitRenderer();

	_context->SetVSync(Utility::Config::StGetWindowProps().VSync);

	InstallNativeMenuBar();
}

GlfwApplication::~GlfwApplication() {
	if (IsRunning()) [[likely]]
		GlfwApplication::Quit();

	_layerStack.Clear();
	_imguiLayer = nullptr;

	_context.reset();
	_window.reset();
}

void GlfwApplication::Start() {
	SetRunning(true);

	ResetDeltaTime();

	const auto& windowProps = Utility::Config::StGetWindowProps();
	_st_TargetFPS = windowProps.VSync ? _window->GetRefreshRate() : windowProps.refreshRate;

	auto nextFrame = std::chrono::steady_clock::now();
	while (IsRunning()) {
		Tick(GetDeltaTime());

		if (_st_TargetFPS == 0) [[unlikely]]
			continue;

		nextFrame += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / _st_TargetFPS));
		if (const auto now = std::chrono::steady_clock::now(); nextFrame > now)
			std::this_thread::sleep_until(nextFrame);
		else
			// Fell behind (long frame or a mode switch): resync so we do not burst-render to catch up.
			nextFrame = now;
	}
}

void GlfwApplication::Quit() {
	UnsubscribeFromDispatcher();
	Input::Shutdown();
	SetRunning(false);
}

void GlfwApplication::Tick(const float deltaTime) {
	assert(_window && "GlfwApplication::Tick: Window must be initialized before ticking application");
	assert(_context && "GlfwApplication::Tick: Renderer must be initialized before ticking application");

	applicationEventHandler.DispatchTickEvent();

	OpenGlContext::ClearBuffers(Types::BufferBit::Color);

	_shaderProgram.Bind();
	glBindVertexArray(_vertexArrayId);
	glDrawElements(GL_TRIANGLES, static_cast<int>(_indexBuffer.GetCount()), GL_UNSIGNED_INT, nullptr);

	for (const auto layer: _layerStack)
		layer->OnUpdate();
	applicationEventHandler.DispatchUpdateEvent();

	if (_imguiLayer) [[likely]] {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();
		applicationEventHandler.DispatchRenderEvent();

		_imguiLayer->End();
	}

	// Clear per-frame input transitions BEFORE polling: the glfwPollEvents call below delivers the events that belong
	// to the next frame, so they must land on a clean transition state.
	Input::EndFrame();

	_window->OnUpdate();
	_context->SwapBuffers();
}

void GlfwApplication::Init() {

}

GlfwApplication& GlfwApplication::StGet() {
	return dynamic_cast<GlfwApplication&>(I_Application::StGet());
}

#if CE_PLATFORM_MACOS

void GlfwApplication::StOnQuitMenuCallback(void*, SEL, const NS::Object*) {
	StGet().Quit();
}

void GlfwApplication::StOnToggleVSyncCallback(void*, SEL, const NS::Object*) {
	Types::WindowProps windowProps = Utility::Config::StGetWindowProps();
	windowProps.VSync = !windowProps.VSync;
	Utility::Config::StSetWindowProps(windowProps);

	// The menu action fires on the main thread during glfwPollEvents, where the GL context is current, so swapping the swap
	// interval here is safe.
	StGet().GetOpenGlContext().SetVSync(windowProps.VSync);
}

void GlfwApplication::StOnMiniaturizeCallback(void*, SEL, const NS::Object*) {
	StGet().GetGlfwWindow().Miniaturize();
}

void GlfwApplication::StOnDeminiaturizeCallback(void*, SEL, const NS::Object*) {
	StGet().GetGlfwWindow().Deminiaturize();
}

void GlfwApplication::StOnToggleFullscreenCallback(void*, SEL, const NS::Object*) {
	StGet().GetGlfwWindow().ToggleFullScreen();
}

#endif

void GlfwApplication::InitImGuiLayer() {
	assert(_window && "GlfwApplication::InitImGuiLayer: Window must be initialized before initializing ImGui layer");
	assert(_context && "GlfwApplication::InitImGuiLayer: Renderer must be initialized before initializing ImGui layer");
	assert(not _imguiLayer && "GlfwApplication::InitImGuiLayer: ImGui layer is already initialized!");

	auto overlay = std::make_unique<ImGuiOpenGlLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
	_imguiLayer->SubscribeToEventHub();
}

void GlfwApplication::_InitWindow() {
	assert(not _window && "GlfwApplication::InitWindow: Window is already initialized!");
	const auto& windowProps = Utility::Config::StGetWindowProps();

	if (not Types::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) [[unlikely]] {
		const auto error = std::format("GlfwApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {}, Window API: {}", windowProps.graphicsApi, windowProps.windowApi);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_window = std::make_unique<GlfwWindow>();

	// Input must exist before SubscribeToHubDispatcher: the input state it owns is the hub's first subscriber.
	Input::Init();

	SetEventHubDispatcher();
	SubscribeToHubDispatcher();
}

void GlfwApplication::_InitRenderer() {
	assert(_window && "GlfwApplication::InitRenderer: Window must be initialized before initializing renderer");
	assert(not _context && "GlfwApplication::InitRenderer: Renderer is already initialized!");

	_context = std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
	_context->Init();

	// The context is created after the window (and after SetEventHubDispatcher), so its resize dispatcher is bound to the hub
	// here rather than in SetEventHubDispatcher.
	_context->openGlContextEventDispatcher.openGlContextLifeCycle.onResizeDispatcher.Bind(
		EventDelegate<int, int>::FromMethod<GlfwEventHubDispatcher, &GlfwEventHubDispatcher::ReceiveWindowResizeEvent>(&eventHubDispatcher)
	);
	_context->openGlContextEventDispatcher.openGlContextLifeCycle.onVSyncChangedDispatcher.Bind(
		EventDelegate<bool>::FromMethod<GlfwEventHubDispatcher, &GlfwEventHubDispatcher::ReceiveContextChangeVSyncEvent>(&eventHubDispatcher)
	);

	glGenVertexArrays(1, &_vertexArrayId);
	glBindVertexArray(_vertexArrayId);

	constexpr uint32_t vertexCount = 3 * 7;
	constexpr float vertices[vertexCount] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};

	constexpr uint32_t indexCount = 3;
	constexpr uint32_t indices[indexCount] = { 0, 1, 2 };

	_vertexBuffer = OpenGlVertexBuffer(vertices, vertexCount);
	_vertexBuffer.Bind();

	_indexBuffer = OpenGlIndexBuffer(indices, indexCount);
	_indexBuffer.Bind();

	const BufferLayout layout = {
		{ShaderDataType::Float3, "inputPosition"},
		{ShaderDataType::Float4, "inputColor"}
	};
	_vertexBuffer.SetLayout(layout);

	_shaderProgram = OpenGlShaderProgram({{
		OpenGlShader(Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Vertex.glsl"), Types::ShaderType::Vertex),
		OpenGlShader(Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "Fragment.glsl"), Types::ShaderType::Fragment)
	}});
	_shaderProgram.Link();
}

void GlfwApplication::SetImGuiLayer(I_Layer* imguiLayer) {
	if (not imguiLayer) [[unlikely]] {
		CE_CORE_WARN("GlfwApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto openGlLayer = dynamic_cast<ImGuiOpenGlLayer*>(imguiLayer)) [[likely]] {
		if (_imguiLayer) {
			_imguiLayer->UnsubscribeFromEventHub();
			PopOverlay(_imguiLayer);
			_imguiLayer = nullptr;
		}

		_imguiLayer = openGlLayer;
		PushOverlay(_imguiLayer);
		_imguiLayer->SubscribeToEventHub();
	}
	else {
		CE_CORE_ERROR("GlfwApplication::SetImGuiLayer: Provided ImGui layer is not compatible with OpenGlContext. Expected ImGuiOpenGlLayer or derived class.");
		throw std::runtime_error("Provided ImGui layer is not compatible with OpenGlContext. Expected ImGuiOpenGlLayer or derived class.");
	}
}

void GlfwApplication::RemoveImGuiLayer() {
	if (not _imguiLayer) [[unlikely]]
		return;

	_imguiLayer->UnsubscribeFromEventHub();
	PopOverlay(_imguiLayer);
	_imguiLayer = nullptr;
}

void GlfwApplication::SetEventHubDispatcher() {
	if (not _window) [[unlikely]] {
		constexpr auto error = "GlfwApplication::SetEventHubDispatcher: Window must be initialized before setting up the event hub dispatcher";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	using hub = GlfwEventHubDispatcher;
	auto& [windowStateEvents, keyboardEvents, mouseEvents] = _window->windowEventHandler;

	// Window resize is fired by the render context (framebuffer size), not the window: see _InitRenderer. The window-size
	// callback still runs to keep the window's cached size in sync, it just no longer feeds the hub.
	windowStateEvents.onCloseDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveWindowCloseEvent>(&eventHubDispatcher));
	windowStateEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveWindowErrorEvent>(&eventHubDispatcher));

	keyboardEvents.onKeyDispatcher.Bind(EventDelegate<int, int, int, int>::FromMethod<hub, &hub::ReceiveKeyEvent>(&eventHubDispatcher));
	keyboardEvents.onCharDispatcher.Bind(EventDelegate<unsigned int>::FromMethod<hub, &hub::ReceiveCharEvent>(&eventHubDispatcher));

	mouseEvents.onMouseButtonDispatcher.Bind(EventDelegate<int, int, int>::FromMethod<hub, &hub::ReceiveMouseButtonEvent>(&eventHubDispatcher));
	mouseEvents.onMousePositionDispatcher.Bind(EventDelegate<double, double>::FromMethod<hub, &hub::ReceiveMousePositionEvent>(&eventHubDispatcher));
	mouseEvents.onMouseDraggedDispatcher.Bind(EventDelegate<int, int, int, double, double>::FromMethod<hub, &hub::ReceiveMouseDraggedEvent>(&eventHubDispatcher));
	mouseEvents.onMouseWheelScrollDispatcher.Bind(EventDelegate<double, double>::FromMethod<hub, &hub::ReceiveMouseWheelScrollEvent>(&eventHubDispatcher));

	// Focus changes feed the hub so input state can reset held keys when the window stops receiving events.
	windowStateEvents.onFocusDispatcher.Bind(EventDelegate<int>::FromMethod<hub, &hub::ReceiveWindowFocusEvent>(&eventHubDispatcher));

	// The application fires its own lifecycle events (tick/update/render each frame, plus errors) into the hub.
	applicationEventHandler.applicationEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveAppErrorEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onTickDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppTickEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onUpdateDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppUpdateEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onRenderDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppRenderEvent>(&eventHubDispatcher));
}

void GlfwApplication::SubscribeToHubDispatcher() {
	// Input state MUST subscribe first: it has to be up to date before any other subscriber (ImGui, layers) handles
	// the same event. Requires Input::Init to have run (it has: _InitWindow calls it right before this method).
	Input::SubscribeToHub(
		eventHubDispatcher.glfwKeyboardEventHub,
		eventHubDispatcher.glfwMouseEventHub,
		eventHubDispatcher.glfwWindowEventHub
	);

	_eventHubHandlers[AppError] = eventHubDispatcher.glfwApplicationEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);

	_eventHubHandlers[VSyncChange] = eventHubDispatcher.openGlRenderContextEventHub.onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromConstMethod<GlfwApplication, &GlfwApplication::_OnVSyncChange>(this)
	);

	_eventHubHandlers[WindowClose] = eventHubDispatcher.glfwWindowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<GlfwApplication, &GlfwApplication::_OnWindowClose>(this)
	);
	_eventHubHandlers[WindowError] = eventHubDispatcher.glfwWindowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);
}

void GlfwApplication::UnsubscribeFromDispatcher() {
	Input::UnsubscribeFromHub(
		eventHubDispatcher.glfwKeyboardEventHub,
		eventHubDispatcher.glfwMouseEventHub,
		eventHubDispatcher.glfwWindowEventHub
	);

	eventHubDispatcher.glfwApplicationEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[AppError]);

	eventHubDispatcher.openGlRenderContextEventHub.onChangeVSyncDispatcher.Unsubscribe(_eventHubHandlers[VSyncChange]);

	eventHubDispatcher.glfwWindowEventHub.onCloseMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowClose]);
	eventHubDispatcher.glfwWindowEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowError]);
}

void GlfwApplication::_OnWindowClose(Events::WindowCloseEvent&) {
	Quit();
}

void GlfwApplication::_OnVSyncChange(const Events::VSyncEvent& event) const {
	const auto& windowProps = Utility::Config::StGetWindowProps();
	_st_TargetFPS = event.GetState() ? _window->GetRefreshRate() : windowProps.refreshRate;
}

}
