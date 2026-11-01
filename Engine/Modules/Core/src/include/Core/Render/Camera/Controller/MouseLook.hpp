//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: MouseLook.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-01
// Updated by: Catalin Chirosca
// Updated: 2026-11-01
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_MOUSELOOK_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_MOUSELOOK_HPP

#include "Core/Render/Camera/Controller/CameraControllerDescriptor.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace CE::Core {

/**
 * @class MouseLook
 * @brief Turns pointer motion into a yaw/pitch orientation, for every controller that looks around with the mouse
 * @details Stores the orientation as two angles rather than as a quaternion it keeps rotating: composing a small
 *			rotation onto the previous one every frame slowly builds up roll, and leaves no number to clamp the pitch
 *			against. The quaternion is rebuilt from the two angles whenever it is asked for, so it never drifts.
 *
 *			Takes the motion as plain numbers instead of reading Input itself: the controller decides when the look is
 *			active (a held button, a captured cursor), and anything that yields a delta - a gamepad stick, a test - can
 *			drive it the same way.
 */
class CE_CORE_API MouseLook {
public:
	explicit MouseLook(const CameraLookSettings& settings);

public:
	/**
	 * @brief Starts from an existing orientation, e.g. the one the camera was placed with
	 * @details Only the look direction survives: a yaw/pitch pair cannot express roll, so a rolled camera is levelled.
	 */
	void SetFromRotation(const glm::quat& rotation);

	/**
	 * @brief Change the settings
	 * @param settings
	 */
	void SetSettings(const CameraLookSettings& settings);

	/**
	 * @brief Applies one frame of pointer motion
	 * @param deltaX Horizontal motion in points, positive to the right
	 * @param deltaY Vertical motion in points, positive downwards
	 */
	void Rotate(f32 deltaX, f32 deltaY);

public:
	/**
	 * @brief Builds the orientation the two angles describe
	 */
	[[nodiscard]] glm::quat GetRotation() const;

	/**
	 * @brief Degrees around the world up, in [-180, 180]; 0 looks down -Z and positive turns left.
	 */
	[[nodiscard]] f32 GetYaw() const { return _yaw; }

	/**
	 * @brief Degrees around the camera right, within [minPitch, maxPitch]; positive looks up.
	 */
	[[nodiscard]] f32 GetPitch() const {return _pitch; }

private:
	CameraLookSettings _settings;
	f32 _yaw = 0._f32;
	f32 _pitch = 0._f32;
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_MOUSELOOK_HPP
