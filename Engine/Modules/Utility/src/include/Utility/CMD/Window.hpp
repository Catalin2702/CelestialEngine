//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_UTILITY_CMD_WINDOW_HPP
#define CE_UTILITY_CMD_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"


namespace CE::Utility {

/**
 * @brief Parses command-line arguments to create window properties
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Types::WindowProps Window configuration extracted from arguments
 * @details Each flag takes its value as the NEXT argument (e.g. "-w 1280"). Supported flags (long / short form):
 *			- --title / -t : Window title
 *			- --width / -w : Window width in pixels
 *			- --height / -h : Window height in pixels
 *			- --vsync / -v : Enables/disables vertical synchronization
 *			- --refresh / -r : Target refresh rate used while VSync is off
 *			- --graphics-api / -g : Graphics API (e.g. metal, opengl)
 *			- --window-api / -wa : Windowing API (e.g. cocoa, glfw)
 *			Unrecognized or valueless flags are ignored and the corresponding defaults are kept.
 *
 *			Without -g the graphics API is the build's default (Types::GetDefaultGraphicsApi, the first entry of
 *			CE_RENDER_BACKENDS); without -wa the window API is the one that API is paired with
 *			(Types::GetDefaultWindowApi). This only parses: an API this build lacks is returned as asked, and rejected
 *			by ValidateWindowProps.
 */
CE_UTILITY_API Types::WindowProps GetWindowProps(int argc, const char* argv[]);

/**
 * @brief Checks that the engine can actually open the window the properties describe
 * @param windowProps The parsed properties, typically from GetWindowProps
 * @throws std::runtime_error When the graphics or window API is not compiled into this build (CE_RENDER_BACKENDS), or
 *		   when the two cannot work together (e.g. Metal in a GLFW window). The message names what is available.
 * @details Run once at startup, before anything is built from the properties: the factories would fail too, but later,
 *			one at a time, and without the list of what the build does offer.
 */
CE_UTILITY_API void ValidateWindowProps(const Types::WindowProps& windowProps);

}

#endif //CE_UTILITY_CMD_WINDOW_HPP
