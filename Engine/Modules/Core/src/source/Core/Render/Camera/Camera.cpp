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
	_rotation(cameraDescriptor.rotation) {}

void Camera::LookAt(const glm::vec3& target, const glm::vec3& up) {
	if (glm::all(glm::epsilonEqual(target, _position, glm::epsilon<f32>())))
		return; // direzione degenere: lookAt produrrebbe NaN
	_rotation = glm::conjugate(glm::quat_cast(glm::lookAt(_position, target, up)));
}

}
