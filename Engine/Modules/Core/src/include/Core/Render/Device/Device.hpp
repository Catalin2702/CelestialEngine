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
 *			Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform, the Mac ones
 *			(Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_DEVICE_HPP
#define CE_CORE_RENDER_DEVICE_DEVICE_HPP

#include "Define/Config.hpp"

#include "Core/Render/Device/I_GraphicDevice.hpp"

#include "Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#endif

#endif //CE_CORE_RENDER_DEVICE_DEVICE_HPP
