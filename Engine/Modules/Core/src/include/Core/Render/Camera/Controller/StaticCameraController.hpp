//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: StaticCameraController.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_STATICCAMERACONTROLLER_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_STATICCAMERACONTROLLER_HPP

#include "Core/Render/Camera/Controller/I_CameraController.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

struct CameraControllerDescriptor;

class CE_API_EXPORT StaticCameraController final: public I_CameraController {
public:
	StaticCameraController(const CameraControllerDescriptor&);

	StaticCameraController(const StaticCameraController&) = delete;
	StaticCameraController(StaticCameraController&&) noexcept = default;

public:
	StaticCameraController& operator = (const StaticCameraController&) = delete;
	StaticCameraController& operator = (StaticCameraController&&) noexcept = default;

public:
	void OnAttach(Camera&) override;
	void OnDetach() override;
	void OnUpdate(Camera&, f32) override;
	bool OnEvent(Events::I_Event&) override { return false; }
	[[nodiscard]] Types::CameraController GetController() const override { return Types::CameraController::Static; }
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_STATICCAMERACONTROLLER_HPP
