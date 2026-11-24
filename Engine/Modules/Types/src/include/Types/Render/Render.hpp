//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

/**
 * @file Render.hpp
 * @brief Umbrella header for the Render sub-module of Types
 * @details Pulls in every rendering type the module defines, so a translation unit that needs several of them
 *			names one header instead of eight. Backend conversions are not here: they live with the backend
 *			that uses them (Core/Render/Conversion for now).
 */

#pragma once

#ifndef CE_TYPES_RENDER_RENDER_HPP
#define CE_TYPES_RENDER_RENDER_HPP

#include "Types/Render/AttachmentAction.hpp"
#include "Types/Render/BlendFactor.hpp"
#include "Types/Render/Camera.hpp"
#include "Types/Render/Compare.hpp"
#include "Types/Render/CullMode.hpp"
#include "Types/Render/GraphicsApi.hpp"
#include "Types/Render/PixelFormat.hpp"
#include "Types/Render/PrimitiveTopology.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/ShaderConstants.hpp"
#include "Types/Render/ShaderDataType.hpp"
#include "Types/Render/TextureUsage.hpp"


#endif //CE_TYPES_RENDER_RENDER_HPP
