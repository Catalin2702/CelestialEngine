//
// Module: Events
// File: I_Event.tpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

namespace CE::Events {

template<typename T>
bool EventDispatcher::Dispatch(EventFn<T> func) {
	if (_event.GetEventType() == T::GetStaticType()) {
		_event._handled |= func(static_cast<T&>(_event));
		return true;
	}
	return false;
}

}
