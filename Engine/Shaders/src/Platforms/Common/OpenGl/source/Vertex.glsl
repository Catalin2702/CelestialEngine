//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: Vertex.glsl
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#version 330 core

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec4 inputColor;

out vec4 color;

void main() {
    color = inputColor;
    gl_Position = vec4(inputPosition, 1.0);
}
