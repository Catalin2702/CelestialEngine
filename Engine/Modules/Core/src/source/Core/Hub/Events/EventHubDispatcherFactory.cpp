//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: EventHubDispatcherFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Hub/Events/I_EventHubDispatcher.hpp"
#include "Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"
#include "Tools/Log/Log.hpp"

#if CE_PLATFORM_MACOS
	#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#endif

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_EventHubDispatcher> I_EventHubDispatcher::MakeEventHubDispatcher(const Types::WindowApi windowApi) {
	switch (windowApi) {
		case Types::WindowApi::GLFW:
			return std::make_unique<GlfwEventHubDispatcher>();

#if CE_PLATFORM_MACOS
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaEventHubDispatcher>();
#endif

		default: {
			constexpr auto error = "I_EventHubDispatcher::MakeEventHubDispatcher: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
