//
// Module: Types/Window
// File: WindowProps.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#include "Types/Window/WindowProps.hpp"


namespace CE::Types::Window {

WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync) {}

}
