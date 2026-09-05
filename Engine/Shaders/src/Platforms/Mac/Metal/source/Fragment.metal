//
// Module: CelestialEngine/Engine/Shaders/Platforms/Mac/Metal
// File: Fragment.metal
// Created by: Catalin Chirosca
// Created: 2026-05-15
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <metal_stdlib>
#include "Common.h"
using namespace metal;

fragment half4 fragmentMain(VertexPayload _fragment [[stage_in]]) {
	return half4(_fragment.color);
}
