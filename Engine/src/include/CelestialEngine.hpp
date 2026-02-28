//
// Created by Catalin Chirosca on 2026-02-15.
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
#include <Tools/Time/Timer.hpp>
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
