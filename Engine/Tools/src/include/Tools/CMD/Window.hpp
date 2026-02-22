//
// Created by Catalin Chirosca on 2026-02-21.
//

#pragma once

#ifndef CE_TOOLS_CMD_WINDOW_HPP
#define CE_TOOLS_CMD_WINDOW_HPP

#include "Types/Window/WindowProps.hpp"

namespace CE::Tools::CMD {

Types::Window::WindowProps GetWindowProps(int argc, char* argv[]);

}

#endif //CE_TOOLS_CMD_WINDOW_HPP
