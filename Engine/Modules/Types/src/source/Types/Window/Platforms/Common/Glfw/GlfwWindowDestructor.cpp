//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowDestructor.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"

#include <GLFW/glfw3.h>

namespace CE::Types {

void GlfwWindowDestructor::operator()(GLFWwindow *window) const {
	if (window)
		glfwDestroyWindow(window);
}

}
