//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: CameraProjectionFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Projection/I_CameraProjection.hpp"

namespace CE::Core {

std::unique_ptr<I_CameraProjection> I_CameraProjection::MakeCameraProjection(Types::CameraProjection) {
	return nullptr;
}

}
