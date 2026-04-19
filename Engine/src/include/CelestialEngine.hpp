//
// Module: Engine
// File: CelestialEngine.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_CELESTIALENGINE_HPP
#define CE_CELESTIALENGINE_HPP

// ReSharper disable CppUnusedIncludeDirective

// ---- Use in CelestialEngine applications ------
// #include <Core/Application.hpp>
#include <Core/Application/I_Application.hpp>
#ifdef CE_PLATFORM_MACOS
#include <Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp>
#include <Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp>
#endif
// -----------------------------------------------

// ---- Event ------------------------------------
#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
// -----------------------------------------------

// ---- Input ------------------------------------
#include <Input/I_Input.hpp>
#include <Input/Platforms/Common/Glfw/GlfwInput.hpp>

#ifdef CE_PLATFORM_MACOS
	#include <Input/Platforms/Mac/Cocoa/CocoaInput.hpp>
#endif
// -----------------------------------------------

// ---- Layer ------------------------------------
#include <Layers/I_Layer.hpp>
#include <Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp>

#ifdef CE_PLATFORM_MACOS
#include <Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp>
#endif
// -----------------------------------------------

// ---- Tools ------------------------------------
#include <Tools/Log/Log.hpp>
// -----------------------------------------------

// ---- Types ------------------------------------
#include <Types/Window/WindowProps.hpp>
// -----------------------------------------------

// ---- Utility ----------------------------------
#include <Utility/CMD/Window.hpp>
#include <Utility/Time/Chronometer.hpp>
// -----------------------------------------------

// ---- Convenient namespace aliases -------------
namespace Application = CE::Core::Application;		///< Application framework
namespace CMD = CE::Utility::CMD;					///< Command-line parsing utilities
namespace Core = CE::Core;							///< Core engine functionality
namespace Events = CE::Events;						///< Event system
namespace Input = CE::Input;						///< Input handling
namespace Log = CE::Tools::Log;						///< Logging system
namespace Layers = CE::Layers;						///< Layer management
namespace Time = CE::Utility::Time;					///< Time utilities
namespace TypeWindow = CE::Types::Window;			///< Window-related types
// -----------------------------------------------

// ---- Entry Point ------------------------------
#include <EntryPoint.hpp>
// -----------------------------------------------

#endif //CE_CELESTIALENGINE_HPP
