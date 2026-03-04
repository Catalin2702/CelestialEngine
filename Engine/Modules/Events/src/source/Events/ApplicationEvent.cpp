//
// Module: CelestialEngine/Engine/Modules/Events
// File: ApplicationEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-02-28
//

#include "Events/ApplicationEvent.hpp"

#include <sstream>


namespace CE::Events {

/**
 * @brief WindowResizeEvent constructor implementation
 * @param width New window width in pixels
 * @param height New window height in pixels
 */
WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height):
	_width(width), _height(height) {}

/**
 * @brief Converts the window resize event to a string representation
 * @return std::string String in format "WindowResizeEvent: width, height"
 */
std::string WindowResizeEvent::ToString() const {
	std::stringstream ss;
	ss << "WindowResizeEvent: " << _width << ", " << _height;
	return ss.str();
}

}
