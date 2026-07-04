//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Dispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
#define CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include <vector>

namespace CE::Utility {

template <typename... Args>
class Delegate {
public:
	using Stub = void(*)(void*, Args...);

	Delegate() = default;

public:
	template<void (*Fn)(Args...)>
	static Delegate FromFunction() {
		Delegate delegate;
		delegate._context = nullptr;
		delegate._stub = &FunctionStub<Fn>;
		return delegate;
	}

	template<typename T, void (T::*Method)(Args...)>
	static Delegate FromMethod(T* instance) {
		Delegate delegate;
		delegate._context = instance;
		delegate._stub = &MethodStub<T, Method>;
		return delegate;
	}

	template<typename T, void(T::*Method)(Args...) const>
	static Delegate FromConstMethod(const T* instance) {
		Delegate delegate;
		delegate._context = const_cast<T*>(instance);
		delegate._stub = &ConstMethodStub<T, Method>;
		return delegate;
	}

public:
	[[nodiscard]] bool IsValid() const { return _stub != nullptr; }

public:
	void operator()(Args... args) const { (*_stub)(_context, args...); }

	bool operator==(const Delegate& other) const { return this->_context == other._context and this->_stub == other._stub; }

private:
	template<void (*Fn)(Args...)>
	static void FunctionStub(void*, Args... args) {
		Fn(args...);
	}

	template<typename T, void (T::*Method)(Args...)>
	static void MethodStub(void* context, Args... args) {
		(static_cast<T*>(context)->*Method)(args...);
	}

	template<typename T, void(T::*Method)(Args...) const>
	static void ConstMethodStub(void* context, Args... args) {
		(static_cast<const T*>(context)->*Method)(args...);
	}

private:
	void* _context = nullptr;
	Stub _stub = nullptr;
};

template <typename... Args>
class CE_API Dispatcher {
public:
	using Handle = uint32_t;
	using DelegateType = Delegate<Args...>;

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

#endif //CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
