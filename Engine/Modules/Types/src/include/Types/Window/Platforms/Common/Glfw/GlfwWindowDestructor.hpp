//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowDestructor.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_TYPES_WINDOW_PLATFORMS_COMMON_GLFW_GLFWWINDOWDESTRUCTOR_HPP
#define CE_TYPES_WINDOW_PLATFORMS_COMMON_GLFW_GLFWWINDOWDESTRUCTOR_HPP

#include "Define/DynamicLinker.hpp"

#include <memory>

struct GLFWwindow;

namespace CE::Types {

/**
 * @struct GlfwWindowDestructor
 * @brief Custom deleter for GLFW window smart pointers
 * @details Provides proper cleanup for GLFW windows when used with std::unique_ptr.
 *			Ensures that glfwDestroyWindow is called when the window goes out of scope.
 */
struct CE_API GlfwWindowDestructor {
	/**
	 * @brief Destroys the GLFW window
	 * @param window Pointer to the GLFW window to destroy
	 * @details Calls glfwDestroyWindow to properly clean up the window resources
	 */
	void operator()(GLFWwindow* window) const;
};

/**
 * @typedef GLFWwindowPtr
 * @brief Smart pointer type for GLFW windows with automatic cleanup
 * @details Uses std::unique_ptr with a custom deleter to manage GLFW window lifetime.
 *			Automatically calls glfwDestroyWindow when the pointer goes out of scope.
 */
using GlfwWindowPtr = std::unique_ptr<GLFWwindow, GlfwWindowDestructor>;

}

#endif //CE_TYPES_WINDOW_PLATFORMS_COMMON_GLFW_GLFWWINDOWDESTRUCTOR_HPP
