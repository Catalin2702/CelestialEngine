//
// Module: CelestialEngine/Engine/Modules/Utility
// File: Utility.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

/**
 * @file Utility.hpp
 * @brief Umbrella header for the Utility module
 * @details One include for the Utility module: delegates and dispatchers, string manipulation, ranges and time - the
 *			generic helpers any target can link, with no window, graphics API or ImGui behind them. The file system has
 *			a module of its own (IO/IO.hpp); the command line options, Config and the ImGui glue are in Core.
 *
 *			No folder here needs an umbrella of its own: each holds a single header, or - Delegate - already has one
 *			named after the folder that is the main header of the two.
 */

#pragma once

#ifndef CE_UTILITY_HPP
#define CE_UTILITY_HPP

#include "Utility/Delegate/Delegate.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include "Utility/Manipulation/String.hpp"

#include "Utility/Range/Enumerate.hpp"

#include "Utility/Time/Chronometer.hpp"

#endif //CE_UTILITY_HPP
