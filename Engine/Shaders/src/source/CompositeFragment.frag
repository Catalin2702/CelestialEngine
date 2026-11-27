//
// Module: CelestialEngine/Engine/Shaders
// File: CompositeFragment.frag
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#version 450

// The scene texture: binding = Types::SCENE_COLOR_TEXTURE_SLOT, where ForwardRenderer binds it. In OpenGL the name is
// what OpenGlPipelineState points at that unit when the program links, so it stays "sceneColor".
layout(set = 0, binding = 0) uniform sampler2D sceneColor;

layout(location = 0) out vec4 outputColor;

void main() {
    // texelFetch, not texture(): integer coordinates, no filtering, no wrap - the scene texture has the size of the
    // framebuffer, so the texel under this pixel is the one at its coordinate. gl_FragCoord is in the backend's own
    // pixel convention (the compiler sees to that), the same one the scene texture was drawn with.
    outputColor = texelFetch(sceneColor, ivec2(gl_FragCoord.xy), 0);
}
