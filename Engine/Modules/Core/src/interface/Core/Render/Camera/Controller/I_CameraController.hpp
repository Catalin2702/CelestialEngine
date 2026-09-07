//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: I_CameraController.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_I_CAMERACONTROLLER_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_I_CAMERACONTROLLER_HPP

#include "Types/Types.hpp"


namespace CE::Events {
	class Event;
}

namespace CE::Core {

class Camera;

class I_CameraController {
public:
	virtual ~I_CameraController() = default;

public:
	virtual void OnAttach(Camera& camera) = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate(Camera& camera, f32 deltaTime) = 0;
	virtual bool OnEvent(Events::Event& event) = 0;

public:
	[[nodiscard]] virtual Types::CameraController GetController() const = 0;
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_I_CAMERACONTROLLER_HPP
