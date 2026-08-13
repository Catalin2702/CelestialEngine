//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Dispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
#define CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Delegate/Delegate.hpp"

#include <vector>

namespace CE::Utility {

/**
 * @class UnicastDispatcher
 * @brief Single-listener event dispatcher
 * @tparam Args Argument types the bound delegate is invoked with
 * @details Holds exactly one EventDelegate: Bind refuses to overwrite an existing binding (logs a warning),
 *			Dispatch is a no-op while unbound. Used for the raw native event channels (view/window callbacks) that
 *			feed the event hub, where exactly one receiver is expected.
 */
template <typename... Args>
class CE_API UnicastDispatcher {
public:
	using DelegateType = EventDelegate<Args...>;

public:
	/**
	 * @brief Binds the listener delegate
	 * @param delegate The delegate to invoke on Dispatch
	 * @details Refuses (with a warning) if a delegate is already bound: Unbind first to rebind.
	 */
	void Bind(DelegateType delegate) {
		if (not IsBound())
			_delegate = delegate;
		else {
			CE_CORE_WARN("A delegate is already assigned");
			return;
		}
	}

	/**
	 * @brief Unbinds the current listener (or replaces it)
	 * @param delegate Optional replacement delegate; default-constructed (invalid) clears the binding
	 */
	void Unbind(const DelegateType& delegate = {}) {
		_delegate = delegate.IsValid() ? delegate : DelegateType{};
	}

	/**
	 * @brief Invokes the bound delegate with the given arguments
	 * @details No-op while unbound.
	 */
	void Dispatch(Args... args) const {
		if (not IsBound())
			return;

		_delegate(args...);
	}

public:
	/**
	 * @brief Checks whether a listener is currently bound
	 */
	[[nodiscard]] bool IsBound() const { return _delegate.IsValid(); }

private:
	DelegateType _delegate;							///< The single bound listener
};

/**
 * @class CallbackDispatcher
 * @brief Single-listener dispatcher for callbacks that return a value
 * @tparam R Return type of the bound callback
 * @tparam Args Argument types the bound callback is invoked with
 * @details Unlike UnicastDispatcher, Execute must produce a result: invoking it while unbound throws instead of
 *			silently doing nothing.
 */
template <typename R, typename... Args>
class CE_API CallbackDispatcher {
public:
	using CallbackType = CallbackDelegate<R, Args...>;

public:
	/**
	 * @brief Binds the callback delegate
	 * @param callback The callback to invoke on Execute
	 * @details Refuses (with a warning) if a callback is already bound: Unbind first to rebind.
	 */
	void Bind(CallbackType callback) {
		if (not IsBound())
			_callback = callback;
		else {
			CE_CORE_WARN("A delegate is already assigned");
			return;
		}
	}

	/**
	 * @brief Unbinds the current callback (or replaces it)
	 * @param callback Optional replacement callback; default-constructed (invalid) clears the binding
	 */
	void Unbind(const CallbackType& callback = {}) {
		_callback = callback.IsValid() ? callback : CallbackType{};
	}

	/**
	 * @brief Invokes the bound callback and returns its result
	 * @return R The callback's return value
	 * @details Throws std::runtime_error if no callback is bound: a missing result cannot be silently ignored.
	 */
	R Execute(Args... args) {
		if (not IsBound()) {
			constexpr auto error = "CallbackDispatcher::Execute: Set the callback before trying to execute this method.";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return _callback(args...);
	}

public:
	/**
	 * @brief Checks whether a callback is currently bound
	 */
	[[nodiscard]] bool IsBound() const { return _callback.IsValid(); }

private:
	CallbackType _callback;						///< The single bound callback
};

/**
 * @class MulticastDispatcher
 * @brief Multi-listener event dispatcher with reentrancy-safe subscription
 * @tparam Args Argument types the subscribed delegates are invoked with
 * @details Backbone of the event hub: every hub channel is a MulticastDispatcher that delivers each event to all
 *			subscribers in subscription order (which is why subscription order matters, e.g. the input state must
 *			subscribe first). Subscribe/Unsubscribe issued while a Dispatch is running are deferred and flushed at the
 *			end of it, so handlers may safely (un)subscribe from within a callback.
 */
template <typename... Args>
class CE_API MulticastDispatcher {
public:
	using Handle = uint32_t;
	using DelegateType = EventDelegate<Args...>;

public:
	/**
	 * @brief Adds a subscriber
	 * @param delegate The delegate to invoke on every Dispatch
	 * @return Handle Token identifying the subscription, needed to Unsubscribe
	 * @details Deferred to the end of the dispatch if called from within a handler.
	 */
	Handle Subscribe(DelegateType delegate) {
		const auto handle = _handle++;
		Entry entry{.handle = handle, .delegate = delegate};

		if (_isDispatching)
			_pendingAdds.push_back(entry);
		else
			_callbacks.push_back(entry);

		return handle;
	}

	/**
	 * @brief Removes a subscriber by its handle
	 * @param handle Token returned by Subscribe
	 * @details Deferred to the end of the dispatch if called from within a handler; unknown handles are ignored.
	 */
	void Unsubscribe(const Handle handle) {
		if (_isDispatching)
			_pendingRemoves.push_back(handle);
		else
			RemoveEntry(handle);
	}

	/**
	 * @brief Invokes every subscribed delegate, in subscription order, with the given arguments
	 * @details Subscriptions added during the dispatch are not invoked for the current event; pending
	 *			additions/removals are flushed once the iteration completes.
	 */
	void Dispatch(Args... args) {
		_isDispatching = true;
		const auto count = _callbacks.size();
		for (size_t i = 0; i < count; ++i) {
			_callbacks[i].delegate(args...);
		}
		_isDispatching = false;
		FlushPending();
	}

private:
	struct Entry {
		Handle handle;								///< Subscription token
		DelegateType delegate;						///< Subscriber to invoke
	};

	/**
	 * @brief Removes the entry with the given handle via swap-and-pop (order of remaining entries is preserved up to the swap)
	 */
	void RemoveEntry(const Handle handle) {
		auto it = std::find_if(_callbacks.begin(), _callbacks.end(), [handle](const Entry& entry) {
			return entry.handle == handle;
		});
		if (it != _callbacks.end()) {
			*it = _callbacks.back();
			_callbacks.pop_back();
		}
	}

	/**
	 * @brief Applies the (un)subscriptions deferred during the last dispatch
	 */
	void FlushPending() {
		for (const auto handle: _pendingRemoves)
			RemoveEntry(handle);
		_pendingRemoves.clear();

		for (const auto entry: _pendingAdds)
			_callbacks.push_back(entry);
		_pendingAdds.clear();
	}

private:
	std::vector<Entry> _callbacks;					///< Active subscribers, in subscription order
	std::vector<Entry> _pendingAdds;				///< Subscriptions requested during a dispatch, flushed after it
	std::vector<Handle> _pendingRemoves;			///< Unsubscriptions requested during a dispatch, flushed after it
	Handle _handle = 1;								///< Next subscription token to hand out
	bool _isDispatching = false;					///< True while Dispatch iterates: defers (un)subscriptions
};

}

template<typename... Args>
using UnicastDispatcher = CE::Utility::UnicastDispatcher<Args...>;

template<typename R, typename... Args>
using CallbackDispatcher = CE::Utility::CallbackDispatcher<R, Args...>;

template<typename... Args>
using MulticastDispatcher = CE::Utility::MulticastDispatcher<Args...>;

#endif //CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
