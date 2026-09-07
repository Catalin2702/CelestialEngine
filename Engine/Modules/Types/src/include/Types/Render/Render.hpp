//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

/**
 * @file Render.hpp
 * @brief Umbrella header for the Render sub-module of Types
 * @details Pulls in every rendering type the module defines, so a translation unit that needs several of them
 *			names one header instead of eight. Anything that needs a single enum should keep including that
 *			enum's own header - GraphicsApi.hpp, PixelFormat.hpp, ... - because this one drags in the
 *			backend-specific tables (metal-cpp on macOS) with it.
 */

#pragma once

#ifndef CE_TYPES_RENDER_RENDER_HPP
#define CE_TYPES_RENDER_RENDER_HPP

#include "Define/Config.hpp"

#include "Types/Render/AttachmentAction.hpp"
#include "Types/Render/BlendFactor.hpp"
#include "Types/Render/Camera.hpp"
#include "Types/Render/Compare.hpp"
#include "Types/Render/CullMode.hpp"
#include "Types/Render/GraphicsApi.hpp"
#include "Types/Render/PixelFormat.hpp"
#include "Types/Render/PrimitiveTopology.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/ShaderDataType.hpp"
#include "Types/Render/TextureUsage.hpp"

#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"

#if CE_PLATFORM_MACOS
#include "Types/Render/Platforms/Mac/Metal/Metal.hpp"
#endif

#endif //CE_TYPES_RENDER_RENDER_HPP
