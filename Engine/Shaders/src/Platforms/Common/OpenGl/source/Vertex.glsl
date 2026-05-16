//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: Vertex.glsl
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-05-16
//

#version 330 core

layout(location = 0) in vec3 inputPosition;

out vec3 position;

void main() {
    position = inputPosition;
    gl_Position = vec4(inputPosition, 1.0);
}
