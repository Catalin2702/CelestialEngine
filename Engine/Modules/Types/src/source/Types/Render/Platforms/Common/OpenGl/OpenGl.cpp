//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Common/OpenGl
// File: OpenGl.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"

#include <glad/glad.h>


static_assert(std::is_same_v<GLint, i32>, "Glad changed the type of GLint; the declaration of GlFormat attributes must follow!");
static_assert(std::is_same_v<GLenum, u32>, "Glad changed the type of GLenum; the declaration of GlFormat attributes must follow!");

namespace CE::Types {

int ToInt(BufferBit bit) {
	return static_cast<int>(bit);
}

GlFormat ToOpenGl(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::RGBA8Unorm: return {.internalFormat = GL_RGBA8, .format = GL_RGBA, .type = GL_UNSIGNED_BYTE};
		case PixelFormat::RGBA8UnormSrgb: return {.internalFormat = GL_SRGB8_ALPHA8, .format = GL_RGBA, .type = GL_UNSIGNED_BYTE};

			// GL has no BGRA internal format: the swizzle is a transfer-time thing, and the storage is RGBA8 either way.
		case PixelFormat::BGRA8Unorm: return {.internalFormat = GL_RGBA8, .format = GL_BGRA, .type = GL_UNSIGNED_BYTE};
		case PixelFormat::BGRA8UnormSrgb: return {.internalFormat = GL_SRGB8_ALPHA8, .format = GL_BGRA, .type = GL_UNSIGNED_BYTE};

		case PixelFormat::RGBA16Float: return {.internalFormat = GL_RGBA16F, .format = GL_RGBA, .type = GL_HALF_FLOAT};
		case PixelFormat::Depth32Float: return {.internalFormat = GL_DEPTH_COMPONENT32F, .format = GL_DEPTH_COMPONENT, .type = GL_FLOAT};

		default: return {.internalFormat = 0, .format = 0, .type = 0};
	}
}

u32 ToOpenGl(const BlendFactor blendFactor) {
	switch (blendFactor) {
		default:
		case BlendFactor::Zero: return GL_ZERO;
		case BlendFactor::One: return GL_ONE;
		case BlendFactor::SrcColor: return GL_SRC_COLOR;
		case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
		case BlendFactor::DstColor: return GL_DST_COLOR;
		case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
		case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
		case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::DstAlpha: return GL_DST_ALPHA;
		case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
		case BlendFactor::ConstantColor: return GL_CONSTANT_COLOR;
		case BlendFactor::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
		case BlendFactor::ConstantAlpha: return GL_CONSTANT_ALPHA;
		case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
		case BlendFactor::SrcAlphaSaturated: return GL_SRC_ALPHA_SATURATE;
	}
}

u32 ToOpenGl(const CompareFunc compareFunc) {
	switch (compareFunc) {
		default:
		case CompareFunc::Never: return GL_NEVER;
		case CompareFunc::Less: return GL_LESS;
		case CompareFunc::Greater: return GL_GREATER;
		case CompareFunc::Equal: return GL_EQUAL;
		case CompareFunc::NotEqual: return GL_NOTEQUAL;
		case CompareFunc::LessEqual: return GL_LEQUAL;
		case CompareFunc::GreaterEqual: return GL_GEQUAL;
		case CompareFunc::Always: return GL_ALWAYS;
	}
}

u32 ToOpenGl(const CullMode cullMode) {
	switch (cullMode) {
		case CullMode::Front: return GL_FRONT;
		// None names no face: the caller disables culling instead of passing this to glCullFace.
		case CullMode::None:
		case CullMode::Back:
		default:
			return GL_BACK;
	}
}

u32 ToOpenGl(const FrontFace frontFace) {
	switch (frontFace) {
		case FrontFace::Clockwise: return GL_CW;
		case FrontFace::CounterClockwise:
		default:
			return GL_CCW;
	}
}

u32 ToOpenGl(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::PointList: return GL_POINTS;
		case PrimitiveTopology::LineList: return GL_LINES;
		case PrimitiveTopology::LineStrip: return GL_LINE_STRIP;
		case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
		case PrimitiveTopology::TriangleList:
		default:
			return GL_TRIANGLES;
	}
}

int ToOpenGlShaderType(const ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		default:
			return -1; // Invalid shader type
	}
}

}
