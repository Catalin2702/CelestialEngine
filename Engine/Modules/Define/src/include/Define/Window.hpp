//
// Module: CelestialEngine/Engine/Modules/Define
// File: Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#pragma once

#ifndef CE_DEFINE_WINDOW_HPP
#define CE_DEFINE_WINDOW_HPP

#define WINDOW_API_TYPE(type) \
	static CE::Types::Window::WindowApi GetStaticType() { return CE::Types::Window::WindowApi::type; } \
	CE::Types::Window::WindowApi GetWindowApi() const override { return GetStaticType(); }


#endif //CE_DEFINE_WINDOW_HPP
