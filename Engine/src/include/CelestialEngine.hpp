//
// Module: Engine
// File: CelestialEngine.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_CELESTIALENGINE_HPP
#define CE_CELESTIALENGINE_HPP

// ReSharper disable CppUnusedIncludeDirective

#include <Define/DynamicLinker.hpp>

// ---- Use in CelestialEngine applications ------
// #include <Core/Application.hpp>
#include <Core/Application/I_Application.hpp>
#include <Core/Application/Application.hpp>
#include <Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp>
#if CE_PLATFORM_MACOS
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
#include <Core/Input/Input.hpp>
// -----------------------------------------------

// ---- Layer ------------------------------------
#include <Core/Layers/I_Layer.hpp>
#include <Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp>

#if CE_PLATFORM_MACOS
#include <Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp>
#endif
// -----------------------------------------------

// ---- Tools ------------------------------------
#include <Tools/Tools.hpp>
// -----------------------------------------------

// ---- Types ------------------------------------
#include <Types/Types.hpp>
// -----------------------------------------------

// ---- Utility ----------------------------------
#include <Utility/Utility.hpp>
// -----------------------------------------------

// ---- Engine Info -------------------------------
namespace CE {
	/**
	 * @brief Returns the CelestialEngine version this application was built against, as "MAJOR.MINOR.PATCH"
	 */
	CE_ENGINE_API const char* GetEngineVersion();
}
// -----------------------------------------------

// ---- Convenient namespace aliases -------------
namespace Core = CE::Core;		///< Application framework
namespace Events = CE::Events;						///< Event system
namespace Tools = CE::Tools;						///< Logging and tooling
namespace Utility = CE::Utility;					///< Utility
// -----------------------------------------------

// ---- Entry Point ------------------------------
#include <EntryPoint.hpp>
// -----------------------------------------------

#endif //CE_CELESTIALENGINE_HPP
