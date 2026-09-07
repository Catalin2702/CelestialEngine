//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Projection
// File: CameraProjectionFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#include "Core/Render/Camera/Projection/I_CameraProjection.hpp"
#include "Core/Render/Camera/Projection/CameraProjectionDescriptor.hpp"
#include "Core/Render/Camera/Projection/OrthographicCameraProjection.hpp"
#include "Tools/Tools.hpp"

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_CameraProjection> I_CameraProjection::MakeCameraProjection(const CameraProjectionDescriptor& descriptor) {
	const auto type = descriptor.type;
	std::string error{};
	switch (type) {
		case Types::CameraProjection::None: {
			error = "I_CameraProjection::MakeCameraProjection: The camera projection type is None!";
			break;
		}
		case Types::CameraProjection::Orthographic:
			return std::make_unique<OrthographicCameraProjection>(descriptor);
		case Types::CameraProjection::Perspective:
		default: {
			error = std::format("I_CameraProjection::MakeCameraProjection: The {} camera projection type is not yet been implemented!", type);
			break;
		}
	}
	CE_CORE_ERROR(error);
	throw std::runtime_error(error);
}

}
