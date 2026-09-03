//
// Module: CelestialEngine/Engine/Modules/Types/Compare
// File: Compare.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Types/Compare/Compare.hpp"

#include <glad/glad.h>


namespace CE::Types {

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
