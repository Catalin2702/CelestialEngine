//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

/**
 * @file Window.hpp
 * @brief Umbrella header for the Window sub-module of Types
 * @details One include for the window description types. The GLFW handle deleter used to live here too; it
 *			moved to Core (Core/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp) because its one call to
 *			glfwDestroyWindow was enough to link the whole of GLFW into libCE_Types, which is not a GLFW client.
 */

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOW_HPP
#define CE_TYPES_WINDOW_WINDOW_HPP

#include "Types/Window/WindowProps.hpp"

#endif //CE_TYPES_WINDOW_WINDOW_HPP
