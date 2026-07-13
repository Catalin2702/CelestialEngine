//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
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
 * @return Types::Window::WindowProps Window configuration extracted from arguments
 * @details Parses command-line arguments looking for window configuration flags:
 *			- --title="Window Title" : Sets window title
 *			- --width=1280 : Sets window width in pixels
 *			- --height=720 : Sets window height in pixels
 *			- --vsync=true/false : Enables/disables vertical synchronization
 *			- --graphics-api=d3d11/d3d12/metal/opengl/vulkan : Sets the graphics API to use
 *			Returns default window properties if no arguments are provided or if
 *			parsing fails. Uses case-insensitive matching for argument names.
 */
CE_API Types::WindowProps GetWindowProps(int argc, const char* argv[]);

}

#endif //CE_UTILITY_CMD_WINDOW_HPP
