//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_EventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_EVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_EVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "I_ApplicationEventHubDispatcher.hpp"
#include "I_KeyboardEventHubDispatcher.hpp"
#include "I_MouseEventHubDispatcher.hpp"

namespace CE::Core {

class CE_API I_EventHubDispatcher:
	public I_ApplicationEventHubDispatcher,
	public I_KeyboardEventHubDispatcher,
	public I_MouseEventHubDispatcher
{
public:
	~I_EventHubDispatcher() override = default;
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_EVENTHUBDISPATCHER_HPP
