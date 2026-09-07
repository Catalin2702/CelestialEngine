//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: CameraControllerFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Controller/I_CameraController.hpp"

namespace CE::Core {

std::unique_ptr<I_CameraController> I_CameraController::MakeCameraController(Types::CameraController) {
	return nullptr;
}

}
