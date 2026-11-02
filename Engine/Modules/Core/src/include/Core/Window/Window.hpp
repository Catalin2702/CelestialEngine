//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Window.hpp
 * @brief Umbrella header for the Window sub-module of Core
 * @details One include for everything a window is made of: the platform and window interfaces, the GLFW backend
 *			and, on macOS, the Cocoa one with the process-wide menu bar. Backend-specific headers come along too,
 *			each only where its backend is compiled in - CE_RENDER_* for the graphics ones, CE_WINDOW_* for the
 *			window ones, both chosen by CE_RENDER_BACKENDS - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_WINDOW_WINDOW_HPP
#define CE_CORE_WINDOW_WINDOW_HPP

#include "Define/Config.hpp"

#include "Core/Window/I_Platform.hpp"
#include "Core/Window/I_Window.hpp"

#if CE_WINDOW_GLFW
#include "Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"
#endif

#if CE_PLATFORM_MACOS
#include "Core/Window/Platforms/Mac/MacMenuBar.hpp"
#endif

#if CE_WINDOW_COCOA
#include "Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#endif

#endif //CE_CORE_WINDOW_WINDOW_HPP
