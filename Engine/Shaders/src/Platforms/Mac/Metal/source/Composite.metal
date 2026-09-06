//
// Module: CelestialEngine/Engine/Shaders/Platforms/Mac/Metal
// File: Composite.metal
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include <metal_stdlib>
#include "Common.h"

using namespace metal;

// The pass that writes the drawable, and the only one that does. It reads the scene texture the rest of the frame
// drew into, so the back buffer goes from its previous contents straight to the finished image - one write, tile by
// tile, with no intermediate state in between for a display scanning it to catch.
//
// It shares VertexInput with the scene shaders: the quad is a normal vertex buffer with position and colour, so no
// new BufferLayout and no second vertex descriptor are needed. The colour is ignored here.
struct CompositePayload {
	float4 position [[position]];
};

vertex CompositePayload compositeVertexMain(VertexInput _vertex [[stage_in]]) {
	CompositePayload payload;
	payload.position = float4(_vertex.position, 1.0);
	return payload;
}

// read(), not sample(): the scene texture is exactly the size of the drawable, so the texel under this fragment is
// the one at its pixel coordinate. That is what makes a sampler unnecessary - and a fetch is also the only way to be
// sure no filtering has quietly blurred a UI that was drawn pixel-aligned.
fragment half4 compositeFragmentMain(CompositePayload _fragment [[stage_in]], texture2d<half, access::read> sceneColor [[texture(0)]]) {
	return sceneColor.read(uint2(_fragment.position.xy));
}
