//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Dispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
#define CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Delegate/Delegate.hpp"

#include <vector>

namespace CE::Utility {

template <typename R, typename... Args>
class CE_API UnicastDispatcher {
public:
	using CallbackType = CallbackDelegate<R, Args...>;

public:
	void Bind(CallbackType delegate) {
		if (not IsBound())
			_callback = delegate;
		else {
			const auto message = "A delegate is already assigned";
			CE_CORE_ERROR(message);
		}
	}

	void Unbind(const CallbackType& delegate = {}) {
		if (delegate)
			_callback = delegate;
		else {
			_callback._context = nullptr;
			_callback._stub = nullptr;
		}
	}

	void Dispatch(Args... args) const {
		_callback(args...);
	}

public:
	[[nodiscard]] bool IsBound() const { return _callback.IsValid(); }

private:
	CallbackType _callback;
};

template <typename... Args>
class CE_API MulticastDispatcher {
public:
	using Handle = uint32_t;
	using DelegateType = EventDelegate<Args...>;

public:
	Handle Subscribe(DelegateType delegate) {
		const auto handle = _handle++;
		Entry entry{handle, delegate};

		if (_isDispatching)
			_pendingAdds.push_back(entry);
		else
			_callbacks.push_back(entry);

		return handle;
	}

	void Unsubscribe(const Handle handle) {
		if (_isDispatching)
			_pendingRemoves.push_back(handle);
		else
			RemoveEntry(handle);
	}

	void Dispatch(Args... args) {
		_isDispatching = true;
		const auto count = _callbacks.size();
		for (size_t i = 0; i < count; ++i) {
			_callbacks[i].delegate(args...);
		}
		FlushPending();
	}

private:
	struct Entry {
		Handle handle;
		DelegateType delegate;
	};

	void RemoveEntry(const Handle handle) {
		auto it = std::find_if(_callbacks.begin(), _callbacks.end(), [handle](const Entry& entry) {
			return entry.handle == handle;
		});
		if (it != _callbacks.end()) {
			*it = _callbacks.back();
			_callbacks.pop_back();
		}
	}

	void FlushPending() {
		for (const auto handle: _pendingRemoves)
			RemoveEntry(handle);
		_pendingRemoves.clear();

		for (const auto entry: _pendingAdds)
			_callbacks.push_back(entry);
		_pendingAdds.clear();
	}

private:
	std::vector<Entry> _callbacks;
	std::vector<Entry> _pendingAdds;
	std::vector<Handle> _pendingRemoves;
	Handle _handle = 1;
	bool _isDispatching = false;
};

}

template<typename... Args>
using UnicastEventDispatcher = CE::Utility::UnicastDispatcher<void, Args...>;

using VoidEventDelegate = EventDelegate<void>;
using VoidUnicastEventDispatcher = CE::Utility::UnicastDispatcher<void, void>;
using VoidMulticastEventDispatcher = CE::Utility::MulticastDispatcher<>;

using BoolEventDelegate = EventDelegate<bool>;
using BoolUnicastEventDispatcher = CE::Utility::UnicastDispatcher<void, bool>;
using BoolMulticastEventDispatcher = CE::Utility::MulticastDispatcher<bool>;

#endif //CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
