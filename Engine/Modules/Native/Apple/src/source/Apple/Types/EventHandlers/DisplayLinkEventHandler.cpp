//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: DisplayLinkEventHandler.cpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Apple/Types/EventHandlers/DisplayLinkEventHandler.hpp"
#include "Apple/Types/Types.hpp"

namespace CE::Apple::Types {

void DisplayLinkEventHandler::OnTick(VoidEventCallback callback) {
	_tickCallback = std::move(callback);
}

void DisplayLinkEventHandler::DispatchTick() {
	if (_tickCallback)
		_tickCallback();
}

}
