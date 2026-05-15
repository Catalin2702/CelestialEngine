//
// Module: CelestialEngine/Resources/Render/Shaders/Platforms/Mac
// File: Vertex.metal
// Created by: Catalin Chirosca
// Created: 2026-05-15
// Updated by: Catalin Chirosca
// Updated: 2026-05-15
//

#include <metal_stdlib>
#include "Common.h"
using namespace metal;



vertex VertexPayload vertexMain(uint _vertexId [[vertex_id]]) {
	VertexPayload payload;
	payload.position = positions[_vertexId];
	payload.color = colors[_vertexId];

	return payload;
}

