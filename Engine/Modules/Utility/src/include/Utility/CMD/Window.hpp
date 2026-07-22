//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
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
 */
CE_API Types::WindowProps GetWindowProps(int argc, const char* argv[]);

}

#endif //CE_UTILITY_CMD_WINDOW_HPP
