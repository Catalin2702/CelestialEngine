//
// Created by Catalin Chirosca on 2026-02-16.
//

#include <utility>

#include "Window/InterfaceWindow.hpp"


namespace CE::Window {

WindowProps::WindowProps(std::string title, const unsigned int width, const unsigned int height, const bool VSync):
title(std::move(title)), width(width), height(height), VSync(VSync) {}

}
