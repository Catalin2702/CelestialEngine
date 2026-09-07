//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: OrthographicCameraProjection.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_PROJECTION_ORTHOGRAPHICCAMERAPROJECTION_HPP
#define CE_CORE_RENDER_CAMERA_PROJECTION_ORTHOGRAPHICCAMERAPROJECTION_HPP

#include "Core/Render/Camera/Projection/I_CameraProjection.hpp"


namespace CE::Core {

class CE_API_EXPORT OrthographicCameraProjection final: public I_CameraProjection {
public:
	OrthographicCameraProjection(const CameraProjectionDescriptor& descriptor);

	OrthographicCameraProjection(const OrthographicCameraProjection&) = delete;
	OrthographicCameraProjection(OrthographicCameraProjection&&) noexcept = default;

public:
	void SetViewportSize(const u32 width, const u32 height) override { _width = width; _height = height; }
	void SetNearClip(const f32 nearClip) override { _nearClip = nearClip; }
	void SetFarClip(const f32 farClip) override { _farClip = farClip; }
	void SetClipConvention(const Types::ClipConvention convention) override { _convention = convention; }

public:
	[[nodiscard]] Types::CameraProjection GetProjection() const override { return Types::CameraProjection::Orthographic; }
	[[nodiscard]] const glm::mat4& GetProjectionMatrix() const override;
	[[nodiscard]] f32 GetAspectRatio() const override;
	[[nodiscard]] f32 GetNearClip() const override { return _nearClip; }
	[[nodiscard]] f32 GetFarClip() const override { return _farClip; }

private:
	void _Rebuild() const;

private:
	Types::ClipConvention _convention;

	f32 _nearClip;
	f32 _farClip;

	u32 _width;
	u32 _height;

	f32 _orthographicSize;

	mutable glm::mat4 _projection{1.0_f32};
	mutable bool _dirty = true;
};

}

#endif //CE_CORE_RENDER_CAMERA_PROJECTION_ORTHOGRAPHICCAMERAPROJECTION_HPP
