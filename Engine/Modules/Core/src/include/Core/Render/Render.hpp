//
// Module: CelestialEngine/Engine/Modules/Core/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Render.hpp
 * @brief Umbrella header for the Render sub-module of Core
 * @details One include for the whole renderer, through the umbrella of each of its parts.
 *
 *			The camera has no umbrella of its own because Camera/Camera.hpp is already the camera class, so its headers
 *			are listed here directly, next to the controller and projection umbrellas.
 */

#pragma once

#ifndef CE_CORE_RENDER_RENDER_HPP
#define CE_CORE_RENDER_RENDER_HPP

#include "Core/Render/Buffer/Buffer.hpp"

#include "Core/Render/Camera/Camera.hpp"
#include "Core/Render/Camera/CameraDescriptor.hpp"
#include "Core/Render/Camera/CameraRig.hpp"
#include "Core/Render/Camera/Controller/Controller.hpp"
#include "Core/Render/Camera/Projection/Projection.hpp"

#include "Core/Render/Command/Command.hpp"

#include "Core/Render/Device/Device.hpp"

#include "Core/Render/Pipeline/Pipeline.hpp"

#include "Core/Render/Renderer/Renderer.hpp"

#include "Core/Render/Shader/Shader.hpp"

#include "Core/Render/Surface/Surface.hpp"

#include "Core/Render/Swapchain/Swapchain.hpp"

#include "Core/Render/Texture/Texture.hpp"

#endif //CE_CORE_RENDER_RENDER_HPP
