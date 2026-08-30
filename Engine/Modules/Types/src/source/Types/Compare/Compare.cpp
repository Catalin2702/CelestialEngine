//
// Module: CelestialEngine/Engine/Modules/Types/Compare
// File: Compare.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#include "Types/Compare/Compare.hpp"

#include <glad/glad.h>


namespace CE::Types {

const char* ToString(const CompareFunc compareFunc) {
	switch (compareFunc) {
		case CompareFunc::Never: return "Never";
		case CompareFunc::Less: return "Less";
		case CompareFunc::Greater: return "Greater";
		case CompareFunc::Equal: return "Equal";
		case CompareFunc::NotEqual: return "NotEqual";
		case CompareFunc::LessEqual: return "LessEqual";
		case CompareFunc::GreaterEqual: return "GreaterEqual";
		case CompareFunc::Always: return "Always";
		default: return "Unknown";
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

}
