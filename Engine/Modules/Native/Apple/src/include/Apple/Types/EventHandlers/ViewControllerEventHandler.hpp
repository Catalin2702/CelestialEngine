//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: ViewControllerEventHandler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWCONTROLLEREVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWCONTROLLEREVENTHANDLER_HPP

#include "Apple/Types/Types.hpp"
#include "Apple/Types/EventHandlers/I_ViewControllerEventHandler.hpp"

namespace CE::Apple::Types {

class ViewControllerEventHandler final: public I_ViewControllerEventHandler {
public:
	void OnViewDidLoad(VoidEventCallback callback) override;

public:
	void DispatchViewDidLoad() override;

private:
	VoidEventCallback _viewDidLoadCallback;
};

}


#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWCONTROLLEREVENTHANDLER_HPP
