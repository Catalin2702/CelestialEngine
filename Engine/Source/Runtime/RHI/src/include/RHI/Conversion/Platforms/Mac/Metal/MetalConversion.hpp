//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Conversion/Platforms/Mac/Metal
// File: MetalConversion.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_RHI_CONVERSION_METALCONVERSION_HPP
#define CE_RHI_CONVERSION_METALCONVERSION_HPP

#include "Define/DynamicLinker.hpp"
#include "Metal/MTLPixelFormat.hpp"
#include "Types/Render/AttachmentAction.hpp"
#include "Types/Render/BlendFactor.hpp"
#include "Types/Render/Compare.hpp"
#include "Types/Render/CullMode.hpp"
#include "Types/Render/PixelFormat.hpp"
#include "Types/Render/PrimitiveTopology.hpp"
#include "Types/Render/ShaderConstants.hpp"
#include "Types/Render/ShaderDataType.hpp"
#include "Types/Render/TextureUsage.hpp"
#include "Types/Var/Vars.hpp"

namespace NS {
using UInteger = std::uintptr_t;
}

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
using TextureUsage = NS::UInteger;
enum VertexFormat: std::uintptr_t;
}

// Temporarily in Core: these tables belong to the RHI backends, and move with them when the RHI becomes a module of
// its own. They were in Types, which must not know any graphics API.
namespace CE::RHI {

/// Vertex-stage buffer slot the geometry is bound to. Metal gives the vertex stage 31 buffer slots and lets the
/// shader pick; slot 0 is the geometry by convention here, so uniforms and argument buffers start at 1. The pipeline's
/// MTLVertexDescriptor and the encoder's setVertexBuffer must agree on it, hence the shared constant.
inline constexpr u32 METAL_VERTEX_BUFFER_INDEX = 0;

/**
 * @brief Converts an engine pixel format into the Metal one
 * @details Returns MTL::PixelFormatInvalid for None, which is what the pipeline validation reports on.
 */
[[nodiscard]] CE_RHI_API MTL::PixelFormat ToMetal(Types::PixelFormat pixelFormat);

/**
 * @brief Converts a Metal pixel format into the engine one
 * @details Returns Types::PixelFormat None for MTL::pixelFormatInvalid
 */
[[nodiscard]] CE_RHI_API Types::PixelFormat FromMetal(MTL::PixelFormat pixelFormat);

/**
 * @brief Converts a depth/stencil comparison into the Metal one
 */
[[nodiscard]] CE_RHI_API MTL::CompareFunction ToMetal(Types::CompareFunc compareFunc);

/**
 * @brief Converts a blend factor into the Metal one
 * @details Metal calls the blend constant "BlendColor"/"BlendAlpha" where the engine (and OpenGL) call it Constant*.
 */
[[nodiscard]] CE_RHI_API MTL::BlendFactor ToMetal(Types::BlendFactor blendFactor);

/**
 * @brief Converts a cull mode into the Metal one
 */
[[nodiscard]] CE_RHI_API MTL::CullMode ToMetal(Types::CullMode cullMode);

/**
 * @brief Converts a winding order into the Metal one
 */
[[nodiscard]] CE_RHI_API MTL::Winding ToMetal(Types::FrontFace frontFace);

/**
 * @brief Converts a load action into the Metal one
 */
[[nodiscard]] CE_RHI_API MTL::LoadAction ToMetal(Types::LoadAction loadAction);

/**
 * @brief Converts a store action into the Metal one
 */
[[nodiscard]] CE_RHI_API MTL::StoreAction ToMetal(Types::StoreAction storeAction);

/**
 * @brief Converts a topology into the primitive type the draw call takes
 * @details Metal splits what OpenGL keeps in one value: the *type* is a draw-call argument, the *class* below is
 *			pipeline state. Both come from the same PrimitiveTopology, which is why there are two functions and not
 *			one overload set.
 */
[[nodiscard]] CE_RHI_API MTL::PrimitiveType ToMetalPrimitiveType(Types::PrimitiveTopology primitiveTopology);

/**
 * @brief Converts a topology into the primitive class the pipeline descriptor takes
 */
[[nodiscard]] CE_RHI_API MTL::PrimitiveTopologyClass ToMetalTopologyClass(Types::PrimitiveTopology primitiveTopology);

/**
 * @brief Converts a vertex attribute type into the Metal vertex format
 * @details Returns MTL::VertexFormatInvalid for the matrix types: a mat4 attribute is four Float4 attributes in
 *			consecutive slots in every API, and expanding it belongs to the vertex-descriptor builder, not here.
 */
[[nodiscard]] CE_RHI_API MTL::VertexFormat ToMetal(Types::ShaderDataType shaderDataType);

[[nodiscard]] CE_RHI_API MTL::TextureUsage ToMetal(Types::TextureUsage textureUsage);

static_assert(Types::CAMERA_CONSTANTS_SLOT != METAL_VERTEX_BUFFER_INDEX and Types::OBJECT_CONSTANTS_SLOT != METAL_VERTEX_BUFFER_INDEX,
	"A constant slot collides with the vertex buffer!");

}

#endif //CE_RHI_CONVERSION_METALCONVERSION_HPP
