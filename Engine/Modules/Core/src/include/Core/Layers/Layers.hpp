//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: Layers.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Layers.hpp
 * @brief Umbrella header for the Layers sub-module of Core
 * @details One include for the layer interface, the stack that orders the layers, the scene layer and the ImGui
 *			overlay. The ImGui backends come in through Core/Layers/ImGui/ImGui.hpp, with its own platform split.
 */

#pragma once

#ifndef CE_CORE_LAYERS_LAYERS_HPP
#define CE_CORE_LAYERS_LAYERS_HPP

#include "Core/Layers/I_Layer.hpp"
#include "Core/Layers/LayerStack.hpp"

#include "Core/Layers/ImGui/ImGui.hpp"

#include "Core/Layers/Scene/SceneLayer.hpp"

#endif //CE_CORE_LAYERS_LAYERS_HPP
