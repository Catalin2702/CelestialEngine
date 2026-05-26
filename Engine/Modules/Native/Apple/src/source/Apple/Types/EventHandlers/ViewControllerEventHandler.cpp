//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: ViewControllerEventHandler.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Apple/Types/EventHandlers/ViewControllerEventHandler.hpp"

void ViewControllerEventHandler::OnViewDidLoad(VoidEventCallback callback) {
	_viewDidLoadCallback = std::move(callback);
}

void ViewControllerEventHandler::DispatchViewDidLoad() {
	if (_viewDidLoadCallback) {
		_viewDidLoadCallback();
	}
}
