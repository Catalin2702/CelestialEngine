//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: CullMode.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include "Types/Render/CullMode.hpp"

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

}
