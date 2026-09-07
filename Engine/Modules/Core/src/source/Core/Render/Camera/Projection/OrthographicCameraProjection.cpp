//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: OrthographicCameraProjection.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Projection/OrthographicCameraProjection.hpp"
#include "Core/Render/Camera/Projection/CameraProjectionDescriptor.hpp"

#include <glm/ext/matrix_clip_space.hpp>


namespace CE::Core {

OrthographicCameraProjection::OrthographicCameraProjection(const CameraProjectionDescriptor& descriptor):
	_convention(descriptor.convention),
	_nearClip(descriptor.nearClip),
	_farClip(descriptor.farClip),
	_width(descriptor.viewportWidth),
	_height(descriptor.viewportHeight),
	_orthographicSize(descriptor.orthographicSize)
{
	_Rebuild();
	_dirty = false;
}

const glm::mat4& OrthographicCameraProjection::GetProjectionMatrix() const {
	if (_dirty) {
		_Rebuild();
		_dirty = false;
	}
	return _projection;
}

f32 OrthographicCameraProjection::GetAspectRatio() const {
	return static_cast<f32>(_width) / static_cast<f32>(_height);
}

void OrthographicCameraProjection::_Rebuild() const {
	const f32 halfHeight = _orthographicSize;
	const f32 halfWidth = _orthographicSize * GetAspectRatio();

	_projection = _convention == Types::ClipConvention::ZeroToOne ?
		glm::orthoRH_ZO(-halfWidth, halfWidth, -halfHeight, halfHeight, _nearClip, _farClip) :
		glm::orthoRH_NO(-halfWidth, halfWidth, -halfHeight, halfHeight, _nearClip, _farClip);
}

}
