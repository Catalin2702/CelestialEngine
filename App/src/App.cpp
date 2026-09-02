//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include <CelestialEngine.hpp>

#include <format>
#include <memory>

std::unique_ptr<Core::I_Application> Core::CreateApplication(const int argc, const char* argv[]) {
	Utility::Config::SetWindowProps(Utility::GetWindowProps(argc, argv));
	const auto& windowProps = Utility::Config::GetWindowProps();
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
	// Both backends bring themselves up in their constructor (window, renderer, event plumbing), so only the opt-in
	// ImGui overlay is left to push here.
	app->InitImGuiLayer();
	return app;
}
