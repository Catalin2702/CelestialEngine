//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: BlendFactor.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include "Types/Render/BlendFactor.hpp"

namespace CE::Types {

const char* ToString(const BlendFactor blendFactor) {
	switch (blendFactor) {
		case BlendFactor::Zero: return "Zero";
		case BlendFactor::One: return "One";
		case BlendFactor::SrcColor: return "SrcColor";
		case BlendFactor::OneMinusSrcColor: return "OneMinusSrcColor";
		case BlendFactor::DstColor: return "DstColor";
		case BlendFactor::OneMinusDstColor: return "OneMinusDstColor";
		case BlendFactor::SrcAlpha: return "SrcAlpha";
		case BlendFactor::OneMinusSrcAlpha: return "OneMinusSrcAlpha";
		case BlendFactor::DstAlpha: return "DstAlpha";
		case BlendFactor::OneMinusDstAlpha: return "OneMinusDstAlpha";
		case BlendFactor::ConstantColor: return "ConstantColor";
		case BlendFactor::OneMinusConstantColor: return "OneMinusConstantColor";
		case BlendFactor::ConstantAlpha: return "ConstantAlpha";
		case BlendFactor::OneMinusConstantAlpha: return "OneMinusConstantAlpha";
		case BlendFactor::SrcAlphaSaturated: return "SrcAlphaSaturated";
		default: return "Unknown";
	}
}

bool IsValidDestinationFactor(const BlendFactor blendFactor) {
	return blendFactor != BlendFactor::SrcAlphaSaturated;
}

}
