//
// Module: CelestialEngine/Engine/Modules/Utility/Config
// File: Config.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Utility/Config/Config.hpp"

namespace CE::Utility::Config {

std::atomic<Config*> Config::_stInstance = nullptr;

Config& Config::StGet() {
	return *_stInstance;
}

const Types::Window::WindowProps& Config::StGetWindowProps() {
	return StGet()._windowProps;
}

void Config::StSetWindowProps(Types::Window::WindowProps windowProps) {
	if (_stInstance == nullptr) {
		_stInstance = new Config();
	}
	_stInstance.load()->_windowProps = std::move(windowProps);
}

}
