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


namespace CE::Core {

struct CE_CORE_API CameraDescriptor {
	glm::vec3 position;
	glm::quat rotation;
};

}

#endif //CE_CORE_RENDER_CAMERA_CAMERADESCRIPTOR_HPP
