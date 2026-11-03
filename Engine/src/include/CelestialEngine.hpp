//
// Module: Engine
// File: CelestialEngine.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef CE_CELESTIALENGINE_HPP
#define CE_CELESTIALENGINE_HPP

// ReSharper disable CppUnusedIncludeDirective

#include <Define/DynamicLinker.hpp>

// ---- Use in CelestialEngine applications ------
// One concrete class for every backend: the window, the renderer and the run loop are chosen at
// runtime from the window properties, so there is nothing here for a client to pick between.
#include <Core/Core.hpp>
// -----------------------------------------------

// ---- Event ------------------------------------
#include <Events/Events.hpp>
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

#endif //CE_CELESTIALENGINE_HPP
