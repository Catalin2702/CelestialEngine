//
// Module: CelestialEngine/Engine/Modules/Core/Render/Texture
// File: Texture.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Texture.hpp
 * @brief Umbrella header for the Texture part of the Core renderer
 * @details One include for everything a texture is made of: its interface and the per-backend textures.
 *			Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform, the Mac ones
 *			(Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_TEXTURE_TEXTURE_HPP
#define CE_CORE_RENDER_TEXTURE_TEXTURE_HPP

#include "Define/Config.hpp"

#include "Core/Render/Texture/I_Texture.hpp"

#include "Core/Render/Texture/Platforms/Common/OpenGl/OpenGlTexture.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#endif

#endif //CE_CORE_RENDER_TEXTURE_TEXTURE_HPP
