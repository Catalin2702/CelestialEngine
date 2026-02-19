//
// Created by Catalin Chirosca on 2026-02-19.
//

#include "Utils/Window.hpp"

#include <GLFW/glfw3.h>


namespace CE::Window {
void GLFWwindowDestructor::operator()(GLFWwindow *window) const {
	if (window)
		glfwDestroyWindow(window);
}
}
