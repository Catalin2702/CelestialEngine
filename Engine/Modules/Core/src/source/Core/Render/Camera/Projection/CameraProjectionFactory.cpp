//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: CameraProjectionFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Projection/I_CameraProjection.hpp"
#include "Core/Render/Camera/Projection/CameraProjectionDescriptor.hpp"
#include "Core/Render/Camera/Projection/OrthographicCameraProjection.hpp"

namespace CE::Core {

std::unique_ptr<I_CameraProjection> I_CameraProjection::MakeCameraProjection(const CameraProjectionDescriptor& descriptor) {
	switch (descriptor.type) {
		case Types::CameraProjection::Orthographic:
			return std::make_unique<OrthographicCameraProjection>(descriptor);
		case Types::CameraProjection::Perspective:
		default:
			return nullptr;
	}
}

}
