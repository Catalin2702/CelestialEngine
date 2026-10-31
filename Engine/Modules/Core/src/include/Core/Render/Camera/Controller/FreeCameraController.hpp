//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: FreeCameraController.hpp
// Created by: Catalin Chirosca
// Created: 2026-10-31
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP

#include "Core/Render/Camera/Controller/I_CameraController.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

struct CameraControllerDescriptor;

class CE_API_EXPORT FreeCameraController final: public I_CameraController {
public:
	explicit FreeCameraController(const CameraControllerDescriptor& cameraControllerDescriptor);

	FreeCameraController(const FreeCameraController&) = delete;
	FreeCameraController(FreeCameraController&&) noexcept = default;

public:
	FreeCameraController& operator = (const FreeCameraController&) = delete;
	FreeCameraController& operator = (FreeCameraController&&) noexcept = default;

public:
	void OnAttach(Camera&) override;
	void OnDetach() override;
	void OnUpdate(Camera& camera, f32 deltaTime) override;
	bool OnEvent(Events::I_Event&) override { return false; }
	[[nodiscard]] Types::CameraController GetController() const override { return Types::CameraController::Free; }
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_FREECAMERACONTROLLER_HPP
