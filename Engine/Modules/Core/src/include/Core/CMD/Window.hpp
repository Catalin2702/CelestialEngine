//
// Module: CelestialEngine/Engine/Modules/Core/CMD
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_CORE_CMD_WINDOW_HPP
#define CE_CORE_CMD_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"


namespace CE::Core {

/**
 * @brief Parses the window options out of the command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Types::WindowProps Window configuration extracted from arguments, with an empty title
 * @details Each flag takes its value as the NEXT argument (e.g. "-w 1280"). Supported flags (long / short form):
 *			- --width / -w : Window width in pixels
 *			- --height / -h : Window height in pixels
 *			- --vsync / -v : Enables/disables vertical synchronization
 *			- --refresh / -r : Target refresh rate used while VSync is off
 *			- --graphics-api / -g : Graphics API (e.g. metal, opengl)
 *			- --window-api / -wa : Windowing API (e.g. cocoa, glfw)
 *			Unrecognized or valueless flags are ignored and the corresponding defaults are kept.
 *
 *			The title is not an option: it is the application's display name (BUNDLE_NAME in ce_configure_application),
 *			which InitFromCommandLine stores in the parsed properties. A "--title" / "-t" is reported as unsupported.
 *
 *			Without -g the graphics API is the build's default (Types::GetDefaultGraphicsApi, the first entry of
 *			CE_RENDER_BACKENDS); without -wa the window API is the one that API is paired with
 *			(Types::GetDefaultWindowApi). This only parses: an API this build lacks is returned as asked, and rejected
 *			by ValidateWindowProps. Nothing is stored either - InitFromCommandLine (CMD/CommandLine.hpp) is what parses,
 *			validates and hands the result to Config, which is where the rest of the engine reads it from.
 */
[[nodiscard]] CE_CORE_API Types::WindowProps ParseWindowArgs(int argc, const char* argv[]);

/**
 * @brief Checks that the engine can actually open the window the properties describe
 * @param windowProps The parsed properties, typically from ParseWindowArgs
 * @throws std::runtime_error When the graphics or window API is not compiled into this build (CE_RENDER_BACKENDS), or
 *		   when the two cannot work together (e.g. Metal in a GLFW window). The message names what is available.
 * @details Run once at startup by InitFromCommandLine, before anything is built from the properties: the factories would fail too, but later,
 *			one at a time, and without the list of what the build does offer.
 */
CE_CORE_API void ValidateWindowProps(const Types::WindowProps& windowProps);

}

#endif //CE_CORE_CMD_WINDOW_HPP
