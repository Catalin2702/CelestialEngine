//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: Swapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Swapchain.hpp
 * @brief Umbrella header for the Swapchain part of the Core renderer
 * @details One include for everything a swapchain is made of: its interface and the per-backend swapchains.
 *			Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform, the Mac ones
 *			(Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP

#include "Define/Config.hpp"

#include "Core/Render/Swapchain/I_Swapchain.hpp"

#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#endif

#endif //CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP
