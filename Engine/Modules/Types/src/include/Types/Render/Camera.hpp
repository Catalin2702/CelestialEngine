//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Camera.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_TYPES_RENDER_CAMERA_HPP
#define CE_TYPES_RENDER_CAMERA_HPP

#include <Define/DynamicLinker.hpp>
#include "Types/Var/Vars.hpp"

#include <glm/glm.hpp>

#include <format>
#include <string_view>


namespace CE::Types {

enum class CameraProjection: u8 {
	None,
	Orthographic,
	Perspective,
};

enum class CameraController: u8 {
	None,
	Static,
	Free,
	Orbit,
	FirstPerson,
	ThirdPerson,
	TopDown,
	SideScroller,
	Cinematic,
	Scripted,
};

enum class ClipConvention: u8 {
	None,
	ZeroToOne,
	NegativeOneToOne
};

struct CE_TYPES_API CameraData {
	glm::mat4 view{1.0};
	glm::mat4 projection{1.0};
	glm::mat4 viewProjection{1.0};
	glm::vec4 position{0.0, 0.0, 0.0, 1.0};   // serve alle luci speculari
};

}

constexpr std::string_view format_as(const CE::Types::CameraProjection projection) {
	switch (projection) {
		case CE::Types::CameraProjection::None: return "None";
		case CE::Types::CameraProjection::Orthographic: return "Orthographic";
		case CE::Types::CameraProjection::Perspective: return "Perspective";
		default:
			return "Unknown Camera Projection";
	}
}

constexpr std::string_view format_as(const CE::Types::CameraController controller) {
	switch (controller) {
		case CE::Types::CameraController::None: return "None";
		case CE::Types::CameraController::Static: return "Static";
		case CE::Types::CameraController::Free: return "Free";
		case CE::Types::CameraController::Orbit: return "Orbit";
		case CE::Types::CameraController::FirstPerson: return "FirstPerson";
		case CE::Types::CameraController::ThirdPerson: return "ThirdPerson";
		case CE::Types::CameraController::TopDown: return "TopDown";
		case CE::Types::CameraController::SideScroller: return "SideScroller";
		case CE::Types::CameraController::Cinematic: return "Cinematic";
		case CE::Types::CameraController::Scripted: return "Scripted";
		default:
			return "Unknown Camera Controller";
	}
}

constexpr std::string_view format_as(const CE::Types::ClipConvention convention) {
	switch (convention) {
		case CE::Types::ClipConvention::None: return "None";
		case CE::Types::ClipConvention::ZeroToOne: return "ZeroToOne";
		case CE::Types::ClipConvention::NegativeOneToOne: return "NegativeOneToOne";
		default:
			return "Unknown Clip Convention";
	}
}

template<>
struct std::formatter<CE::Types::CameraProjection>: std::formatter<std::string_view> {
	auto format(const CE::Types::CameraProjection value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

template<>
struct std::formatter<CE::Types::CameraController>: std::formatter<std::string_view> {
	auto format(const CE::Types::CameraController value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

template<>
struct std::formatter<CE::Types::ClipConvention>: std::formatter<std::string_view> {
	auto format(const CE::Types::ClipConvention value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_CAMERA_HPP
