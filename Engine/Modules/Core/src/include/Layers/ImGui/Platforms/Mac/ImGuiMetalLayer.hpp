//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-23
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/ImGui/I_ImGuiLayer.hpp"

#include "Define/DynamicLinker.hpp"
#include "Render/Context/Platforms/Mac/MetalContext.hpp"
#include "Window/Platforms/Mac/CocoaWindow.hpp"

#include <semaphore>
#include <Foundation/Foundation.hpp>

namespace CA {

class MetalDrawable;
class MetalLayer;

}

namespace MTL {
class CommandBuffer;
class RenderCommandEncoder;

}

namespace CE::Layers {


class CE_API ImGuiMetalLayer final: public I_ImGuiLayer {

	struct MetalFrameContext {
		NS::SharedPtr<CA::MetalDrawable> drawable = nullptr;			///< Pointer to the Metal drawable
		NS::SharedPtr<MTL::CommandBuffer> commandBuffer = nullptr;		//< Pointer to the Metal command buffer
		MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;		///< Pointer to the Metal render command encoder
	};

public:
	ImGuiMetalLayer();

	~ImGuiMetalLayer() override;

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
	Render::Context::MetalContext* _metalContext = nullptr;	///< Cached Metal context for rendering
	Window::CocoaWindow* _cocoaWindow = nullptr;	///< Cached Cocoa window for event handling and context access
	MetalFrameContext _frameContext;				///< Cached frame context for the current frame

	std::counting_semaphore<3> _renderSemaphore{3};		///< Semaphore to synchronize frame rendering with Metal
};

}

#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
