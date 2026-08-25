//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include <CelestialEngine.hpp>

#include <format>
#include <memory>

std::unique_ptr<Core::I_Application> Core::CreateApplication(const int argc, const char* argv[]) {
	Utility::Config::StSetWindowProps(Utility::GetWindowProps(argc, argv));
	const auto& windowProps = Utility::Config::StGetWindowProps();
	std::unique_ptr<I_Application> app;
	switch (windowProps.windowApi) {
		case Types::WindowApi::GLFW: {
			app = std::make_unique<GlfwApplication>();
			break;
		}
#if CE_PLATFORM_MACOS
		case Types::WindowApi::Cocoa: {
			app = std::make_unique<CocoaApplication>();
			break;
		}
#endif
		default: {
			const auto error = std::format("Application::CreateApplication: Unsupported window API specified in window properties. Window API: {}", windowProps.windowApi);
			Tools::Log::LogCoreError(error);
			throw std::runtime_error(error);
		}
	}
	app->Init();
	app->InitImGuiLayer();
	return app;
}
