//
// Module: CelestialEngine/Engine/Shaders
// File: TexturedVertex.vert
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#version 450

// Its own vertex input: position and texture coordinate, built from a BufferLayout of Float3 + Float2. Vertex.vert stays
// position + colour, which the scene geometry and the composite quad rely on.
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec2 inputTexCoord;

// The same blocks, names and bindings as Vertex.vert: ForwardRenderer uploads them the same way for every draw.
layout(set = 0, binding = 1, std140) uniform CameraConstants {
    mat4 viewProjection;
} camera;

layout(set = 0, binding = 2, std140) uniform ObjectConstants {
    mat4 transform;
} object;

// Read by TexturedFragment.frag at the same location, interpolated across the triangle.
layout(location = 0) out vec2 texCoord;

void main() {
    texCoord = inputTexCoord;
    gl_Position = camera.viewProjection * object.transform * vec4(inputPosition, 1.0);
}
