//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/ApplicationEvent.hpp"

#include <sstream>

CE::Events::WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height):
	_width(width), _height(height) {}

std::string CE::Events::WindowResizeEvent::ToString() const {
	std::stringstream ss;
	ss << "WindowResizeEvent: " << _width << ", " << _height;
	return ss.str();
}
