//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: EventHubDispatcherFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Hub/Hub.hpp"
#include "Tools/Tools.hpp"

#if CE_WINDOW_COCOA
	#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_EventHubDispatcher> I_EventHubDispatcher::MakeEventHubDispatcher(const Types::WindowApi windowApi) {
	switch (windowApi) {
#if CE_WINDOW_GLFW
		case Types::WindowApi::GLFW:
			return std::make_unique<GlfwEventHubDispatcher>();
#endif

#if CE_WINDOW_COCOA
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaEventHubDispatcher>();
#endif

		default: {
			// Either a backend that does not exist yet, or one this build was configured without (CE_RENDER_BACKENDS).
			const auto error = std::format("I_EventHubDispatcher::MakeEventHubDispatcher: {} is not compiled into this build!", windowApi);
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
