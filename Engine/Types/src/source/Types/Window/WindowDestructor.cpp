//
// Module: Types/Window
// File: WindowDestructor.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#include "Types/Window/WindowDestructor.hpp"

#include <GLFW/glfw3.h>


namespace CE::Types::Window {

void GLFWwindowDestructor::operator()(GLFWwindow *window) const {
	if (window)
		glfwDestroyWindow(window);
}

}
