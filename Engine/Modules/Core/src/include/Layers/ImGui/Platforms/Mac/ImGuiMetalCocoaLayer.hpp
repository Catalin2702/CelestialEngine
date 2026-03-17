//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalCocoaLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALCOCOALAYER_HPP
#define CE_LAYERS_IMGUIMETALCOCOALAYER_HPP

#include "Layers/ImGui/I_ImGuiLayer.hpp"

#include <dispatch/dispatch.h>
#include <Foundation/Foundation.hpp>

namespace CA {
class MetalDrawable;
class MetalLayer;
}
namespace MTL {
class CommandBuffer;
class CommandQueue;
class Device;
class RenderCommandEncoder;
class RenderPassDescriptor;
}
namespace NS {
class AutoreleasePool;
}
namespace CE::Window {
class MetalCocoaWindow;
}


namespace CE::Layers {

class ImGuiMetalCocoaLayer final: public I_ImGuiLayer {
	struct MetalContext {
		Window::MetalCocoaWindow* window = nullptr;						///< Pointer to the Metal window
		MTL::Device* metalDevice = nullptr;								///< Pointer to the Metal device
		MTL::CommandQueue* commandQueue = nullptr;						///< Pointer to the Metal command queue
		CA::MetalLayer* metalLayer = nullptr;							///< Pointer to the Core Animation Metal layer
		MTL::RenderPassDescriptor* renderPassDescriptor = nullptr;		///< Pointer to the Metal render pass descriptor
	};

	struct MetalFrameContext {
		CA::MetalDrawable* drawable = nullptr;							///< Pointer to the Metal drawable
		MTL::CommandBuffer* commandBuffer = nullptr;					///< Pointer to the Metal command buffer
		MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;		///< Pointer to the Metal render command encoder
		NS::SharedPtr<NS::AutoreleasePool> autoreleasePool = nullptr;	///< Shared pointer to the autorelease pool for the frame
	};

public:
	ImGuiMetalCocoaLayer();

	~ImGuiMetalCocoaLayer() override;

public:
	void OnRender() const override;

	void OnEvent(Events::I_Event& event) override;

	void Begin() override;

	void End() override;

protected:
	void _Init() override;

	void _Shutdown() override;

	bool _OnMouseMoved(Events::MouseMovedEvent& event) const override;

	bool _OnMouseScrolled(Events::MouseScrolledEvent& event) const override;

	bool _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const override;

	bool _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const override;

	bool _OnKeyPressed(Events::KeyPressedEvent& event) const override;

	bool _OnKeyReleased(Events::KeyReleasedEvent& event) const override;

	bool _OnKeyTyped(Events::KeyTypedEvent& event) const override;

	bool _OnWindowResized(Events::WindowResizeEvent& event) const override;

private:
	MetalContext _metalContext;							///< Cached Metal context for rendering
	MetalFrameContext _frameContext;					///< Cached frame context for the current frame

	dispatch_semaphore_t _renderSemaphore = nullptr;	///< Semaphore to synchronize frame rendering with Metal
	const unsigned int _maxFramesInFlight = 3;			///< Maximum number of frames that can be in flight for rendering
};

}

#endif //CE_LAYERS_IMGUIMETALCOCOALAYER_HPP
