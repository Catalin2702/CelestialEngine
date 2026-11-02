//
// Module: CelestialEngine/Engine/Modules/Define
// File: Log.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Log.hpp
 * @brief The client logging macros: CE_TRACE, CE_INFO, CE_WARN, CE_ERROR, CE_CRITICAL
 * @details They write to the client logger. The engine has its own, reached through the CE_CORE_* macros of
 *			Define/CoreLog.hpp, which this header pulls in only for engine code (CE_ENGINE_INTERNAL) - so the two
 *			voices stay distinguishable in the output.
 *
 *			What survives each configuration:
 *			| Level    | Debug | Release | Dist |
 *			|----------|-------|---------|------|
 *			| TRACE    | yes   | -       | -    |
 *			| INFO     | yes   | -       | -    |
 *			| WARN     | yes   | yes     | -    |
 *			| ERROR    | yes   | yes     | -    |
 *			| CRITICAL | yes   | yes     | yes  |
 *			A removed level compiles to nothing, arguments included. CRITICAL stays in Dist on purpose: it is what is
 *			left to read when a shipped build dies, and Tools::Log gives Dist a sink for exactly that.
 */

#pragma once

#ifndef CE_DEFINE_LOG_HPP
#define CE_DEFINE_LOG_HPP

#include "Define/Config.hpp"
#include "Define/LogDetail.hpp"

#if CE_DEBUG
	/// @brief Client trace-level message (Debug only)
	#define CE_TRACE(...) CE_DETAIL_LOG(GetClientLogger, trace, __VA_ARGS__)

	/// @brief Client info-level message (Debug only)
	#define CE_INFO(...) CE_DETAIL_LOG(GetClientLogger, info, __VA_ARGS__)
#else
	#define CE_TRACE(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
	#define CE_INFO(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
#endif

#if not CE_DIST
	/// @brief Client warning-level message (Debug and Release)
	#define CE_WARN(...) CE_DETAIL_LOG(GetClientLogger, warn, __VA_ARGS__)

	/// @brief Client error-level message (Debug and Release)
	#define CE_ERROR(...) CE_DETAIL_LOG(GetClientLogger, error, __VA_ARGS__)
#else
	#define CE_WARN(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
	#define CE_ERROR(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
#endif

/// @brief Client critical-level message, kept in every configuration - Dist included
#define CE_CRITICAL(...) CE_DETAIL_LOG(GetClientLogger, critical, __VA_ARGS__)

// The engine's own macros, for engine code only. See Define/CoreLog.hpp.
#if defined(CE_ENGINE_INTERNAL) and CE_ENGINE_INTERNAL
	#include "Define/CoreLog.hpp"
#endif

#endif //CE_DEFINE_LOG_HPP
