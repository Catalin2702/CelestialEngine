//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac
// File: MacMenuBar.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_WINDOW_MACMENUBAR_HPP
#define CE_CORE_WINDOW_MACMENUBAR_HPP

#include "Define/DynamicLinker.hpp"


namespace CE::Core {

/**
 * @brief Builds the macOS menu bar and installs it on the running NSApplication
 * @details Every macOS process gets a menu bar whether it asks for one or not; the only question is whose. Without
 *			this the bar belongs to whatever put the process on screen - an empty one under Cocoa, GLFW's own default
 *			under GLFW - and neither can reach the engine.
 *
 *			It sits at Platforms/Mac rather than under an API folder because it is shared: the menu bar belongs to the
 *			process, not to the windowing library, and GLFW creates a real NSApplication on macOS exactly as the Cocoa
 *			backend does. Both platforms call this from Prepare(), and the one AppKit object it needs -
 *			NSApplication - is reached through sharedApplication rather than passed in, because that is the same
 *			singleton either way.
 *
 *			What it is *not* is a template over the application type, which is what the old MacMenuBar had to be when
 *			there were two of them. There is one Application now, so the menu items reach it through Application::Get
 *			and the menu bar needs to know nothing about who is running.
 *
 *			Idempotent: installing twice replaces the bar with an identical one.
 */
CE_CORE_API void InstallMacMenuBar();

}

#endif //CE_CORE_WINDOW_MACMENUBAR_HPP
