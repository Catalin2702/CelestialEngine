//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: CameraDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
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
	glm::quat rotation {1.0, 0.0, 0.0,0.0 }; ///<  Identity: glm::quat takes w first, so {1,0,0,0} is no rotation at all.
	std::optional<glm::vec3> lookAtTarget; ///< If present, wins against rotation
};

}

#endif //CE_CORE_RENDER_CAMERA_CAMERADESCRIPTOR_HPP
