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
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/FileSystem/File.hpp"
#include "Utility/FileSystem/FileSystem.hpp"

#include <glad/glad.h>

#include <cassert>

namespace CE::Core {

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
	ShutdownInput();
	SetRunning(false);
}

void GlfwApplication::Tick(const float deltaTime) {
	assert(_window && "GlfwApplication::Tick: Window must be initialized before ticking application");
	assert(_context && "GlfwApplication::Tick: Renderer must be initialized before ticking application");
	assert(_shaderProgram && "GlfwApplication::Tick: Vertex shader must be initialized before ticking application");

	OpenGlContext::ClearBuffers(Types::BufferBit::Color);

	_shaderProgram->Bind();
	glBindVertexArray(_vertexArrayId);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	for (const auto layer: _layerStack)
		layer->OnUpdate();

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			layer->OnRender();

		_imguiLayer->End();
	}

	_window->OnUpdate();
	_context->SwapBuffers();
}

void GlfwApplication::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher eventDispatcher{event};

	switch (event.GetEventType()) {
		case Events::EventType::WindowClose: {
			eventDispatcher.Dispatch<Events::WindowCloseEvent>([this](const Events::WindowCloseEvent& e) {
				Quit();
				e.Consume();
				return true;
			});
			break;
		}
		default:
			break;
	}

	if (event.IsHandled())
		return;

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(event);
		if (event.IsHandled())
			break;
	}
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

	eventHubDispatcher.glfwMouseEventHub.onMovedMulticastDispatcher.Subscribe(EventDelegate<Events::MouseMovedEvent&>::FromFunction<&LogMouseMoved>());

	InitInput(windowProps.windowApi);
}

void GlfwApplication::_InitRenderer() {
	assert(_window && "GlfwApplication::InitRenderer: Window must be initialized before initializing renderer");
	assert(not _context && "GlfwApplication::InitRenderer: Renderer is already initialized!");

	_context = std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
	_context->Init();

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
			PopOverlay(_imguiLayer);
			_imguiLayer = nullptr;
		}

		_imguiLayer = openGlLayer;
		PushOverlay(_imguiLayer);
	}
	else {
		CE_CORE_ERROR("GlfwApplication::SetImGuiLayer: Provided ImGui layer is not compatible with OpenGlContext. Expected ImGuiOpenGlLayer or derived class.");
		throw std::runtime_error("Provided ImGui layer is not compatible with OpenGlContext. Expected ImGuiOpenGlLayer or derived class.");
	}
}

void GlfwApplication::RemoveImGuiLayer() {
	if (not _imguiLayer)
		return;

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

	windowStateEvents.onResizeDispatcher.Bind(EventDelegate<int, int>::FromMethod<hub, &hub::ReceiveWindowResizeEvent>(&eventHubDispatcher));
	windowStateEvents.onCloseDispatcher.Bind(EventDelegate<>::FromMethod<hub, &hub::ReceiveWindowCloseEvent>(&eventHubDispatcher));
	windowStateEvents.onErrorDispatcher.Bind(EventDelegate<int, const char*>::FromMethod<hub, &hub::ReceiveAppErrorEvent>(&eventHubDispatcher));

	keyboardEvents.onKeyDispatcher.Bind(EventDelegate<int, int, int, int>::FromMethod<hub, &hub::ReceiveKeyEvent>(&eventHubDispatcher));
	keyboardEvents.onCharDispatcher.Bind(EventDelegate<unsigned int>::FromMethod<hub, &hub::ReceiveCharEvent>(&eventHubDispatcher));

	mouseEvents.onMouseButtonDispatcher.Bind(EventDelegate<int, int, int>::FromMethod<hub, &hub::ReceiveMouseButtonEvent>(&eventHubDispatcher));
	mouseEvents.onMousePositionDispatcher.Bind(EventDelegate<double, double>::FromMethod<hub, &hub::ReceiveMousePositionEvent>(&eventHubDispatcher));
	mouseEvents.onMouseDraggedDispatcher.Bind(EventDelegate<int, int, int, double, double>::FromMethod<hub, &hub::ReceiveMouseDraggedEvent>(&eventHubDispatcher));
	mouseEvents.onMouseWheelScrollDispatcher.Bind(EventDelegate<double, double>::FromMethod<hub, &hub::ReceiveMouseWheelScrollEvent>(&eventHubDispatcher));
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
