//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: I_DisplayLinkEventHandler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-20
// Updated by: Catalin Chirosca
// Updated: 2026-05-20
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_DISPLAYLINKEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_DISPLAYLINKEVENTHANDLER_HPP

#include "Types/Types.hpp"

class I_DisplayLinkEventHandler {
public:
	virtual ~I_DisplayLinkEventHandler() = default;

public:
	virtual void OnTick(VoidEventCallback callback) = 0;

	virtual void DispatchTick() = 0;
};

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_DISPLAYLINKEVENTHANDLER_HPP
