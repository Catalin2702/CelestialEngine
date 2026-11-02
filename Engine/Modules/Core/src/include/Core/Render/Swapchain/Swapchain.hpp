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
 *			Backend-specific headers come along too, each only where its backend is compiled in - CE_RENDER_* for
 *			the graphics ones, CE_WINDOW_* for the window ones, both chosen by CE_RENDER_BACKENDS - the same split
 *			Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP

#include "Define/Config.hpp"

#include "Core/Render/Swapchain/I_Swapchain.hpp"

#if CE_RENDER_OPENGL
#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#endif

#endif //CE_CORE_RENDER_SWAPCHAIN_SWAPCHAIN_HPP
