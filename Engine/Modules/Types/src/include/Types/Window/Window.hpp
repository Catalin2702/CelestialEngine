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
 * @details One include for the window description types and the GLFW handle deleter that goes with them.
 *			The deleter is not guarded by a platform macro because GLFW is the cross-platform backend: it is
 *			built everywhere, and only a forward-declared GLFWwindow reaches this header.
 */

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOW_HPP
#define CE_TYPES_WINDOW_WINDOW_HPP

#include "Types/Window/WindowProps.hpp"

#include "Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"

#endif //CE_TYPES_WINDOW_WINDOW_HPP
