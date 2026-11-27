//
// Module: CelestialEngine/Engine/Shaders
// File: TexturedFragment.frag
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#version 450

// Interpolated from TexturedVertex.vert's output at the same location.
layout(location = 0) in vec2 texCoord;

// binding = Types::BASE_COLOR_TEXTURE_SLOT. One combined sampler2D: CE_ShaderCompiler turns it into a texture and a
// sampler at the same Metal index, and OpenGL binds both to that texture unit - the unit OpenGlPipelineState points
// this name at when the program links, so the name stays "baseColorTexture".
layout(set = 0, binding = 0) uniform sampler2D baseColorTexture;

layout(location = 0) out vec4 outputColor;

void main() {
    // texture(), not texelFetch(): normalised coordinates, filtered by the sampler bound at the same slot.
    outputColor = texture(baseColorTexture, texCoord);
}
