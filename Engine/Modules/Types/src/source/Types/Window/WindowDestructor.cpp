//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowDestructor.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-02-28
//

#include "Types/Window/WindowDestructor.hpp"

#include <GLFW/glfw3.h>


namespace CE::Types::Window {

/**
 * @brief Destroys the GLFW window
 * @param window Pointer to the GLFW window to destroy
 * @details Calls glfwDestroyWindow to properly clean up the window resources
 */
void GLFWwindowDestructor::operator()(GLFWwindow *window) const {
	if (window)
		glfwDestroyWindow(window);
}

}
