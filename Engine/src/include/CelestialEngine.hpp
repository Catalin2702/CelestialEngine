//
// Module: Engine
// File: CelestialEngine.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

/**
 * @file CelestialEngine.hpp
 * @brief Main include file for the Celestial Engine
 * @details This is the primary header file that client applications should include.
 *          It provides access to all public engine APIs including:
 *          - Application framework (Core::Application)
 *          - Event system (Events::*)
 *          - Layer system (Layers::*)
 *          - Tools and utilities (Log, Chronometer, CMD)
 *          - Type definitions (WindowProps)
 *          - Entry point (main function)
 *
 *          Client applications only need to include this single header to access
 *          the full engine functionality.
 *
 * @author Catalin Chirosca
 * @date 2026-02-15
 */

#pragma once

#ifndef CE_CELESTIALENGINE_HPP
#define CE_CELESTIALENGINE_HPP

// ReSharper disable CppUnusedIncludeDirective

// ---- Use in CelestialEngine applications ------
#include <Core/Application.hpp>
// -----------------------------------------------

// ---- Event ------------------------------------
#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
// -----------------------------------------------

// ---- Layer ------------------------------------
#include <Layers/I_Layer.hpp>
#include <Layers/ImGuiMetalLayer.hpp>
#include <Layers/ImGuiOpenGlLayer.hpp>
// -----------------------------------------------

// ---- Tools ------------------------------------
#include <Tools/CMD/Window.hpp>
#include <Tools/Log/Log.hpp>
#include <Tools/Time/Chronometer.hpp>
// -----------------------------------------------

// ---- Types ------------------------------------
#include <Types/Window/WindowProps.hpp>
// -----------------------------------------------

// ---- Convenient namespace aliases -------------
namespace CMD = CE::Tools::CMD;						///< Command-line parsing utilities
namespace Core = CE::Core;							///< Core engine functionality
namespace Events = CE::Events;						///< Event system
namespace Log = CE::Tools::Log;						///< Logging system
namespace Layers = CE::Layers;						///< Layer management
namespace Time = CE::Tools::Time;					///< Time utilities
namespace TypeWindow = CE::Types::Window;			///< Window-related types
// -----------------------------------------------

// ---- Entry Point ------------------------------
#include <EntryPoint.hpp>
// -----------------------------------------------

#endif //CE_CELESTIALENGINE_HPP
