//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Mac/Metal
// File: Metal.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_METAL_HPP
#define CE_TYPES_RENDER_METAL_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace MTL {
	enum PixelFormat: std::uintptr_t;
	enum CompareFunction: std::uintptr_t;
	enum BlendFactor: std::uintptr_t;
	enum CullMode: std::uintptr_t;
	enum Winding: std::uintptr_t;
	enum LoadAction: std::uintptr_t;
	enum StoreAction: std::uintptr_t;
	enum PrimitiveType: std::uintptr_t;
	enum PrimitiveTopologyClass: std::uintptr_t;
	enum VertexFormat: std::uintptr_t;
}

namespace CE::Types {

/// Vertex-stage buffer slot the geometry is bound to. Metal gives the vertex stage 31 buffer slots and lets the
/// shader pick; slot 0 is the geometry by convention here, so uniforms and argument buffers start at 1. The pipeline's
/// MTLVertexDescriptor and the encoder's setVertexBuffer must agree on it, hence the shared constant.
inline constexpr u32 METAL_VERTEX_BUFFER_INDEX = 0;

/**
 * @brief Converts an engine pixel format into the Metal one
 * @details Returns MTL::PixelFormatInvalid for None, which is what the pipeline validation reports on.
 */
[[nodiscard]] CE_TYPES_API MTL::PixelFormat ToMetal(PixelFormat pixelFormat);

/**
 * @brief Converts a depth/stencil comparison into the Metal one
 */
[[nodiscard]] CE_TYPES_API MTL::CompareFunction ToMetal(CompareFunc compareFunc);

/**
 * @brief Converts a blend factor into the Metal one
 * @details Metal calls the blend constant "BlendColor"/"BlendAlpha" where the engine (and OpenGL) call it Constant*.
 */
[[nodiscard]] CE_TYPES_API MTL::BlendFactor ToMetal(BlendFactor blendFactor);

/**
 * @brief Converts a cull mode into the Metal one
 */
[[nodiscard]] CE_TYPES_API MTL::CullMode ToMetal(CullMode cullMode);

/**
 * @brief Converts a winding order into the Metal one
 */
[[nodiscard]] CE_TYPES_API MTL::Winding ToMetal(FrontFace frontFace);

/**
 * @brief Converts a load action into the Metal one
 */
[[nodiscard]] CE_TYPES_API MTL::LoadAction ToMetal(LoadAction loadAction);

/**
 * @brief Converts a store action into the Metal one
 */
[[nodiscard]] CE_TYPES_API MTL::StoreAction ToMetal(StoreAction storeAction);

/**
 * @brief Converts a topology into the primitive type the draw call takes
 * @details Metal splits what OpenGL keeps in one value: the *type* is a draw-call argument, the *class* below is
 *			pipeline state. Both come from the same PrimitiveTopology, which is why there are two functions and not
 *			one overload set.
 */
[[nodiscard]] CE_TYPES_API MTL::PrimitiveType ToMetalPrimitiveType(PrimitiveTopology primitiveTopology);

/**
 * @brief Converts a topology into the primitive class the pipeline descriptor takes
 */
[[nodiscard]] CE_TYPES_API MTL::PrimitiveTopologyClass ToMetalTopologyClass(PrimitiveTopology primitiveTopology);

/**
 * @brief Converts a vertex attribute type into the Metal vertex format
 * @details Returns MTL::VertexFormatInvalid for the matrix types: a mat4 attribute is four Float4 attributes in
 *			consecutive slots in every API, and expanding it belongs to the vertex-descriptor builder, not here.
 */
[[nodiscard]] CE_TYPES_API MTL::VertexFormat ToMetal(ShaderDataType shaderDataType);

}

#endif //CE_TYPES_RENDER_METAL_HPP
