//
// Module: CelestialEngine/Engine/Modules/Define
// File: LogDetail.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file LogDetail.hpp
 * @brief The machinery shared by the client (Define/Log.hpp) and engine (Define/CoreLog.hpp) logging macros
 * @details A header of its own so the two can both include it without including each other.
 */

#pragma once

#ifndef CE_DEFINE_LOGDETAIL_HPP
#define CE_DEFINE_LOGDETAIL_HPP

#include <cassert>

/**
 * @brief Implementation detail of every CE_* and CE_CORE_* macro - not meant to be used directly
 * @param getter The CE::Tools::Log accessor of the logger to write to (GetCoreLogger or GetClientLogger)
 * @param level The spdlog member to call (trace, info, warn, error, critical)
 * @param ... Format string and arguments, fmt-style
 * @details Asserts the logger exists, because logging before Log::Init is a caller's bug, but also checks it: the assert
 *			is compiled out under NDEBUG, and a Release or Dist build must not dereference a null logger over it.
 */
#define CE_DETAIL_LOG(getter, level, ...) do {                                       \
	const auto& ceDetailLogger = CE::Tools::Log::getter();                           \
	assert(ceDetailLogger and #getter " returned no logger: call Log::Init first");  \
	if (ceDetailLogger) [[likely]]                                                   \
		ceDetailLogger->level(__VA_ARGS__);                                          \
} while (0)

/// @brief Expands to nothing - what a logging macro becomes in a configuration that removes its level.
#define CE_DETAIL_LOG_DISABLED(...) do { } while (0)

#endif //CE_DEFINE_LOGDETAIL_HPP
