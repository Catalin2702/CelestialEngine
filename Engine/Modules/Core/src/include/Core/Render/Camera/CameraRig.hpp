//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: CameraRig.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CAMERARIG_HPP
#define CE_CORE_RENDER_CAMERA_CAMERARIG_HPP

#include "Core/Render/Camera/Camera.hpp"
#include "Core/Render/Camera/Controller/I_CameraController.hpp"

#include <memory>


namespace CE::Core {

class CameraRig {
public:
	CameraRig(const CameraDescriptor& cameraDescriptor,
		const CameraProjectionDescriptor& cameraProjectionDescriptor,
		const CameraControllerDescriptor& cameraControllerDescriptor);

	CameraRig(const CameraRig&) = delete;
	CameraRig(CameraRig&&) noexcept = default;

public:
	CameraRig& operator = (const CameraRig&) = delete;
	CameraRig& operator = (CameraRig&&) noexcept = default;

public:
	void OnUpdate(f32 deltaTime);
	[[nodiscard]] bool OnEvent(Events::I_Event& event) const;

public:
	void SetProjection(const CameraProjectionDescriptor& descriptor);
	void SetController(const CameraControllerDescriptor& descriptor);

public:
	[[nodiscard]] Types::CameraData GetCameraData() const;
	[[nodiscard]] const Camera& GetCamera() const { return _camera; }
	[[nodiscard]] Camera& GetCamera() { return _camera; }

private:
	Camera _camera;
	std::unique_ptr<I_CameraController> _cameraController;
};

}

#endif //CE_CORE_RENDER_CAMERA_CAMERARIG_HPP
