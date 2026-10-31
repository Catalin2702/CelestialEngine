//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: CameraDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CAMERADESCRIPTOR_HPP
#define CE_CORE_RENDER_CAMERA_CAMERADESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <optional>


namespace CE::Core {

struct CE_CORE_API CameraDescriptor {
	glm::vec3 position {0.0, 0.0, 0.0};
	/**
	 * @brief Orientation of the camera, as a unit quaternion
	 * @details A rotation of angle t around the unit axis (ax, ay, az) is stored as
	 *			w = cos(t/2), (x, y, z) = (ax, ay, az) * sin(t/2): w says how much it rotates, (x, y, z) around which axis.
	 *			The components are NOT Euler angles. Only a rotation around a single axis maps onto one of them: a pure
	 *			pitch (around +X) touches only w and x, a pure yaw (around +Y) only w and y, a pure roll (around +Z) only
	 *			w and z. Once two of them are combined, every component depends on all three angles.
	 *			To build it from angles use glm::quat(glm::radians(glm::vec3(pitch, yaw, roll))).
	 *			Identity: glm::quat takes w first, so {1,0,0,0} is no rotation at all.
	 */
	glm::quat rotation {1.0, 0.0, 0.0, 0.0};
	std::optional<glm::vec3> lookAtTarget = std::nullopt; ///< If present, wins against rotation
};

}

#endif //CE_CORE_RENDER_CAMERA_CAMERADESCRIPTOR_HPP
