//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: I_ViewControllerEventHandler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWCONTROLLEREVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWCONTROLLEREVENTHANDLER_HPP

#include "Apple/Types/Types.hpp"

class I_ViewControllerEventHandler {
public:
	virtual ~I_ViewControllerEventHandler() = default;

public:
	virtual void OnViewDidLoad(VoidEventCallback callback) = 0;

public:
	virtual void DispatchViewDidLoad() = 0;
};

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWCONTROLLEREVENTHANDLER_HPP
