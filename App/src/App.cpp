//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-13
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
	 * @details Creates the application with individual window parameters and initializes layers
	 */
	SandBox(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const TypeWindow::GraphicsApi graphicsApi):
		Application(title, width, height, VSync, graphicsApi) {}

	/**
	 * @brief Destructor
	 * @details Default destructor for cleanup
	 */
	~SandBox() override = default;
};

/**
 * @brief Factory function to create the application instance with default properties
 * @return Core::Application* Pointer to the newly created SandBox application
 * @details This function is called by the engine's entry point to instantiate the application.
 *			Must be implemented by the client application.
 */
std::unique_ptr<Core::Application> Core::CreateApplication() {
	return std::make_unique<SandBox>();
}

/**
 * @brief Factory function to create the application instance with window properties
 * @param windowProps Window configuration properties (title, width, height, VSync)
 * @return Core::Application* Pointer to the newly created SandBox application
 * @details This function is called by the engine's entry point to instantiate the application
 *			with specific window properties. Must be implemented by the client application.
 */
std::unique_ptr<Core::Application> Core::CreateApplication(const TypeWindow::WindowProps& windowProps) {
	return std::make_unique<SandBox>(windowProps);
}

/**
 * @brief Factory function to create the application instance with individual window parameters
 * @param title Window title string
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param VSync Enable or disable vertical synchronization
 * @param graphicsApi Graphics API to use for rendering
 * @return Core::Application* Pointer to the newly created SandBox application
 * @details This function is called by the engine's entry point to instantiate the application
 *			with individual window parameters. Must be implemented by the client application.
 */
std::unique_ptr<Core::Application> Core::CreateApplication(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const TypeWindow::GraphicsApi graphicsApi) {
	return std::make_unique<SandBox>(title, width, height, VSync, graphicsApi);
}
