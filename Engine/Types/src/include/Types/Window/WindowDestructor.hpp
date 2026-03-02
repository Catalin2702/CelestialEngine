//
// Module: Types/Window
// File: WindowDestructor.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP
#define CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP

#include <memory>


struct GLFWwindow;

/**
 * @namespace CE::Types::Window
 * @brief Types and utilities related to window management
 * @details Contains type definitions and helper structures for managing windows,
 *			including custom deleters for smart pointers to ensure proper resource cleanup.
 */
namespace CE::Types::Window {

/**
 * @struct GLFWwindowDestructor
 * @brief Custom deleter for GLFW window smart pointers
 * @details Provides proper cleanup for GLFW windows when used with std::unique_ptr.
 *			Ensures that glfwDestroyWindow is called when the window goes out of scope.
 */
struct GLFWwindowDestructor {
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
using GLFWwindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDestructor>;

}

#endif //CE_TYPES_WINDOW_WINDOWDESTRUCTOR_HPP
