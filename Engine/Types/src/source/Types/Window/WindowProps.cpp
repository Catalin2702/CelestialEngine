//
// Created by Catalin Chirosca on 2026-02-21.
//

#include "Types/Window/WindowProps.hpp"


namespace CE::Types::Window {

WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync) {}

}
