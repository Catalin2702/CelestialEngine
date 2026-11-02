//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: Device.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Device.hpp
 * @brief Umbrella header for the Device part of the Core renderer
 * @details One include for everything the graphics device is made of: its interface and the per-backend devices.
 *			Backend- specific headers come along too, each only where its backend is compiled in - CE_RENDER_* for
 *			the graphics ones, CE_WINDOW_* for the window ones, both chosen by CE_RENDER_BACKENDS - the same split
 *			Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_DEVICE_HPP
#define CE_CORE_RENDER_DEVICE_DEVICE_HPP

#include "Define/Config.hpp"

#include "Core/Render/Device/I_GraphicDevice.hpp"

#if CE_RENDER_OPENGL
#include "Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#endif

#endif //CE_CORE_RENDER_DEVICE_DEVICE_HPP
