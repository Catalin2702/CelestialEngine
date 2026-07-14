//
// Module: CelestialEngine/Engine/Modules/Core/Application/Platforms/Common/Glfw
// File: GlfwApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Input/Platforms/Common/Glfw/GlfwInput.hpp"
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

namespace CE::Core {

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
}

GlfwApplication::~GlfwApplication() {
	if (IsRunning())
		GlfwApplication::Quit();

	_layerStack.Clear();
	_imguiLayer = nullptr;

	_shaderProgram.reset();
	_context.reset();
	_window.reset();
}

void GlfwApplication::Run() {
	SetRunning(true);

	ResetDeltaTime();
	while (IsRunning()) {
		Tick(GetDeltaTime());
	}
}

void GlfwApplication::Quit() {
	UnsubscribeFromDispatcher();
	ShutdownInput();
	SetRunning(false);
}

void GlfwApplication::Tick(const float deltaTime) {
	assert(_window && "GlfwApplication::Tick: Window must be initialized before ticking application");
	assert(_context && "GlfwApplication::Tick: Renderer must be initialized before ticking application");
	assert(_shaderProgram && "GlfwApplication::Tick: Vertex shader must be initialized before ticking application");

	applicationEventHandler.DispatchTickEvent();

	OpenGlContext::ClearBuffers(Types::BufferBit::Color);

	_shaderProgram->Bind();
	glBindVertexArray(_vertexArrayId);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	for (const auto layer: _layerStack)
		layer->OnUpdate();
	applicationEventHandler.DispatchUpdateEvent();

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();
		applicationEventHandler.DispatchRenderEvent();

		_imguiLayer->End();
	}

	_window->OnUpdate();
	_context->SwapBuffers();
}

void GlfwApplication::Init() {
	_InitWindow();
	_InitRenderer();

	_window->GetReady(Utility::Config::Config::StGetWindowProps().VSync);
}

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
	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	if (not Types::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("GlfwApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_window = std::make_unique<GlfwWindow>();
	SetEventHubDispatcher();
	SubscribeToHubDispatcher();

	InitInput(windowProps.windowApi);
}

void GlfwApplication::_InitRenderer() {
	assert(_window && "GlfwApplication::InitRenderer: Window must be initialized before initializing renderer");
	assert(not _context && "GlfwApplication::InitRenderer: Renderer is already initialized!");

	_context = std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
	_context->Init();

	// The context is created after the window (and after SetEventHubDispatcher), so its resize dispatcher is bound to the hub
	// here rather than in SetEventHubDispatcher.
	_context->openGlContextEventDispatcher.onResizeDispatcher.Bind(
		EventDelegate<int, int>::FromMethod<GlfwEventHubDispatcher, &GlfwEventHubDispatcher::ReceiveWindowResizeEvent>(&eventHubDispatcher)
	);

	glGenVertexArrays(1, &_vertexArrayId);
	glBindVertexArray(_vertexArrayId);

	glGenBuffers(1, &_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);

	constexpr float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

	constexpr unsigned int indices[3] = { 0, 1, 2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	std::array<std::unique_ptr<I_Shader>, 2> shaders{
		std::make_unique<OpenGlShader>(Utility::FileSystem::StLoad("../Resources/Shaders/OpenGL/Vertex.glsl"), Types::ShaderType::Vertex),
		std::make_unique<OpenGlShader>(Utility::FileSystem::StLoad("../Resources/Shaders/OpenGL/Fragment.glsl"), Types::ShaderType::Fragment)
	};

	_shaderProgram = std::make_unique<OpenGlShaderProgram>(
		std::initializer_list{
			shaders[0].release(),
			shaders[1].release()
		}
	);
	_shaderProgram->Link();
}

void GlfwApplication::SetImGuiLayer(I_Layer* imguiLayer) {
	if (not imguiLayer) {
		CE_CORE_WARN("GlfwApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto openGlLayer = dynamic_cast<ImGuiOpenGlLayer*>(imguiLayer)) {
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
	if (not _imguiLayer)
		return;

	_imguiLayer->UnsubscribeFromEventHub();
	PopOverlay(_imguiLayer);
	_imguiLayer = nullptr;
}

void GlfwApplication::SetEventHubDispatcher() {
	if (not _window) {
		const auto error = "GlfwApplication::SetEventHubDispatcher: Window must be initialized before setting up the event hub dispatcher";
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

	// The application fires its own lifecycle events (tick/update/render each frame, plus errors) into the hub.
	applicationEventHandler.applicationEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveAppErrorEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onTickDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppTickEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onUpdateDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppUpdateEvent>(&eventHubDispatcher));
	applicationEventHandler.applicationEvents.onRenderDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveAppRenderEvent>(&eventHubDispatcher));
}

void GlfwApplication::SubscribeToHubDispatcher() {
	_eventHubHandlers[AppError] = eventHubDispatcher.glfwApplicationEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);

	_eventHubHandlers[WindowClose] = eventHubDispatcher.glfwWindowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<GlfwApplication, &GlfwApplication::_OnWindowClose>(this)
	);
	_eventHubHandlers[WindowError] = eventHubDispatcher.glfwWindowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromFunction<&LogError>()
	);
}

void GlfwApplication::UnsubscribeFromDispatcher() {
	eventHubDispatcher.glfwApplicationEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[AppError]);

	eventHubDispatcher.glfwWindowEventHub.onCloseMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowClose]);
	eventHubDispatcher.glfwWindowEventHub.onErrorMulticastDispatcher.Unsubscribe(_eventHubHandlers[WindowError]);
}

void GlfwApplication::_OnWindowClose(Events::WindowCloseEvent&) {
	Quit();
}

I_Window& GlfwApplication::GetWindow() const {
	return *_window;
}

GlfwWindow& GlfwApplication::GetGlfwWindow() const {
	return *_window;
}

I_Context& GlfwApplication::GetRenderContext() const {
	return *_context;
}

OpenGlContext& GlfwApplication::GetOpenGlContext() const {
	return *_context;
}

}
