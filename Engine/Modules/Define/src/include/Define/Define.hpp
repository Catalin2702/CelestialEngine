//
// Module: CelestialEngine/Engine/Modules/Define
// File: Define.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

/**
 * @file Define.hpp
 * @brief Umbrella header for the Define module
 * @details One include for the macros every module may use: export macros, platform and build configuration,
 *			binding, bits and logging. A macro that names a type of a higher module lives in that module instead
 *			(EVENT_CLASS_TYPE in Events/EventMacros.hpp), since Define links nothing but CE_Config.
 */

#pragma once

#ifndef CE_DEFINE_DEFINE_HPP
#define CE_DEFINE_DEFINE_HPP

#include "Define/Bind.hpp"
#include "Define/Config.hpp"
#include "Define/DynamicLinker.hpp"
#include "Define/Log.hpp"
#include "Define/Types.hpp"

#endif //CE_DEFINE_DEFINE_HPP
