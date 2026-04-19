//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: GlfwApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP
#define CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP

#include "Core/Application/I_Application.hpp"
#include "Define/DynamicLinker.hpp"

#include <chrono>
#include <memory>


namespace CE {
namespace Layers {
class ImGuiOpenGlLayer;
}

namespace Render::Context {
class I_Context;
class OpenGlContext;
}

namespace Window {
class I_Window;
class GlfwWindow;
}
}

namespace CE::Core::Application {

/**
 * @class GlfwApplication
 * @brief GLFW-specific application implementation
 * @details Provides a GLFW-specific implementation of the I_Application interface for platforms that support GLFW.
 *			Manages the application lifecycle and event handling using GLFW for window management and input handling.
 */
class CE_API GlfwApplication: public I_Application {
public:
	/**
	 * @brief Default constructor
	 */
	GlfwApplication();

	/**
	 * @brief Copy constructor (deleted)
	 */
	GlfwApplication(const GlfwApplication& other) = delete;

	/**
	 * @brief Move constructor (deleted)
	 */
	GlfwApplication(GlfwApplication&& other) noexcept = delete;

	/**
	 * @brief Destructor
	 */
	~GlfwApplication() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 */
	GlfwApplication& operator=(const GlfwApplication& other) = delete;

	/**
	 * @brief Move assignment operator (deleted)
	 */
	GlfwApplication& operator=(GlfwApplication&& other) noexcept = delete;

public:
	/**
	 * @brief Runs the application
	 * @details Starts the GLFW application loop
	 */
	void Run() override;

	/**
	 * @brief Quits the application
	 * @details Terminates the GLFW application loop
	 */
	void Quit() override;

	/**
	 * @brief Updates the application state
	 * @param deltaTime Time elapsed since the last update in seconds
	 * @details Called every frame to update the application. Updates all layers in the layer stack.
	 */
	void Tick(float deltaTime) override;

	/**
	 * @brief Handles events
	 * @param event Reference to the event to be processed
	 * @details Dispatches events to the appropriate layers in the layer stack
	 */
	void OnEvent(Events::I_Event& event) override;

	/**
	 * @brief Initializes the application window
	 * @param windowProps Window configuration properties
	 * @details Creates the application window and sets up event callbacks based on the provided window properties.
	 *			Initializes the appropriate input system based on the window API.
	 */
	void Init(const Types::Window::WindowProps& windowProps) override;

protected:
	/**
	 * @brief Initializes the application window
	 * @param windowProps Window configuration properties
	 * @details Creates the application window and sets up event callbacks based on the provided window properties.
	 *			Initializes the appropriate input system based on the window API.
	 */
	void InitWindow(const Types::Window::WindowProps& windowProps) override;

	/**
	 * @brief Initializes the renderer
	 * @details Initializes the rendering context based on the specified graphics API. For GLFW, this will typically involve setting up an OpenGL or Vulkan rendering context.
	 */
	void InitRenderer(Types::Render::GraphicsApi) override;

	/**
	 * @brief Initializes the ImGui layer
	 * @details Initializes the ImGui layer for rendering UI based on the specified graphics API. For GLFW with OpenGL, this will involve creating an ImGuiOpenGLLayer instance and pushing it as an overlay.
	 */
	void InitImGuiLayer(Types::Render::GraphicsApi) override;

public:
	/**
	 * @brief Gets the application window
	 * @return Reference to the application window
	 * @details Returns a reference to the application window, which can be used for various operations such as event handling and rendering.
	 */
	[[nodiscard]] Window::I_Window& GetWindow() const override;

	/**
	* @brief Gets the rendering context
	* @return Render::Context::I_Context& Reference to the rendering context
	 * @details Returns a reference to the application renderer context, which can be used for rendering operations and managing graphics resources.
	 */
	Render::Context::I_Context& GetRenderContext() const override;

private:
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	std::unique_ptr<Render::Context::OpenGlContext> _context; ///< Pointer to the OpenGL rendering context
	std::unique_ptr<Window::GlfwWindow> _window; ///< Pointer to the GLFW window

	Layers::ImGuiOpenGlLayer* _imguiLayer; ///< Pointer to the ImGui layer for rendering UI

	TimePoint _lastFrameTime; ///< Timestamp of the last frame for delta time calculation
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP
