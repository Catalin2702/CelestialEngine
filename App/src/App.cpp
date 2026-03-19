//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
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
	 * @brief Destructor
	 * @details Default destructor for cleanup
	 */
	~SandBox() override = default;
};

std::unique_ptr<Core::Application> Core::CreateApplication() {
	return std::make_unique<SandBox>();
}
