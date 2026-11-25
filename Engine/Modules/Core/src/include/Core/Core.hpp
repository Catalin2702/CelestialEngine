//
// Module: CelestialEngine/Engine/Modules/Core
// File: Core.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

/**
 * @file Core.hpp
 * @brief Umbrella header for the Core module
 * @details One include for the whole of Core, through the umbrella of each sub-module.
 *
 *			Application and Input have no umbrella of their own: Application/Application.hpp is the only header there,
 *			and Input/Input.hpp already includes the input state and KeyShortcut. CMD and Config are listed directly too.
 *			Layers/ImGui/ImGuiHelpers.hpp is left out on purpose: it includes imgui.h, and this header reaches every
 *			client. The ImGui layers include it directly.
 */

#pragma once

#ifndef CE_CORE_CORE_HPP
#define CE_CORE_CORE_HPP

#include "Core/Application/Application.hpp"
#include "Core/Application/ApplicationSpecification.hpp"
#include "Core/Application/I_ApplicationClient.hpp"

#include "Core/CMD/CommandLine.hpp"
#include "Core/CMD/Window.hpp"

#include "Core/Config/Config.hpp"

#include "Core/Hub/Hub.hpp"

#include "Core/Input/Input.hpp"

#include "Core/Layers/Layers.hpp"

#include "Core/Render/Render.hpp"

#include "Core/RunLoop/RunLoop.hpp"

#include "Core/Window/Window.hpp"

#include "Core/Version.hpp"

#endif //CE_CORE_CORE_HPP
