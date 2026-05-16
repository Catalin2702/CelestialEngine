//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: Fragment.glsl
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-05-16
//

#version 330 core

layout(location = 0) out vec4 color;

in vec3 position;

void main() {
    color = vec4(position * 0.5 + 0.5, 1.0);
}
