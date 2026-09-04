//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline/Platforms/Mac/Metal
// File: MetalPipelineState.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_METALPIPELINESTATE_HPP
#define CE_CORE_RENDER_PIPELINE_METALPIPELINESTATE_HPP

#include "Core/Render/Pipeline/I_PipelineState.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>


namespace MTL {
	class Device;
	class DepthStencilState;
	class RenderPipelineState;
}

namespace CE::Core {

/**
 * @class MetalPipelineState
 * @brief An MTL::RenderPipelineState and the depth state that goes with it, compiled once at creation
 * @details The real thing the abstraction was designed around: Metal compiles the shaders, the vertex layout, the
 *			blend equation and the attachment formats into one immutable object, and validates the lot up front. That
 *			is why PipelineDescriptor carries RenderTargetFormats at all - a pipeline is only valid inside a render
 *			pass whose attachments match what it was built against.
 *
 *			It holds *two* Metal objects, because Metal splits the state three ways rather than one:
 *			- the pipeline object: shaders, vertex descriptor, blending, attachment formats;
 *			- MTLDepthStencilState: the depth test and depth write, a separate object with its own descriptor;
 *			- encoder state: cull mode and winding, which are setters on the render command encoder.
 *			The first two are built here; the third is read back out of GetDescriptor() by MetalCommandEncoder. One
 *			engine-side PipelineDescriptor, three Metal destinations - which is exactly the kind of seam a descriptor
 *			exists to hide.
 *
 *			There is no Apply(): a Metal pipeline is handed to an encoder, not applied to a context.
 */
class CE_CORE_API MetalPipelineState final: public I_PipelineStateBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Compiles the descriptor into a Metal render pipeline state and a depth/stencil state
	 * @param nativeDevice The device to compile on
	 * @param descriptor The state to bake; its shader modules must be MetalShaderModule instances
	 * @details Throws std::runtime_error when a stage is missing, is not a Metal module, carries the wrong stage,
	 *			when a destination blend factor is one no API accepts, when an attribute type has no single vertex
	 *			format, or when Metal rejects the pipeline - its error text is logged before rethrowing, and it is
	 *			worth reading: it names the mismatched attachment format or vertex attribute outright.
	 */
	MetalPipelineState(MTL::Device* nativeDevice, PipelineDescriptor descriptor);

	MetalPipelineState(const MetalPipelineState&) = delete;
	MetalPipelineState(MetalPipelineState&&) noexcept = default;

public:
	MetalPipelineState& operator = (const MetalPipelineState&) = delete;
	MetalPipelineState& operator = (MetalPipelineState&&) = default;

public:
	[[nodiscard]] const PipelineDescriptor& GetDescriptor() const override { return _descriptor; }

	/**
	 * @brief Gets the compiled pipeline object, for MetalCommandEncoder to set
	 */
	[[nodiscard]] MTL::RenderPipelineState* GetPipelineState() const { return _nativePipelineState.get(); }

	/**
	 * @brief Gets the depth/stencil state, for MetalCommandEncoder to set alongside the pipeline
	 * @details Never null: a pipeline that does no depth testing still gets a state object, with the comparison set
	 *			to Always and writing disabled. Leaving it unset would inherit whatever the previous draw used.
	 */
	[[nodiscard]] MTL::DepthStencilState* GetDepthStencilState() const { return _nativeDepthStencilState.get(); }

private:
	PipelineDescriptor _descriptor;

	NS::SharedPtr<MTL::RenderPipelineState> _nativePipelineState;
	NS::SharedPtr<MTL::DepthStencilState> _nativeDepthStencilState;
};

}

#endif //CE_CORE_RENDER_PIPELINE_METALPIPELINESTATE_HPP
