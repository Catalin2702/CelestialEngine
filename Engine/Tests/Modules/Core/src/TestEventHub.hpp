//
// Module: CelestialEngine/Engine/Tests/Modules/Core
// File: TestEventHub.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file TestEventHub.hpp
 * @brief The concrete event hub the Core tests drive, whichever window backend this build has
 * @details Most tests only need a real hub to dispatch through: the Dispatch* methods and the channels belong to the
 *			shared base, so every backend's hub serves equally. Which ones exist depends on CE_RENDER_BACKENDS, so this
 *			picks the first one compiled in - GLFW, the one the tests were written against, then Cocoa - and names the
 *			window API it reports. Tests that exercise a backend's own raw callbacks stay behind that backend's macro.
 */

#pragma once

#ifndef CE_TESTS_CORE_TESTEVENTHUB_HPP
#define CE_TESTS_CORE_TESTEVENTHUB_HPP

#include <Define/Config.hpp>
#include <Types/Window/WindowProps.hpp>

#if CE_WINDOW_GLFW
	#include <Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp>
#elif CE_WINDOW_COCOA
	#include <Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp>
#endif


namespace CE::Tests {

#if CE_WINDOW_GLFW
	using TestEventHubDispatcher = Core::GlfwEventHubDispatcher;
	inline constexpr auto TestEventHubWindowApi = Types::WindowApi::GLFW;
#elif CE_WINDOW_COCOA
	using TestEventHubDispatcher = Core::CocoaEventHubDispatcher;
	inline constexpr auto TestEventHubWindowApi = Types::WindowApi::Cocoa;
#else
	#error "No window backend compiled in: the Core tests need at least one event hub."
#endif

}

#endif //CE_TESTS_CORE_TESTEVENTHUB_HPP
