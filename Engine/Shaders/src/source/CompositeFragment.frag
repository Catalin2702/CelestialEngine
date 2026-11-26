//
// Module: CelestialEngine/Engine/Shaders
// File: CompositeFragment.frag
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#version 450

// The scene texture, at the slot ForwardRenderer binds it to (SetFragmentTexture(0, ...)). In OpenGL the name is what
// OpenGlCommandEncoder::SetFragmentTexture looks up, so it stays "sceneColor".
layout(set = 0, binding = 0) uniform sampler2D sceneColor;

layout(location = 0) out vec4 outputColor;

void main() {
    // texelFetch, not texture(): integer coordinates, no filtering, no wrap - the scene texture has the size of the
    // framebuffer, so the texel under this pixel is the one at its coordinate. gl_FragCoord is in the backend's own
    // pixel convention (the compiler sees to that), the same one the scene texture was drawn with.
    outputColor = texelFetch(sceneColor, ivec2(gl_FragCoord.xy), 0);
}