//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: FreeCameraController.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP

#include "Core/Render/Camera/Controller/CameraControllerDescriptor.hpp"
#include "Core/Render/Camera/Controller/I_CameraController.hpp"
#include "Core/Render/Camera/Controller/MouseLook.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

class CE_CORE_API FreeCameraController final: public I_CameraController {
public:
	explicit FreeCameraController(const CameraControllerDescriptor& descriptor);

	FreeCameraController(const FreeCameraController&) = delete;
	FreeCameraController(FreeCameraController&&) noexcept = default;

public:
	FreeCameraController& operator = (const FreeCameraController&) = delete;
	FreeCameraController& operator = (FreeCameraController&&) noexcept = default;

public:
	void OnAttach(Camera& camera) override;
	void OnDetach() override;
	void OnUpdate(Camera& camera, f32 deltaTime) override;
	bool OnEvent(Events::I_Event&) override { return false; }
	[[nodiscard]] Types::CameraController GetController() const override { return Types::CameraController::Free; }

private:
	/**
	 * @brief Whether this frame's pointer motion is meant for the camera, as lookTrigger defines it
	 */
	[[nodiscard]] bool _IsLooking() const;

private:
	/**
	 * @brief Captures or releases the cursor when captureShortcut is pressed
	 */
	void _UpdateCapture();

	/**
	 * @brief Turns the camera with the pointer motion of this frame, while _IsLooking says so
	 */
	void _UpdateLook(Camera& camera);

	/**
	 * @brief Retunes the fly speed with the mouse wheel, within [minSpeed, maxSpeed]
	 */
	void _UpdateSpeed();

	/**
	 * @brief Flies the camera along its own axes with WASD, and up/down the world vertical with E/Q
	 */
	void _UpdateMove(Camera& camera, f32 deltaTime);

private:
	CameraFreeControllerDescriptor _descriptor; ///< Tuning, copied out of the variant once so no frame pays for std::get
	MouseLook _look; ///< Built from _descriptor.look, so it must stay declared after it

	f32 _speed = 0.0_f32; ///< Current cruise speed in units per second: move.speed until the wheel retunes it
	glm::vec3 _velocity{0._f32}; ///< World-space velocity, carried across frames so acceleration has something to ease

	bool _capturedCursor = false; ///< Whether this controller is the one captured the cursor, so it knows to release it
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP
