//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include <CelestialEngine.hpp>
#include <memory>

std::unique_ptr<Application::I_Application> Application::CreateApplication(const int argc, const char* argv[]) {
	const auto windowProps = CMD::GetWindowProps(argc, argv);
	std::unique_ptr<I_Application> app;
	switch (windowProps.windowApi) {
		case Types::Window::WindowApi::GLFW: {
			app = std::make_unique<GlfwApplication>();
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case Types::Window::WindowApi::Cocoa: {
			app = std::make_unique<CocoaApplication>();
			break;
		}
#endif
		default: {
			CE_CORE_ERROR("Application::CreateApplication: Unsupported window API specified in window properties. Window API: {0}", windowProps.windowApi);
			throw std::runtime_error("Unsupported window API specified in window properties");
		}
	}
	app->Init(windowProps);
	app->InitImGuiLayer(windowProps.graphicsApi);
	return app;
}
