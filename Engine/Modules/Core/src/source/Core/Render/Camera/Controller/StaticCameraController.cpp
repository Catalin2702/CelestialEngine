//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: StaticCameraController.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Core/Render/Camera/Controller/StaticCameraController.hpp"
#include "Events/I_Event.hpp"


namespace CE::Core {

StaticCameraController::StaticCameraController(const CameraControllerDescriptor&) {}

void StaticCameraController::OnAttach(Camera&) {}

void StaticCameraController::OnDetach() {}

void StaticCameraController::OnUpdate(Camera&, f32) {}

}
