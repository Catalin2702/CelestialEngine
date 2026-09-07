//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: Camera.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CAMERA_HPP
#define CE_CORE_RENDER_CAMERA_CAMERA_HPP

#include "Core/Render/Camera/Projection/I_CameraProjection.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace CE::Core {

struct CameraDescriptor;
struct CameraProjectionDescriptor;

class CE_CORE_API Camera {
public:
	Camera(const CameraDescriptor& cameraDescriptor, const CameraProjectionDescriptor& projectionDescriptor);

	Camera(const Camera&) = delete;
	Camera(Camera&& other) noexcept = default;

public:
	Camera& operator = (const Camera&) = delete;
	Camera& operator = (Camera&& other) noexcept = default;

public:
	void SetProjection(const std::shared_ptr<I_CameraProjection>& projection) { _projection = projection; }

	void SetPosition(const glm::vec3& position) { _position = position; }
	void SetRotation(const glm::quat& rotation) { _rotation = glm::normalize(rotation); }
	void LookAt(const glm::vec3& target, const glm::vec3& up = {0.0, 1.0, 0.0});

public:
	[[nodiscard]] const I_CameraProjection& GetProjection() const { return *_projection; }
	[[nodiscard]] I_CameraProjection& GetProjection() { return *_projection; }

	[[nodiscard]] const glm::vec3& GetPosition() const { return _position; }
	[[nodiscard]] const glm::quat& GetRotation() const { return _rotation; }

	[[nodiscard]] glm::vec3 GetForward() const { return _rotation * CE_WORLD_FORWARD; }
	[[nodiscard]] glm::vec3 GetRight() const { return _rotation * CE_WORLD_RIGHT; }
	[[nodiscard]] glm::vec3 GetUp() const { return _rotation * CE_WORLD_UP;}

private:
	std::shared_ptr<I_CameraProjection> _projection;

	glm::vec3 _position;
	glm::quat _rotation;
};

}

#endif //CE_CORE_RENDER_CAMERA_CAMERA_HPP
