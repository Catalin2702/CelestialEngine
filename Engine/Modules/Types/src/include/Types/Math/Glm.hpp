//
// Module: CelestialEngine/Engine/Modules/Types/Math
// File: Glm.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#pragma once

#ifndef CE_TYPES_MATH_GLM_HPP
#define CE_TYPES_MATH_GLM_HPP

// The engine's one door to glm. glm is header-only, so what has to stay identical in every translation unit is not a
// library but its configuration (GLM_FORCE_*): two files seeing glm configured differently compile the same inline
// functions differently, an ODR violation nobody is told about. That configuration is set as compile definitions on
// CE_Vendor_Glm - never with a #define in a header - and only CE_Types may link it (ce_restrict_link in
// Engine/Vendor/Glm/CMakeLists.txt): every other target gets glm, and the same configuration, through CE_Types.
// CE_GLM_CONFIGURED comes with that configuration, so a file that reached glm some other way stops here.
#if !defined(CE_GLM_CONFIGURED)
	#error "glm must be reached through CE_Types: link CE_Types, not glm or CE_Vendor_Glm (see Engine/Vendor/Glm/CMakeLists.txt)"
#endif

#include "Types/Var/Vars.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <format>
#include <string_view>

/// The engine's world axes: right-handed, Y up, looking down -Z.
constexpr glm::vec3 CE_WORLD_RIGHT{1.0_f32, 0.0_f32, 0.0_f32};
constexpr glm::vec3 CE_WORLD_UP{0.0_f32, 1.0_f32, 0.0_f32};
constexpr glm::vec3 CE_WORLD_FORWARD{0.0_f32, 0.0_f32, -1.0_f32};

constexpr std::string format_as(const glm::vec3 value) {
	return std::format("({}, {}, {})", value.x, value.y, value.z);
}

template <>
struct std::formatter<glm::vec3>: std::formatter<std::string_view> {
	auto format(const glm::vec3 value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

constexpr std::string format_as(const glm::quat value) {
	return std::format("({}, {}, {}, {})", value.w, value.x, value.y, value.z);
}

template <>
struct std::formatter<glm::quat>: std::formatter<std::string_view> {
	auto format(const glm::quat value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_MATH_GLM_HPP
