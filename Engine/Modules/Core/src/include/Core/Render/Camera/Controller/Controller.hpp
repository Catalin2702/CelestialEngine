//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: Controller.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Controller.hpp
 * @brief Umbrella header for the camera controllers
 * @details One include for the controller interface, the descriptors every controller is built from, the shared
 *			mouse-look component and the controllers implemented so far.
 */

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_CONTROLLER_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_CONTROLLER_HPP

#include "Core/Render/Camera/Controller/I_CameraController.hpp"

#include "Core/Render/Camera/Controller/CameraControllerDescriptor.hpp"
#include "Core/Render/Camera/Controller/FreeCameraController.hpp"
#include "Core/Render/Camera/Controller/MouseLook.hpp"
#include "Core/Render/Camera/Controller/StaticCameraController.hpp"

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_CONTROLLER_HPP
