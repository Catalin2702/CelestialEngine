//
// Module: CelestialEngine/Engine/Modules/Utility/Config
// File: Config.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_UTILITY_CONFIG_CONFIG_HPP
#define CE_UTILITY_CONFIG_CONFIG_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <atomic>

namespace CE::Utility {

/**
 * @class Config
 * @brief Global engine configuration store
 * @details Static-access singleton holding the runtime configuration (currently the window properties parsed from
 *			the command line). Not instantiable by clients: everything is reached through the St* static methods.
 */
class CE_API Config {
	/**
	 * @brief Private constructor
	 * @details Only StGet() can create the singleton instance
	 */
	Config() = default;

public:
	Config(const Config&) = delete;
	Config(Config&&) noexcept = delete;
	~Config() = delete;

public:
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) noexcept = delete;

public:
	/**
	 * @brief Gets the singleton configuration instance, creating it on first use
	 * @return Config& Reference to the global configuration
	 */
	static Config& StGet();

	/**
	 * @brief Gets the current window properties
	 * @return const Types::WindowProps& The stored window configuration (size, APIs, VSync, ...)
	 */
	static const Types::WindowProps& StGetWindowProps();

public:
	/**
	 * @brief Replaces the stored window properties
	 * @param windowProps The new window configuration
	 */
	static void StSetWindowProps(Types::WindowProps windowProps);

private:
	static std::atomic<Config*> _stInstance;		///< Singleton instance (atomic for thread-safe first access)

	Types::WindowProps _windowProps;				///< Stored window configuration
};

}

#endif //CE_UTILITY_CONFIG_CONFIG_HPP
