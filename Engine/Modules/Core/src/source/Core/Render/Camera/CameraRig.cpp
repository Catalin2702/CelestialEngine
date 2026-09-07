//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera
// File: CameraRig.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#include "Core/Render/Camera/CameraRig.hpp"

#include <glm/gtc/matrix_transform.hpp>


namespace CE::Core {

CameraRig::CameraRig(const CameraDescriptor& cameraDescriptor, const CameraProjectionDescriptor& cameraProjectionDescriptor,
	const CameraControllerDescriptor& cameraControllerDescriptor):
	_camera(cameraDescriptor, cameraProjectionDescriptor) {
	SetController(cameraControllerDescriptor);
}

void CameraRig::OnUpdate(const f32 deltaTime) {
	_cameraController->OnUpdate(_camera, deltaTime);
}

bool CameraRig::OnEvent(Events::I_Event& event) const {
	return _cameraController->OnEvent(event);
}

void CameraRig::SetProjection(const CameraProjectionDescriptor& descriptor) {
	if (_cameraController)
		_cameraController->OnDetach();

	_camera.SetProjection(descriptor);

	if (_cameraController)
		_cameraController->OnAttach(_camera);
}

void CameraRig::SetController(const CameraControllerDescriptor& descriptor) {
	if (_cameraController)
		_cameraController->OnDetach();

	_cameraController = I_CameraController::MakeCameraController(descriptor);

	_cameraController->OnAttach(_camera);
}

Types::CameraData CameraRig::GetCameraData() const {
	const glm::quat& rotation = _camera.GetRotation();
	const glm::vec3& position = _camera.GetPosition();

	/// The camera transform is rigid, so its inverse is the conjugate of the rotation followed by the negated
	/// translation - no general 4x4 inverse needed, and no drift from one.
	const glm::mat4 view = glm::translate(glm::mat4_cast(glm::conjugate(rotation)), -position);
	const glm::mat4& projection = _camera.GetProjection().GetProjectionMatrix();

	return Types::CameraData{
		.view = view,
		.projection = projection,
		.viewProjection = projection * view,
		.position = glm::vec4{position, 1.0_f32},
	};
}

}
