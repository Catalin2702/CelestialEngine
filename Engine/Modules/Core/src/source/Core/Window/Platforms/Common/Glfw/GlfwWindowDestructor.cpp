//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindowDestructor.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include "Core/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"

#include <GLFW/glfw3.h>


namespace CE::Core {

void GlfwWindowDestructor::operator()(GLFWwindow *window) const {
	if (window) [[likely]]
		glfwDestroyWindow(window);
}

}
