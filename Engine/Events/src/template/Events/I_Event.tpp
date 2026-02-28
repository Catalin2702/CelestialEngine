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
