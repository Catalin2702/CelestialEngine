//
// Module: CelestialEngine/Engine/Modules/Define
// File: CoreLog.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file CoreLog.hpp
 * @brief The engine's own logging macros: CE_CORE_TRACE, CE_CORE_INFO, CE_CORE_WARN, CE_CORE_ERROR, CE_CORE_CRITICAL
 * @details They write to the core logger, which is the engine's voice - a client writing to it would make its messages
 *			indistinguishable from the engine's. So this header is for engine code only: every engine module and the
 *			tests link CE_Internal (CelestialEngineMacros.cmake), which defines CE_ENGINE_INTERNAL, and Define/Log.hpp
 *			includes this file only when it is set. Clients use the CE_* macros of Define/Log.hpp.
 *
 *			Public headers must not use these macros in inline or template code: that code is compiled in the client's
 *			translation units too, where they do not exist. Route such messages through a function defined in a .cpp
 *			instead (see Utility/Delegate/Dispatcher.hpp).
 *
 *			Configurations keep the same levels as the client macros - see the table in Define/Log.hpp.
 */

#pragma once

#ifndef CE_DEFINE_CORELOG_HPP
#define CE_DEFINE_CORELOG_HPP

#if not defined(CE_ENGINE_INTERNAL) or not CE_ENGINE_INTERNAL
	#error "CE_CORE_* logging is reserved to the engine. Use the client macros (CE_TRACE ... CE_CRITICAL) of Define/Log.hpp."
#endif

#include "Define/Config.hpp"
#include "Define/LogDetail.hpp"

#if CE_DEBUG
	/// @brief Engine trace-level message (Debug only)
	#define CE_CORE_TRACE(...) CE_DETAIL_LOG(GetCoreLogger, trace, __VA_ARGS__)

	/// @brief Engine info-level message (Debug only)
	#define CE_CORE_INFO(...) CE_DETAIL_LOG(GetCoreLogger, info, __VA_ARGS__)
#else
	#define CE_CORE_TRACE(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
	#define CE_CORE_INFO(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
#endif

#if not CE_DIST
	/// @brief Engine warning-level message (Debug and Release)
	#define CE_CORE_WARN(...) CE_DETAIL_LOG(GetCoreLogger, warn, __VA_ARGS__)

	/// @brief Engine error-level message (Debug and Release)
	#define CE_CORE_ERROR(...) CE_DETAIL_LOG(GetCoreLogger, error, __VA_ARGS__)
#else
	#define CE_CORE_WARN(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
	#define CE_CORE_ERROR(...) CE_DETAIL_LOG_DISABLED(__VA_ARGS__)
#endif

/// @brief Engine critical-level message, kept in every configuration - Dist included
#define CE_CORE_CRITICAL(...) CE_DETAIL_LOG(GetCoreLogger, critical, __VA_ARGS__)

#endif //CE_DEFINE_CORELOG_HPP
