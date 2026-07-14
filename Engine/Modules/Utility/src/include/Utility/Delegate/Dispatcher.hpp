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

template <typename... Args>
class CE_API UnicastDispatcher {
public:
	using DelegateType = EventDelegate<Args...>;

public:
	void Bind(DelegateType delegate) {
		if (not IsBound())
			_delegate = delegate;
		else {
			const auto message = "A delegate is already assigned";
			CE_CORE_ERROR(message);
		}
	}

	void Unbind(const DelegateType& delegate = {}) {
		if (delegate)
			_delegate = delegate;
		else {
			_delegate._context = nullptr;
			_delegate._stub = nullptr;
		}
	}

	void Dispatch(Args... args) const {
		if (not IsBound()) {
			const auto error = "UnicastDispatcher::Dispatch: Set the delegate before trying to execute this method.";
			CE_CORE_WARN(error);
			return;
		}
		_delegate(args...);
	}

public:
	[[nodiscard]] bool IsBound() const { return _delegate.IsValid(); }

private:
	DelegateType _delegate;
};

template <typename R, typename... Args>
class CE_API CallbackDispatcher {
public:
	using CallbackType = CallbackDelegate<R, Args...>;

public:
	void Bind(CallbackType callback) {
		if (not IsBound())
			_callback = callback;
		else {
			const auto message = "A delegate is already assigned";
			CE_CORE_ERROR(message);
		}
	}

	void Unbind(const CallbackType& callback = {}) {
		if (callback)
			_callback = callback;
		else {
			_callback._context = nullptr;
			_callback._stub = nullptr;
		}
	}

	R Execute(Args... args) {
		if (not IsBound()) {
			const auto error = "CallbackDispatcher::Execute: Set the callback before trying to execute this method.";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return _callback(args...);
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
using UnicastDispatcher = CE::Utility::UnicastDispatcher<Args...>;

template<typename R, typename... Args>
using CallbackDispatcher = CE::Utility::CallbackDispatcher<R, Args...>;

template<typename... Args>
using MulticastDispatcher = CE::Utility::MulticastDispatcher<Args...>;

#endif //CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
