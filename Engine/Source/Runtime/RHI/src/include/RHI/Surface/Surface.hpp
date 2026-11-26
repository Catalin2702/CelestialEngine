//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Surface
// File: Surface.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

/**
 * @file Surface.hpp
 * @brief Umbrella header for the Surface part of the RHI
 * @details One include for the surfaces a window can expose to a swapchain. Interfaces only - a window implements them
 *			- with the Metal one limited to macOS like every other Mac header.
 */

#pragma once

#ifndef CE_RHI_SURFACE_SURFACE_HPP
#define CE_RHI_SURFACE_SURFACE_HPP

#include "Define/Config.hpp"

#include "RHI/Surface/I_Surface.hpp"

#if CE_RENDER_OPENGL
#include "RHI/Surface/Common/OpenGl/I_OpenGlSurface.hpp"
#endif

#if CE_RENDER_METAL
#include "RHI/Surface/Mac/Metal/I_MetalSurface.hpp"
#endif

#endif //CE_RHI_SURFACE_SURFACE_HPP
