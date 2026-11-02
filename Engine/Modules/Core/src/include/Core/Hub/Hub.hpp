//
// Module: CelestialEngine/Engine/Modules/Core/Hub
// File: Hub.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Hub.hpp
 * @brief Umbrella header for the Hub sub-module of Core
 * @details One include for everything the event hub is made of: the dispatcher interfaces, the multicast channels
 *			and the per-backend hubs that translate native callbacks into engine events. Backend-specific headers
 *			come along too, each only where its backend is compiled in - CE_RENDER_* for the graphics ones,
 *			CE_WINDOW_* for the window ones, both chosen by CE_RENDER_BACKENDS - the same split
 *			Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_HUB_HUB_HPP
#define CE_CORE_HUB_HUB_HPP

#include "Define/Config.hpp"

#include "Core/Hub/Events/I_ApplicationEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_EventHubDispatcher.hpp"
#include "Core/Hub/Events/I_KeyboardEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_MouseEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_RenderEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_WindowEventHubDispatcher.hpp"

#if CE_WINDOW_GLFW
#include "Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"
#endif

#if CE_WINDOW_COCOA
#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#endif

#endif //CE_CORE_HUB_HUB_HPP
