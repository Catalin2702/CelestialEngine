//
// Created by Catalin Chirosca on 2026-02-21.
//

#include "Types/Window/WindowDestructor.hpp"

#include <GLFW/glfw3.h>


namespace CE::Types::Window {

void GLFWwindowDestructor::operator()(GLFWwindow *window) const {
	if (window)
		glfwDestroyWindow(window);
}

}
