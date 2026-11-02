//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: BlendFactor.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/Render/BlendFactor.hpp"


namespace CE::Types {

bool IsValidDestinationFactor(const BlendFactor blendFactor) {
	return blendFactor != BlendFactor::SrcAlphaSaturated;
}

}
