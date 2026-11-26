//
// Module: CelestialEngine/Engine/Shaders
// File: CompositeVertex.vert
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#version 450

// The composite pass's full-screen quad. Same vertex layout as the scene shaders, so the quad needs no BufferLayout of
// its own; the colour is declared and unused.
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec4 inputColor;

void main() {
    gl_Position = vec4(inputPosition, 1.0);
}
