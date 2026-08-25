//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: Fragment.glsl
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#version 330 core

layout(location = 0) out vec4 outputColor;

// Names must match the vertex shader's outputs exactly: in GLSL 330 varyings are linked by name
in vec4 color;

void main() {
    outputColor = color;
}
