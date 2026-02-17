//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/ApplicationEvent.hpp"

#include <sstream>


namespace CE::Events {

WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height):
	_width(width), _height(height) {}

std::string WindowResizeEvent::ToString() const {
	std::stringstream ss;
	ss << "WindowResizeEvent: " << _width << ", " << _height;
	return ss.str();
}

}
