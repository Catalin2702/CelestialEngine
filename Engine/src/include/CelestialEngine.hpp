//
// Module: Engine
// File: CelestialEngine.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

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

// ---- Name spaces ------------------------------
namespace CMD = CE::Tools::CMD;
namespace Core = CE::Core;
namespace Events = CE::Events;
namespace Log = CE::Tools::Log;
namespace Layers = CE::Layers;
namespace Time = CE::Tools::Time;
namespace TypeWindow = CE::Types::Window;
// -----------------------------------------------

// ---- Entry Point ------------------------------
#include <EntryPoint.hpp>
// -----------------------------------------------

#endif //CE_CELESTIALENGINE_HPP
