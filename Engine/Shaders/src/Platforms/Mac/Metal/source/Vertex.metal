//
// Module: CelestialEngine/Engine/Shaders/Platforms/Mac/Metal
// File: Vertex.metal
// Created by: Catalin Chirosca
// Created: 2026-05-15
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <metal_stdlib>
#include "Common.h"
using namespace metal;

// Reads the vertex buffer the engine uploaded, instead of the constant triangle this used to hold: the geometry now
// comes from I_VertexBuffer, and [[stage_in]] is what makes the vertex descriptor in the pipeline feed it here.
vertex VertexPayload vertexMain(VertexInput _vertex [[stage_in]]) {
	VertexPayload payload;
	payload.position = float4(_vertex.position, 1.0);
	payload.color = _vertex.color;

	return payload;
}
