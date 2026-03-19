//
// Module: CelestialEngine/Engine/Modules/Define
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#pragma once

#ifndef CE_DEFINE_RENDER_HPP
#define CE_DEFINE_RENDER_HPP

#define RENDER_API_TYPE(type) \
	static CE::Types::Render::GraphicsApi GetStaticType() { return CE::Types::Render::GraphicsApi::type; } \
	CE::Types::Render::GraphicsApi GetGraphicsApi() const override { return GetStaticType(); }

#endif //CE_DEFINE_RENDER_HPP
