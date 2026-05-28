//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: DisplayLinkEventHandler.hpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-28
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_DISPLAYLINKEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_DISPLAYLINKEVENTHANDLER_HPP

#include "Apple/Types/EventHandlers/I_DisplayLinkEventHandler.hpp"

namespace CE::Apple::Types {

class DisplayLinkEventHandler final: public I_DisplayLinkEventHandler {
public:
	void OnTick(VoidEventCallback callback) override;

public:
	void DispatchTick() override;

private:
	VoidEventCallback _tickCallback;
};

}

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_DISPLAYLINKEVENTHANDLER_HPP
