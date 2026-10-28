//
// Module: CelestialEngine/Engine/Shaders/Platforms/Mac/Metal
// File: Common.h
// Created by: Catalin Chirosca
// Created: 2026-05-15
// Updated by: Catalin Chirosca
// Updated: 2026-10-29
//

#pragma once

#ifndef CE_SHADERS_PLATFORMS_MAC_COMMON_H
#define CE_SHADERS_PLATFORMS_MAC_COMMON_H

#include <metal_stdlib>
using namespace metal;

// Attribute indices, not names: Metal binds by [[attribute(i)]], and the pipeline's MTLVertexDescriptor is built
// from the engine's BufferLayout in declaration order - so element i there is attribute i here. Getting the two out
// of step is silent, and shows up as a window that clears and draws nothing.
struct VertexInput {
	float3 position [[attribute(0)]];
	float4 color	[[attribute(1)]];
};

struct VertexPayload {
	float4 position [[position]];
	float4 color;
};

enum ConstantsSlot: uint {
	CameraConstantsSlot = 1,
	ObjectConstantsSlot = 2,
};

// Mirrors Types/Render/ShaderConstants.hpp: keep the two in step
struct CameraConstants {
	float4x4 viewProjection;
};

// Mirrors Types/Render/ShaderConstants.hpp: keep the two in step.
struct ObjectConstants {
	float4x4 transform;
};

#endif //CE_SHADERS_PLATFORMS_MAC_COMMON_H
