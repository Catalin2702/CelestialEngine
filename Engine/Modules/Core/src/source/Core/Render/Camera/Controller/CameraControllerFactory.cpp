//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: CameraControllerFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Controller/CameraControllerDescriptor.hpp"
#include "Core/Render/Camera/Controller/I_CameraController.hpp"
#include "Core/Render/Camera/Controller/StaticCameraController.hpp"
#include "Tools/Tools.hpp"

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_CameraController> I_CameraController::MakeCameraController(const CameraControllerDescriptor& descriptor) {
	const auto type = descriptor.GetController();
	std::string error{};
	switch (type) {
		case Types::CameraController::None: {
			error = "I_CameraController::MakeCameraController: The camera controller type is None!";
			break;
		}
		case Types::CameraController::Static:
			return std::make_unique<StaticCameraController>(descriptor);
		default: {
			error = std::format("I_CameraController::MakeCameraController: The {} camera controller type has not yet been implemented!", type);
			break;
		}
	}
	CE_CORE_ERROR(error);
	throw std::runtime_error(error);
}

}
