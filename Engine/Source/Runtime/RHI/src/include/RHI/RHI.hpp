//
// Module: CelestialEngine/Engine/Source/Runtime/RHI
// File: RHI.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

/**
 * @file RHI.hpp
 * @brief Umbrella header for the RHI module
 * @details One include for the whole Render Hardware Interface, through the umbrella of each of its parts. The
 *			backend conversion tables (RHI/Conversion) are left out: only the backends themselves use them.
 */

#pragma once

#ifndef CE_RHI_RHI_HPP
#define CE_RHI_RHI_HPP

#include "RHI/Buffer/Buffer.hpp"

#include "RHI/Command/Command.hpp"

#include "RHI/Device/Device.hpp"

#include "RHI/Pipeline/Pipeline.hpp"

#include "RHI/Shader/Shader.hpp"

#include "RHI/Surface/Surface.hpp"

#include "RHI/Swapchain/Swapchain.hpp"

#include "RHI/Texture/Texture.hpp"

#endif //CE_RHI_RHI_HPP
