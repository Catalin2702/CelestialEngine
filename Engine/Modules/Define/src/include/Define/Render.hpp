//
// Module: CelestialEngine/Engine/Modules/Define
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_DEFINE_RENDER_HPP
#define CE_DEFINE_RENDER_HPP

/**
 * @brief Injects the graphics API identification boilerplate into a render class
 * @param type The GraphicsApi enum value the class belongs to (e.g. OpenGL, Metal)
 * @details Defines the static GetStaticType() and the virtual GetGraphicsApi() override so instances can be
 *			identified both at compile time and through a base-class pointer.
 */
#define RENDER_API_TYPE(type) \
	static CE::Types::GraphicsApi GetStaticType() { return CE::Types::GraphicsApi::type; } \
	CE::Types::GraphicsApi GetGraphicsApi() const override { return GetStaticType(); }

#endif //CE_DEFINE_RENDER_HPP
