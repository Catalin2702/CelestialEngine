//
// Module: CelestialEngine/Engine/Modules/Utility/Config
// File: Config.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Utility/Config/Config.hpp"


namespace CE::Utility {

std::atomic<Config*> Config::_instance = nullptr;

Config& Config::StGet() {
	return *_instance;
}

const Types::WindowProps& Config::GetWindowProps() {
	return StGet()._windowProps;
}

void Config::SetWindowProps(Types::WindowProps windowProps) {
	if (_instance == nullptr) [[unlikely]] {
		_instance = new Config();
	}
	_instance.load()->_windowProps = std::move(windowProps);
}

}
