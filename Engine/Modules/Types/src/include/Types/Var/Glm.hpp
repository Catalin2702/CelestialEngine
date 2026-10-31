//
// Module: CelestialEngine/Engine/Modules/Types/Var
// File: Glm.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#pragma once

#ifndef CE_TYPES_VAR_GLM_HPP
#define CE_TYPES_VAR_GLM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <format>
#include <string_view>

constexpr std::string format_as(const glm::vec3 value) {
	return std::format("({}, {}, {})", value.x, value.y, value.z);
}

template<>
struct std::formatter<glm::vec3>: std::formatter<std::string_view> {
	auto format(const glm::vec3 value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value),ctx);
	}
};

constexpr std::string format_as(const glm::quat value) {
	return std::format("({}, {}, {}, {})", value.w, value.x, value.y, value.z);
}

template<>
struct std::formatter<glm::quat>: std::formatter<std::string_view> {
	auto format(const glm::quat value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value),ctx);
	}
};

#endif //CE_TYPES_VAR_GLM_HPP
