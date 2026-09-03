//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: BlendFactor.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Types/Render/BlendFactor.hpp"

#include <glad/glad.h>


namespace CE::Types {

bool IsValidDestinationFactor(const BlendFactor blendFactor) {
	return blendFactor != BlendFactor::SrcAlphaSaturated;
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

}
