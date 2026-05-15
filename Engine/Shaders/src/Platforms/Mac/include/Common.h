//
// Module: CelestialEngine/Resources/Render/Shaders/Platforms/Mac
// File: Common.h
// Created by: Catalin Chirosca
// Created: 2026-05-15
// Updated by: Catalin Chirosca
// Updated: 2026-05-15
//

#pragma once

#ifndef CE_SHADERS_PLATFORMS_MAC_COMMON_H
#define CE_SHADERS_PLATFORMS_MAC_COMMON_H

#include <metal_stdlib>
using namespace metal;

constant float4 positions[] = {
	float4(-0.5, -0.5, 0.0, 1.0),
	float4(0.5, -0.5, 0.0, 1.0),
	float4(0.0,  0.5, 0.0, 1.0)
};

constant half3 colors[] = {
	half3(1.0, 0.0, 0.0),
	half3(0.0, 1.0, 0.0),
	half3(0.0, 0.0, 1.0)
};

struct VertexPayload {
	float4 position [[position]];
	half3 color;
};


#endif //CE_SHADERS_PLATFORMS_MAC_COMMON_H
