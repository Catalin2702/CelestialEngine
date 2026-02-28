//
// Module: Types/Window
// File: WindowProps.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWPROPS_HPP
#define CE_TYPES_WINDOW_WINDOWPROPS_HPP

#include <functional>
#include <string>


namespace CE::Types::Window {

namespace Events {
class Event;
}

struct WindowProps {
	std::string title;
	unsigned int width;
	unsigned int height;
	bool VSync;

	WindowProps(const std::string& title, unsigned int width, unsigned int height, bool VSync);
};

template<class T>
using CallbackFn = std::function<void(T&)>;

template<class T>
struct WindowData: WindowProps {
	CallbackFn<T> eventCallback;

	WindowData(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync)
		: WindowProps(title, width, height, VSync) {}
};

}

#endif //CE_TYPES_WINDOW_WINDOWPROPS_HPP
