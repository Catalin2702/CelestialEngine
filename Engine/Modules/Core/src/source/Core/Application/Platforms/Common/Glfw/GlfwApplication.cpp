//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: GlfwApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Input/Platforms/Common/Glfw/GlfwInput.hpp"
#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"
#include "Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"
#include "Window/Platforms/Common/Glfw/GlfwWindow.hpp"

#include <cassert>

namespace CE::Core::Application {

GlfwApplication::GlfwApplication(): _context(nullptr), _window(nullptr), _imguiLayer(nullptr) {
	assert(_stInstance == nullptr && "GlfwApplication::GlfwApplication: GlfwApplication already exists!");
	_stInstance = this;

	SetRunning(false);
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
	Input::ShutdownInput();
	SetRunning(false);
}

void GlfwApplication::Tick(const float deltaTime) {
	assert(_window && "GlfwApplication::Tick: Window must be initialized before ticking application");
	assert(_context && "GlfwApplication::Tick: Renderer must be initialized before ticking application");
	assert(_shaderProgram && "GlfwApplication::Tick: Vertex shader must be initialized before ticking application");

	Render::Context::OpenGlContext::ClearBuffers(Types::Render::BufferBit::Color);

	_shaderProgram->Bind();

	for (const auto layer: _layerStack)
		layer->OnUpdate();

	if (_imguiLayer) {
		_imguiLayer->Begin(deltaTime);

		for (const auto layer: _layerStack)
			if (const auto renderLayer = dynamic_cast<Layers::I_RenderLayer*>(layer))
				renderLayer->OnRender();

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

void GlfwApplication::Init(const Types::Window::WindowProps& windowProps) {
	_InitWindow(windowProps);
	_InitRenderer(windowProps.graphicsApi);

	_window->GetReady();
}

void GlfwApplication::_InitWindow(const Types::Window::WindowProps& windowProps) {
	assert(not _window && "GlfwApplication::InitWindow: Window is already initialized!");

	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("GlfwApplication::InitWindow: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_window = std::make_unique<Window::GlfwWindow>(windowProps);
	_window->SetEventCallback(BIND_FN_ONE_PARAM(GlfwApplication::OnEvent));

	Input::InitInput(windowProps.windowApi);
}

void GlfwApplication::_InitRenderer(Types::Render::GraphicsApi) {
	assert(_window && "GlfwApplication::InitRenderer: Window must be initialized before initializing renderer");
	assert(not _context && "GlfwApplication::InitRenderer: Renderer is already initialized!");

	_context = std::make_unique<Render::Context::OpenGlContext>(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
	_context->Init();

	const auto vertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;

		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = vec4(a_Position, 1.0);
		}
	)";

	const auto fragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		void main()
		{
			color = vec4(v_Position * 0.5 + 0.5, 1.0);
		}
	)";

	std::array<std::unique_ptr<Render::Shader::I_Shader>, 2> shaders{
		std::make_unique<Render::Shader::OpenGlShader>(vertexSrc, Types::Render::ShaderType::Vertex),
		std::make_unique<Render::Shader::OpenGlShader>(fragmentSrc, Types::Render::ShaderType::Fragment)
	};

	_shaderProgram = std::make_unique<Render::Shader::OpenGlShaderProgram>(
		std::initializer_list{
			shaders[0].release(),
			shaders[1].release()
		}
	);
	_shaderProgram->Link();
}

void GlfwApplication::InitImGuiLayer(Types::Render::GraphicsApi) {
	assert(_window && "GlfwApplication::InitImGuiLayer: Window must be initialized before initializing ImGui layer");
	assert(_context && "GlfwApplication::InitImGuiLayer: Renderer must be initialized before initializing ImGui layer");
	assert(not _imguiLayer && "GlfwApplication::InitImGuiLayer: ImGui layer is already initialized!");

	auto overlay = std::make_unique<Layers::ImGuiOpenGlLayer>();
	_imguiLayer = overlay.release();
	PushOverlay(_imguiLayer);
}

void GlfwApplication::SetImGuiLayer(Layers::I_Layer* imguiLayer) {
	if (not imguiLayer) {
		CE_CORE_WARN("GlfwApplication::SetImGuiLayer: Provided ImGui layer is null. Ignoring.");
		return;
	}

	if (const auto openGlLayer = dynamic_cast<Layers::ImGuiOpenGlLayer*>(imguiLayer)) {
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

Window::I_Window& GlfwApplication::GetWindow() const {
	return *_window;
}

Render::Context::I_Context& GlfwApplication::GetRenderContext() const {
	return *_context;
}

}
