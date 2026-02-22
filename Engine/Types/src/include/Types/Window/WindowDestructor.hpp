//
// Created by Catalin Chirosca on 2026-02-21.
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP
#define CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP

#include <memory>


struct GLFWwindow;

namespace CE::Types::Window {

struct GLFWwindowDestructor {
	void operator()(GLFWwindow* window) const;
};

using GLFWwindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDestructor>;

}

#endif //CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP
