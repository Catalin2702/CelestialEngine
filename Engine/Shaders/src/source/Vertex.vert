//
// Module: CelestialEngine/Engine/Shaders
// File: Vertex.vert
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#version 450

// The scene's vertex shader, written once for every backend: CE_ShaderCompiler turns it into MSL for Metal and GLSL 4.10
// for OpenGL. Vulkan-style GLSL - every input has a location, every resource a set and a binding - because SPIR-V needs
// them explicit, and they are what the back ends translate into each API's own slots.

// The vertex buffer's elements, in BufferLayout order: element i is location i (and [[attribute(i)]] in Metal).
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec4 inputColor;

// binding = Types::CAMERA_CONSTANTS_SLOT and Types::OBJECT_CONSTANTS_SLOT (Types/Render/ShaderConstants.hpp). The
// block names are what OpenGL binds them by, so they stay the C++ struct names.
layout(set = 0, binding = 1, std140) uniform CameraConstants {
    mat4 viewProjection;
} camera;

layout(set = 0, binding = 2, std140) uniform ObjectConstants {
    mat4 transform;
} object;

// Read by Fragment.frag at the same location.
layout(location = 0) out vec4 color;

void main() {
    color = inputColor;
    gl_Position = camera.viewProjection * object.transform * vec4(inputPosition, 1.0);
}
