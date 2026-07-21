//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_RenderContextEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/RenderEvent.hpp"

namespace CE::Core {

class CE_API I_RenderContextEventHubDispatcher {
public:
	virtual ~I_RenderContextEventHubDispatcher() = default;

public:
	virtual void DispatchRenderContextChangeVSyncEvent(Events::VSyncChangeEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP
