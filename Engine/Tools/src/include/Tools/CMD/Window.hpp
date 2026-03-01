//
// Module: Tools/CMD
// File: Window.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TOOLS_CMD_WINDOW_HPP
#define CE_TOOLS_CMD_WINDOW_HPP

#include "Types/Window/WindowProps.hpp"

/**
 * @namespace CE::Tools::CMD
 * @brief Command-line argument parsing utilities
 * @details Provides functions to parse command-line arguments and extract configuration
 */
namespace CE::Tools::CMD {

/**
 * @brief Parses command-line arguments to create window properties
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Types::Window::WindowProps Window configuration extracted from arguments
 * @details Parses command-line arguments looking for window configuration flags:
 *          - --title="Window Title" : Sets window title
 *          - --width=1280 : Sets window width in pixels
 *          - --height=720 : Sets window height in pixels
 *          - --vsync=true/false : Enables/disables vertical synchronization
 *          Returns default window properties if no arguments are provided or if
 *          parsing fails. Uses case-insensitive matching for argument names.
 */
Types::Window::WindowProps GetWindowProps(int argc, char* argv[]);

}

#endif //CE_TOOLS_CMD_WINDOW_HPP
