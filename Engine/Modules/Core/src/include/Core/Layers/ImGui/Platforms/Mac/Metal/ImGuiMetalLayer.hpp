//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Apple/MetalCpp/Foundation/Foundation.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Define/DynamicLinker.hpp"

#include <functional>
#include <optional>
#include <semaphore>

namespace CA {

class MetalDrawable;
class MetalLayer;

}

namespace MTL {
class CommandBuffer;
class RenderCommandEncoder;

}

namespace CE::Core::Layers {


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

	void Begin(float deltaTime) override;

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
	std::optional<std::reference_wrapper<Render::Context::MetalContext>> _context;	///< Cached Metal context for rendering
	std::optional<std::reference_wrapper<Window::CocoaWindow>> _window;	///< Cached Cocoa window for event handling and context access
	MetalFrameContext _frameContext;				///< Cached frame context for the current frame

	std::counting_semaphore<3> _renderSemaphore{3};		///< Semaphore to synchronize frame rendering with Metal
};

}

#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
