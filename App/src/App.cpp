//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include <CelestialEngine.hpp>
#include <memory>


/**
 * @brief SandBox application class - main application entry point
 * @details Concrete implementation of the Application class that sets up the example layer
 *			and ImGui overlay layer. This class demonstrates how to create a basic application
 *			using the Celestial Engine framework.
 */
class SandBox final: public Core::Application {
public:
	/**
	 * @brief Default constructor
	 * @details Creates the application with default window properties and initializes layers
	 */
	SandBox() = default;

	/**
	 * @brief Constructor with window properties
	 * @param windowProps Window configuration properties (title, width, height, VSync)
	 * @details Creates the application with specified window properties and initializes layers
	 */
	SandBox(const TypeWindow::WindowProps& windowProps):Application(windowProps) {}

	/**
	 * @brief Constructor with individual window parameters
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 * @param graphicsApi Graphics API to use for rendering
	 * @param windowApi Windowing API to use for window management
	 * @details Creates the application with individual window parameters and initializes layers
	 */
	SandBox(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const TypeWindow::GraphicsApi graphicsApi, const TypeWindow::WindowApi windowApi):
		Application(title, width, height, VSync, graphicsApi, windowApi) {}

	/**
	 * @brief Destructor
	 * @details Default destructor for cleanup
	 */
	~SandBox() override = default;
};

std::unique_ptr<Core::Application> Core::CreateApplication() {
	return std::make_unique<SandBox>();
}

std::unique_ptr<Core::Application> Core::CreateApplication(const TypeWindow::WindowProps& windowProps) {
	return std::make_unique<SandBox>(windowProps);
}

std::unique_ptr<Core::Application> Core::CreateApplication(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const TypeWindow::GraphicsApi graphicsApi, const TypeWindow::WindowApi windowApi) {
	return std::make_unique<SandBox>(title, width, height, VSync, graphicsApi, windowApi);
}
