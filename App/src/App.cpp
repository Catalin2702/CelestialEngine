//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#include <CelestialEngine.hpp>
#include <memory>


/**
 * @brief SandBox application class - main application entry point
 * @details Concrete implementation of the Application class that sets up the example layer
 *			and ImGui overlay layer. This class demonstrates how to create a basic application
 *			using the Celestial Engine framework.
 */
// class SandBox final: public Application::CocoaApplication {
// public:
// 	/**
// 	 * @brief Default constructor
// 	 * @details Creates the application with default window properties and initializes layers
// 	 */
// 	SandBox() = default;
//
// 	/**
// 	 * @brief Destructor
// 	 * @details Default destructor for cleanup
// 	 */
// 	~SandBox() override = default;
// };

std::unique_ptr<Application::I_Application> Application::CreateApplication(const int argc, const char* argv[]) {
	const auto windowProps = CMD::GetWindowProps(argc, argv);
	auto app = std::make_unique<CocoaApplication>();
	app->Init(windowProps);
	return app;
}
