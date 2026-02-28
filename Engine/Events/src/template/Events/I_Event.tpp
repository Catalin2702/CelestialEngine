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

/**
 * @brief Template implementation of EventDispatcher::Dispatch method
 * @tparam T The concrete event type to dispatch to
 * @param func Handler function that takes a reference to the event type
 * @return bool True if the event type matched and was dispatched, false otherwise
 * @details Checks if the stored event is of the specified type T by comparing
 *          the event's runtime type with T's static type. If they match, the event
 *          is safely cast to type T and passed to the handler function. The event's
 *          handled flag is updated with a bitwise OR to preserve any previous handled state.
 */
template<typename T>
bool EventDispatcher::Dispatch(EventFn<T> func) {
	if (_event.GetEventType() == T::GetStaticType()) {
		_event._handled |= func(static_cast<T&>(_event));
		return true;
	}
	return false;
}

}
