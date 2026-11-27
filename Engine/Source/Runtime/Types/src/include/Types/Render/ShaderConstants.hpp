//
// Module: CelestialEngine/Engine/Source/Runtime/Types/Render
// File: ShaderConstants.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-29
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADERCONSTANTS_HPP
#define CE_TYPES_RENDER_SHADERCONSTANTS_HPP

#include "Types/Var/Vars.hpp"

#include "Types/Math/Glm.hpp"

namespace CE::Types {

inline constexpr u32 MAX_CONSTANT_SLOTS = 8;

inline constexpr u32 CAMERA_CONSTANTS_SLOT = 1;

inline constexpr u32 OBJECT_CONSTANTS_SLOT = 2;

/// Slots a fragment stage can read textures from. Sixteen is the floor OpenGL guarantees for GL_MAX_TEXTURE_IMAGE_UNITS
/// and the number of sampler slots Metal gives a stage, so a slot below it is valid on every backend.
inline constexpr u32 MAX_TEXTURE_SLOTS = 16;

/// The scene colour the composite pass reads (CompositeFragment.frag, binding = 0).
inline constexpr u32 SCENE_COLOR_TEXTURE_SLOT = 0;

/// The base colour a textured draw samples (TexturedFragment.frag, binding = 0). Texture slot N and sampler slot N go
/// together: the shaders declare one combined sampler2D per slot, which CE_ShaderCompiler turns into [[texture(N)]] and
/// [[sampler(N)]] for Metal, and OpenGL binds both to texture unit N. One number per material input, not two.
inline constexpr u32 BASE_COLOR_TEXTURE_SLOT = 0;

struct alignas(16) CameraConstants {
	glm::mat4 viewProjection{1.0_f32};
};

struct alignas(16) ObjectConstants {
	glm::mat4 transform{1.0_f32};
};

static_assert(CAMERA_CONSTANTS_SLOT < MAX_CONSTANT_SLOTS and OBJECT_CONSTANTS_SLOT < MAX_CONSTANT_SLOTS,
	"A constant slot is past the last one every backend reserves!");

static_assert(SCENE_COLOR_TEXTURE_SLOT < MAX_TEXTURE_SLOTS and BASE_COLOR_TEXTURE_SLOT < MAX_TEXTURE_SLOTS,
	"A texture slot is past the last one every backend offers!");

static_assert(sizeof(CameraConstants) == 64, "CameraConstants must match its block in Engine/Shaders/src/Vertex.vert!");

static_assert(sizeof(ObjectConstants) == 64, "ObjectConstants must match its block in Engine/Shaders/src/Vertex.vert!");

}

#endif //CE_TYPES_RENDER_SHADERCONSTANTS_HPP
