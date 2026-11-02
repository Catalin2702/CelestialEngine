//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: FreeCameraController.cpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Render/Camera/Controller/FreeCameraController.hpp"
#include "Core/Render/Camera/Camera.hpp"
#include "Core/Input/Input.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace CE::Core {

FreeCameraController::FreeCameraController(const CameraControllerDescriptor& descriptor):
	// The factory only builds this class for the Free alternative, so std::get cannot throw here.
	_descriptor(std::get<CameraFreeControllerDescriptor>(descriptor.descriptor)),
	_look(_descriptor.look),
	_speed(std::clamp(_descriptor.move.speed, _descriptor.minSpeed, _descriptor.maxSpeed)) {}

void FreeCameraController::OnAttach(Camera& camera) {
	// Starts from wherever the camera already looks (its descriptor, a LookAt), instead of snapping it to -Z on the
	// first mouse motion.
	_look.SetFromRotation(camera.GetRotation());
	_velocity = glm::vec3{0._f32};
}

void FreeCameraController::OnDetach() {
	// A capture this controller started must not outlive it: the next controller may have no shortcut to undo it, and
	// would inherit a hidden, frozen cursor. One captured by someone else is theirs to release.
	if (std::exchange(_capturedCursor, false) and Input::IsCursorCaptured())
		Input::SetCursorMode(Types::CursorMode::Normal);
}

void FreeCameraController::OnUpdate(Camera& camera, const f32 deltaTime) {
	_UpdateCapture();
	// Look first, so this frame's movement already follows the direction the camera faces now.
	_UpdateLook(camera);
	_UpdateSpeed();
	_UpdateMove(camera, deltaTime);
}

bool FreeCameraController::_IsLooking() const {
	switch (_descriptor.lookTrigger) {
		case CameraLookTrigger::CursorCaptured: return Input::IsCursorCaptured();
		case CameraLookTrigger::ButtonHeld: return Input::IsMouseButtonPressed(_descriptor.lookButton);
		case CameraLookTrigger::Always: return true;
	}

	// No default inside the switch: -Wswitch then flags any trigger added to the enum and not handled above. This is
	// only reached by a value outside the enum.
	return false;
}

void FreeCameraController::_UpdateCapture() {
	if (not Input::IsShortcutJustPressed(_descriptor.captureShortcut))
		return;

	_capturedCursor = not Input::IsCursorCaptured();
	Input::SetCursorMode(_capturedCursor ? Types::CursorMode::Captured : Types::CursorMode::Normal);
}

void FreeCameraController::_UpdateLook(Camera& camera) {
	if (not _IsLooking())
		return;

	const auto [deltaX, deltaY] = Input::GetMouseDeltaXY();
	if (IsNearlyZero(deltaX) and IsNearlyZero(deltaY))
		return;

	_look.Rotate(deltaX, deltaY);
	camera.SetRotation(_look.GetRotation());
}

void FreeCameraController::_UpdateSpeed() {
	const auto notches = Input::GetScrollDeltaY();
	if (IsNearlyZero(notches))
		return;

	// Multiplicative, one speedScrollStep per notch: an additive step would be imperceptible at 200 units per second
	// and would cross zero at 0.5. pow also handles the fractional notches a trackpad reports.
	_speed = std::clamp(_speed * std::pow(_descriptor.speedScrollStep, notches), _descriptor.minSpeed, _descriptor.maxSpeed);
}

void FreeCameraController::_UpdateMove(Camera& camera, f32 deltaTime) {
	glm::vec3 direction{0.0_f32};

	if (Input::IsKeyPressed(Types::KeyboardKeyCode::W))
		direction += camera.GetForward();
	else if (Input::IsKeyPressed(Types::KeyboardKeyCode::S))
		direction -= camera.GetForward();

	if (Input::IsKeyPressed(Types::KeyboardKeyCode::D))
		direction += camera.GetRight();
	else if (Input::IsKeyPressed(Types::KeyboardKeyCode::A))
		direction -= camera.GetRight();

	if (Input::IsKeyPressed(Types::KeyboardKeyCode::E))
		direction += CE_WORLD_UP;
	else if (Input::IsKeyPressed(Types::KeyboardKeyCode::Q))
		direction -= CE_WORLD_UP;

	auto speed = _speed;
	if (Input::IsKeyPressed(Types::KeyboardKeyCode::LeftShift))
		speed *= _descriptor.move.sprintMultiplier;

	// Nothing held, or opposite keys cancelling out, asks for a stop: normalising a null vector would fill the
	// position with NaNs. Normalised otherwise, so W + D does not fly sqrt(2) times faster than W alone.
	const glm::vec3 targetVelocity = IsNearlyZero(glm::dot(direction, direction)) ?
		glm::vec3{0.0_f32} : glm::normalize(direction) * speed;

	if (const auto acceleration = _descriptor.move.acceleration; acceleration <= 0._f32) {
		// 0 means instant, as the descriptor documents - and it keeps the division below away from zero.
		_velocity = targetVelocity;
	}
	else {
		// Moves towards the target by a fixed rate rather than lerping by a factor, so it reaches it in exactly
		// `acceleration` seconds from rest instead of only approaching it. The rate follows the faster of the two
		// velocities, so releasing the sprint slows down just as quickly as pressing it sped up.
		const glm::vec3 change = targetVelocity - _velocity;
		const auto distance = glm::length(change);
		const auto maxStep = std::max(glm::length(targetVelocity), glm::length(_velocity)) / acceleration * deltaTime;

		_velocity = distance <= maxStep ? targetVelocity : _velocity + change / distance * maxStep;
	}

	if (IsNearlyZero(glm::dot(_velocity, _velocity)))
		return;

	camera.SetPosition(camera.GetPosition() + _velocity * deltaTime);
}

}
