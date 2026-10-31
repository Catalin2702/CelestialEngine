//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: FreeCameraController.cpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#include "Core/Render/Camera/Controller/FreeCameraController.hpp"
#include "Core/Render/Camera/Camera.hpp"
#include "Core/Input/Input.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace CE::Core {

FreeCameraController::FreeCameraController(const CameraControllerDescriptor&) {}

void FreeCameraController::OnAttach(Camera&) {}

void FreeCameraController::OnDetach() {}

void FreeCameraController::OnUpdate(Camera& camera, const f32 deltaTime) {
	constexpr f32 CAMERA_MOVE_SPEED = 0.5_f32;
	constexpr f32 CAMERA_ROTATION_SPEED = 0.005_f32;

	const auto movementSpeed = CAMERA_MOVE_SPEED * deltaTime;
	const auto rotationSpeed = CAMERA_ROTATION_SPEED * deltaTime;

	glm::vec3 position = camera.GetPosition();
	glm::quat rotation = camera.GetRotation();

	if (Input::IsKeyPressed(Types::KeyboardKeyCode::W))
		position.y += movementSpeed;
	else if (Input::IsKeyPressed(Types::KeyboardKeyCode::S))
		position.y -= movementSpeed;

	if (Input::IsKeyPressed(Types::KeyboardKeyCode::D))
		position.x += movementSpeed;
	else if (Input::IsKeyPressed(Types::KeyboardKeyCode::A))
		position.x -= movementSpeed;

	const auto [deltaX, deltaY] = Input::GetMouseDeltaXY();

	if (not IsNearlyZero(deltaX))
		rotation.y -= deltaX * rotationSpeed;
	if (not IsNearlyZero(deltaY))
		rotation.x -= deltaY * rotationSpeed;

	camera.SetPosition(position);
	camera.SetRotation(rotation);
}

}
