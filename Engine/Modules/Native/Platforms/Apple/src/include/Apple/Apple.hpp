//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple
// File: Apple.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Apple.hpp
 * @brief Umbrella header for the Apple module
 * @details One include for the Apple platform layer: the metal-cpp wrappers and, when Metal is compiled in
 *			(CE_RENDER_METAL), the ImGui bridge. Only meaningful on macOS, where the module is built at all.
 */

#pragma once

#ifndef CE_NATIVE_APPLE_APPLE_HPP
#define CE_NATIVE_APPLE_APPLE_HPP

#include "Define/Config.hpp"

#if CE_RENDER_METAL
#include "Apple/Bridge/ImGui/ImGuiBridge.h"
#endif

#include "Apple/MetalCpp/MetalCpp.hpp"

#endif //CE_NATIVE_APPLE_APPLE_HPP
