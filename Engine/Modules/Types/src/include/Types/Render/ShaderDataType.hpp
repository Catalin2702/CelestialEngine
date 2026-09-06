//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: ShaderDataType.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADERDATATYPE_HPP
#define CE_TYPES_RENDER_SHADERDATATYPE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum ShaderDataType
 * @brief The type of a single vertex attribute, as the shader sees it
 * @details Backend-neutral on purpose: every API names these differently (MTL::VertexFormat, GL_FLOAT plus a
 *			component count, VK_FORMAT_*), so the layout is described once here and each backend translates.
 */
enum class CE_TYPES_API ShaderDataType: u8 {
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

/**
 * @brief Size in bytes one attribute of this type occupies in a vertex buffer
 * @param type The attribute type
 * @return u32 The size in bytes, or 0 for ShaderDataType::None
 */
constexpr u32 ShaderDataTypeSize(const ShaderDataType type) {
	constexpr u32 FLOAT_SIZE = sizeof(f32);
	constexpr u32 INT_SIZE = sizeof(u32);
	constexpr u32 BOOL_SIZE = sizeof(bool);
	switch (type) {
		case ShaderDataType::Float: return FLOAT_SIZE;
		case ShaderDataType::Float2: return FLOAT_SIZE * 2;
		case ShaderDataType::Float3: return FLOAT_SIZE * 3;
		case ShaderDataType::Float4: return FLOAT_SIZE * 4;
		case ShaderDataType::Mat3: return FLOAT_SIZE * 3 * 3;
		case ShaderDataType::Mat4: return FLOAT_SIZE * 4 * 4;
		case ShaderDataType::Int: return INT_SIZE;
		case ShaderDataType::Int2: return INT_SIZE * 2;
		case ShaderDataType::Int3: return INT_SIZE * 3;
		case ShaderDataType::Int4: return INT_SIZE * 4;
		case ShaderDataType::Bool: return BOOL_SIZE;
		case ShaderDataType::None:
		default:
			return 0;
	}
}

/**
 * @brief How many scalar components the type is made of
 * @param type The attribute type
 * @return u32 The component count, or 0 for ShaderDataType::None
 */
constexpr u32 ShaderDataTypeComponentCount(const ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat3: return 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::None:
		default:
			return 0;
	}
}

/**
 * @brief Names a ShaderDataType, for fmt/spdlog and - through the formatter below - for std::format
 * @param shaderDataType The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const ShaderDataType shaderDataType) {
	switch (shaderDataType) {
		case ShaderDataType::None: return "None";
		case ShaderDataType::Float: return "Float";
		case ShaderDataType::Float2: return "Float2";
		case ShaderDataType::Float3: return "Float3";
		case ShaderDataType::Float4: return "Float4";
		case ShaderDataType::Mat3: return "Mat3";
		case ShaderDataType::Mat4: return "Mat4";
		case ShaderDataType::Int: return "Int";
		case ShaderDataType::Int2: return "Int2";
		case ShaderDataType::Int3: return "Int3";
		case ShaderDataType::Int4: return "Int4";
		case ShaderDataType::Bool: return "Bool";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::ShaderDataType>: std::formatter<std::string_view> {
	auto format(const CE::Types::ShaderDataType value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_SHADERDATATYPE_HPP
