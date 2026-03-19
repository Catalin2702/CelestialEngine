//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#pragma once

#ifndef CE_RENDER_CONTEXT_METALCONTEXT_HPP
#define CE_RENDER_CONTEXT_METALCONTEXT_HPP

#include "Render/Context/I_Context.hpp"

#include "Define/Render.hpp"

namespace NS {
class Window;
}

namespace CE::Render::Context {

class MetalContext final: public I_Context {
public:
	MetalContext(NS::Window* window);
	~MetalContext() override = default;

public:
	void Init() override;
	void SwapBuffers() override;

RENDER_API_TYPE(Metal)

private:
	NS::Window* _window;							///< Pointer to the Cocoa window associated with this Metal context
};

}

#endif //CE_RENDER_CONTEXT_METALCONTEXT_HPP
