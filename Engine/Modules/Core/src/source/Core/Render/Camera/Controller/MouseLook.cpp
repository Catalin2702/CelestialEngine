//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: MouseLook.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-01
// Updated by: Catalin Chirosca
// Updated: 2026-11-01
//

#include "Core/Render/Camera/Controller/MouseLook.hpp"

#include <algorithm>
#include <cmath>

namespace CE::Core {

MouseLook::MouseLook(const CameraLookSettings& settings): _settings(settings) {}

void MouseLook::SetFromRotation(const glm::quat& rotation) {
	const glm::vec3 forward = rotation * CE_WORLD_FORWARD;

	// Inverts GetRotation on the forward vector (0, 0, -1): yaw is its heading on the horizontal plane, pitch how far
	// it rises above it. The clamp on y guards asin against a component a rounding error pushed past 1.
	_yaw = glm::degrees(std::atan2(-forward.x, -forward.z));
	_pitch = std::clamp(glm::degrees(std::asin(std::clamp(forward.y, -1._f32, 1._f32))), _settings.minPitch, _settings.maxPitch);
}

void MouseLook::SetSettings(const CameraLookSettings& settings) {
	_settings = settings;
}

void MouseLook::Rotate(const f32 deltaX, const f32 deltaY) {
	// A positive yaw turns left (counter-clockwise around +Y seen from above), while screen X grows to the right.
	_yaw -= deltaX * _settings.sensitivity;
	// Wrapped into [-180, 180]: an angle that only ever grows would lose float precision after enough turns.
	_yaw = std::remainder(_yaw, 360._f32);

	// A positive pitch looks up, while screen Y grows downwards: moving the mouse down has to look down.
	const f32 pitchDelta = deltaY * _settings.sensitivity;
	_pitch = std::clamp(_settings.invertY ? _pitch + pitchDelta : _pitch - pitchDelta, _settings.minPitch, _settings.maxPitch);
}

glm::quat MouseLook::GetRotation() const {
	// Applied right to left: the pitch tilts the camera around its own right axis first, then the yaw turns the tilted
	// camera around the world vertical - so the horizon stays level whatever the two angles are.
	const glm::quat yaw = glm::angleAxis(glm::radians(_yaw), CE_WORLD_UP);
	const glm::quat pitch = glm::angleAxis(glm::radians(_pitch), CE_WORLD_RIGHT);

	return yaw * pitch;
}

}
