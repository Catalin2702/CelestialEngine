//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: I_Context.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_CONTEXT_CONTEXT_HPP
#define CE_RENDER_CONTEXT_CONTEXT_HPP

#include "Types/Render/Render.hpp"

#include <utility>

namespace CE::Render::Context {

class I_Context {
public:
	virtual ~I_Context() = default;

public:
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;

public:
	virtual void HandleContentScaleChange(const std::pair<float, float>& scale) = 0;

public:
	[[nodiscard]] virtual Types::Render::GraphicsApi GetGraphicsApi() const = 0;
};

}

#endif //CE_RENDER_CONTEXT_CONTEXT_HPP
