//
// Module: CelestialEngine/Engine/Shaders
// File: Fragment.frag
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#version 450

// Interpolated from Vertex.vert's output at the same location.
layout(location = 0) in vec4 color;

layout(location = 0) out vec4 outputColor;

void main() {
    outputColor = color;
}
