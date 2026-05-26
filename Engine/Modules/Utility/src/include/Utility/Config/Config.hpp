//
// Module: CelestialEngine/Engine/Modules/Utility/Config
// File: Config.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_UTILITY_CONFIG_CONFIG_HPP
#define CE_UTILITY_CONFIG_CONFIG_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <atomic>

namespace CE::Utility::Config {

class CE_API Config {
	Config() = default;

public:
	Config(const Config&) = delete;
	Config(Config&&) noexcept = delete;
	~Config() = delete;

public:
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) noexcept = delete;

public:
	static Config& StGet();

	static const Types::Window::WindowProps& StGetWindowProps();

public:
	static void StSetWindowProps(Types::Window::WindowProps windowProps);

private:
	static std::atomic<Config*> _stInstance;

	Types::Window::WindowProps _windowProps;
};

}

#endif //CE_UTILITY_CONFIG_CONFIG_HPP
