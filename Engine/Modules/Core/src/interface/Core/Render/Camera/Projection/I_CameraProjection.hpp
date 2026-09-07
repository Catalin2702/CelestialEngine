//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: I_CameraProjection.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_PROJECTION_I_CAMERAPROJECTION_HPP
#define CE_CORE_RENDER_CAMERA_PROJECTION_I_CAMERAPROJECTION_HPP

#include "Types/Types.hpp"

#include <glm/glm.hpp>

#include <memory>


namespace CE::Core {

class I_CameraProjection {
public:
	virtual ~I_CameraProjection() = default;

public:
	[[nodiscard]] static std::unique_ptr<I_CameraProjection> MakeCameraProjection(Types::CameraProjection projection);

public:
	virtual void SetViewportSize(u32 width, u32 height) = 0;
	virtual void SetNearClip(f32 nearClip) = 0;
	virtual void SetFarClip(f32 farClip) = 0;
	virtual void SetClipConvention(Types::ClipConvention convention) = 0;

public:
	[[nodiscard]] virtual Types::CameraProjection GetProjection() const = 0;
	[[nodiscard]] virtual const glm::mat4& GetProjectionMatrix() const = 0;
	[[nodiscard]] virtual f32 GetAspectRatio() const = 0;
	[[nodiscard]] virtual f32 GetNearClip() const = 0;
	[[nodiscard]] virtual f32 GetFarClip() const = 0;
};

}

#endif //CE_CORE_RENDER_CAMERA_PROJECTION_I_CAMERAPROJECTION_HPP
