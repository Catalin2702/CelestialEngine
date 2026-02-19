//
// Created by Catalin Chirosca on 2026-02-18.
//

#pragma once

#ifndef CE_WINDOW_UTILS_WINDOW_HPP
#define CE_WINDOW_UTILS_WINDOW_HPP

#include <memory>


struct GLFWwindow;

namespace CE::Window {

struct GLFWwindowDestructor {
	void operator()(GLFWwindow* window) const;
};

using GLFWWindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDestructor>;

}

#endif //CE_WINDOW_UTILS_WINDOW_HPP
