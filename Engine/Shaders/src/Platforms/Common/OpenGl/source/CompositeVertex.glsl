//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: CompositeVertex.glsl
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#version 330 core

// Same layout as the scene shaders, so the quad needs no BufferLayout of its own. inputColor is bound and unused.
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec4 inputColor;

void main() {
    gl_Position = vec4(inputPosition, 1.0);
}
