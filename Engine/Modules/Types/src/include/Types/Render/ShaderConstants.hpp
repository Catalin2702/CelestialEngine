//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: ShaderConstants.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-29
// Updated by: Catalin Chirosca
// Updated: 2026-10-29
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADERCONSTANTS_HPP
#define CE_TYPES_RENDER_SHADERCONSTANTS_HPP

#include "Types/Var/Vars.hpp"

#include <glm/glm.hpp>

namespace CE::Types {

inline constexpr u32 MAX_CONSTANT_SLOTS = 8;

inline constexpr u32 CAMERA_CONSTANTS_SLOT = 1;

inline constexpr u32 OBJECT_CONSTANTS_SLOT = 2;

struct alignas(16) CameraConstants {
	glm::mat4 viewProjection{1.0_f32};
};

struct alignas(16) ObjectConstants {
	glm::mat4 transform{1.0_f32};
};

static_assert(CAMERA_CONSTANTS_SLOT < MAX_CONSTANT_SLOTS and OBJECT_CONSTANTS_SLOT < MAX_CONSTANT_SLOTS,
			"A constant slot is past the last one every backend reserves!");

static_assert(sizeof(CameraConstants) == 64, "CameraConstants must match its twins in the shader files!");

static_assert(sizeof(ObjectConstants) == 64, "ObjectConstants must match its twins in the shader files!");

}

#endif //CE_TYPES_RENDER_SHADERCONSTANTS_HPP
