//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: Camera.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Camera.hpp"
#include "Core/Render/Camera/CameraDescriptor.hpp"

#include <glm/gtc/epsilon.hpp>


namespace CE::Core {

Camera::Camera(const CameraDescriptor& cameraDescriptor, const CameraProjectionDescriptor& projectionDescriptor):
	_projection(I_CameraProjection::MakeCameraProjection(projectionDescriptor)),
	_position(cameraDescriptor.position),
	_rotation(cameraDescriptor.rotation) {
	if (cameraDescriptor.lookAtTarget.has_value())
		LookAt(cameraDescriptor.lookAtTarget.value());
}

void Camera::LookAt(const glm::vec3& target, const glm::vec3& up) {
	// if (glm::all(glm::epsilonEqual(target, _position, glm::epsilon<f32>())))
	// 	return; // direzione degenere: lookAt produrrebbe NaN
	// _rotation = glm::conjugate(glm::quat_cast(glm::lookAt(_position, target, up)));
	const glm::vec3 direction = target - _position;

	/// Nothing to look at: normalising a null direction would fill the view matrix with NaNs.
	if (glm::dot(direction, direction) < glm::epsilon<f32>())
		return;

	/// lookAt derives the right vector from direction x up, which collapses when the two are parallel - looking
	/// straight up or straight down. Swapping in a perpendicular axis keeps the basis well formed.
	glm::vec3 safeUp = up;
	if (const glm::vec3 side = glm::cross(direction, up); glm::dot(side, side) < glm::epsilon<f32>())
		safeUp = glm::abs(glm::normalize(direction).y) < 0.999_f32 ? CE_WORLD_UP: CE_WORLD_FORWARD;

	SetRotation(glm::conjugate(glm::quat_cast(glm::lookAt(_position, target, safeUp))));
}

}
