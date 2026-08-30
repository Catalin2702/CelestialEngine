//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: CullMode.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#include "Types/Render/CullMode.hpp"

#include <glad/glad.h>

namespace CE::Types {

const char* ToString(const CullMode cullMode) {
	switch (cullMode) {
		case CullMode::None: return "None";
		case CullMode::Front: return "Front";
		case CullMode::Back: return "Back";
		default: return "Unknown";
	}
}

const char* ToString(const FrontFace frontFace) {
	switch (frontFace) {
		case FrontFace::CounterClockwise: return "CounterClockwise";
		case FrontFace::Clockwise: return "Clockwise";
		default: return "Unknown";
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

}
