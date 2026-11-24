//
// Module: CelestialEngine/Engine/Modules/Utility
// File: Utility.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

/**
 * @file Utility.hpp
 * @brief Umbrella header for the Utility module
 * @details One include for the Utility module: command line, config, delegates and dispatchers, string manipulation,
 *			ranges and time. The file system has a module of its own (IO/IO.hpp).
 *
 *			Utility/ImGui/ImGui.hpp is left out on purpose: it includes imgui.h, and this header is pulled into almost
 *			every file of the engine. The few files that convert ImGui keys include it directly.
 *
 *			No folder here needs an umbrella of its own: each holds a single header, or - Delegate - already has one
 *			named after the folder that is the main header of the two.
 */

#pragma once

#ifndef CE_UTILITY_HPP
#define CE_UTILITY_HPP

#include "Utility/CMD/CommandLine.hpp"
#include "Utility/CMD/Window.hpp"

#include "Utility/Config/Config.hpp"

#include "Utility/Delegate/Delegate.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include "Utility/Manipulation/String.hpp"

#include "Utility/Range/Enumerate.hpp"

#include "Utility/Time/Chronometer.hpp"

#endif //CE_UTILITY_HPP
