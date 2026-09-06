//
// Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
// File: CompositeFragment.glsl
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#version 330 core

layout(location = 0) out vec4 outputColor;

// The name matters: OpenGlCommandEncoder::SetFragmentTexture looks this uniform up to tell it which texture unit to
// read. GLSL 330 has no binding qualifier to say it here instead.
uniform sampler2D sceneColor;

void main() {
    // texelFetch, not texture(): integer coordinates, no filtering, no wrap - the GL half of the read() in the
    // Metal shader. gl_FragCoord is already in window pixels, and the scene texture matches the framebuffer.
    outputColor = texelFetch(sceneColor, ivec2(gl_FragCoord.xy), 0);
}
