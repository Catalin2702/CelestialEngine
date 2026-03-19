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

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>

namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
}
namespace NS {
class Window;
}

namespace CE::Render::Context {

struct MetalContextProps {
	NS::Window* window = nullptr;
	MTL::PixelFormat pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
};

class MetalContext final: public I_Context {
public:
	MetalContext(const MetalContextProps& props);
	~MetalContext() override;

public:
	void Init() override;
	void SwapBuffers() override;

RENDER_API_TYPE(Metal)

private:
	NS::SharedPtr<CA::MetalLayer> _layer = nullptr;				///< Core Animation Metal layer for rendering
	NS::SharedPtr<MTL::Device> _device = nullptr;				///< Metal device (GPU) for resource creation and rendering
	NS::SharedPtr<MTL::CommandQueue> _commandQueue = nullptr;	///< Metal command queue for issuing rendering commands

	MetalContextProps _props;							///< Properties for initializing the Metal context, including window and pixel format
};

}

#endif //CE_RENDER_CONTEXT_METALCONTEXT_HPP
