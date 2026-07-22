//
// Module: CelestialEngine/Engine/Modules/Define
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_DEFINE_WINDOW_HPP
#define CE_DEFINE_WINDOW_HPP

/**
 * @brief Injects the window API identification boilerplate into a window class
 * @param type The WindowApi enum value the class belongs to (e.g. GLFW, Cocoa)
 * @details Defines the static GetStaticType() and the virtual GetWindowApi() override so instances can be
 *			identified both at compile time and through a base-class pointer.
 */
#define WINDOW_API_TYPE(type) \
	static CE::Types::WindowApi GetStaticType() { return CE::Types::WindowApi::type; } \
	CE::Types::WindowApi GetWindowApi() const override { return GetStaticType(); }


#endif //CE_DEFINE_WINDOW_HPP
