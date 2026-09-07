//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: CameraProjectionDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_PROJECTION_CAMERAPROJECTIONDESCRIPTOR_HPP
#define CE_CORE_RENDER_CAMERA_PROJECTION_CAMERAPROJECTIONDESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace CE::Core {

struct CE_CORE_API CameraProjectionDescriptor {
	Types::CameraProjection type = Types::CameraProjection::None;
	Types::ClipConvention convention = Types::ClipConvention::None;

	f32 nearClip = 0.1;
	f32 farClip = 1000.0;

	u32 viewportWidth = 1280;
	u32 viewportHeight = 720;

	/// Perspective only: vertical field of view, in degrees.
	f32 fieldOfView = 60.0;

	/// Orthographic only: half-height of the visible box, in world units.
	f32 orthographicSize = 10.0;
};

}

#endif //CE_CORE_RENDER_CAMERA_PROJECTION_CAMERAPROJECTIONDESCRIPTOR_HPP
