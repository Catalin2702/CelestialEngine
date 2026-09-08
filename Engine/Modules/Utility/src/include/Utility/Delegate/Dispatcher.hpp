//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Dispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
#define CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP

#include "Tools/Tools.hpp"
#include "Types/Var/Vars.hpp"
#include "Utility/Delegate/Delegate.hpp"

#include <cassert>
#include <utility>
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
class UnicastDispatcher {
public:
	using DelegateType = EventDelegate<Args...>;

public:
	// Declared explicitly rather than left implicit: the dispatchers are held by value inside movable owners
	// (GlfwWindow, CocoaWindow, ...), and a move that is not noexcept would silently degrade into a copy inside
	// std::vector and friends. Copies stay available - a dispatcher is just a delegate holder, cheap and trivially copyable.
	UnicastDispatcher() = default;
	UnicastDispatcher(const UnicastDispatcher&) = default;
	UnicastDispatcher(UnicastDispatcher&&) noexcept = default;
	~UnicastDispatcher() = default;

	UnicastDispatcher& operator = (const UnicastDispatcher&) = default;
	UnicastDispatcher& operator = (UnicastDispatcher&&) noexcept = default;

public:
	/**
	 * @brief Binds the listener delegate
	 * @param delegate The delegate to invoke on Dispatch
	 * @details Refuses (with a warning) if a delegate is already bound: Unbind first to rebind.
	 */
	void Bind(DelegateType delegate) {
		if (not IsBound()) [[likely]]
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
		if (not IsBound()) [[unlikely]]
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
class CallbackDispatcher {
public:
	using CallbackType = CallbackDelegate<R, Args...>;

public:
	CallbackDispatcher() = default;
	CallbackDispatcher(const CallbackDispatcher&) = default;
	CallbackDispatcher(CallbackDispatcher&&) noexcept = default;
	~CallbackDispatcher() = default;

	CallbackDispatcher& operator = (const CallbackDispatcher&) = default;
	CallbackDispatcher& operator = (CallbackDispatcher&&) noexcept = default;

public:
	/**
	 * @brief Binds the callback delegate
	 * @param callback The callback to invoke on Execute
	 * @details Refuses (with a warning) if a callback is already bound: Unbind first to rebind.
	 */
	void Bind(CallbackType callback) {
		if (not IsBound()) [[likely]]
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
		if (not IsBound()) [[unlikely]] {
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

template <typename... Args> class MulticastDispatcher;

/**
 * @class Subscription
 * @brief Owning token for one MulticastDispatcher subscription, which it cancels when it dies
 * @details Solves the problem a bare handle cannot: a u32 is meaningless without the dispatcher that issued it, so
 *			anything holding one has to keep a reference to that dispatcher too, and remember to unsubscribe before it
 *			is destroyed. A Subscription carries both halves, so the subscriber keeps neither - and a destructor,
 *			an early return or a thrown exception all release it without anybody writing the call.
 *
 *			The dispatcher is erased behind a void* and a thunk rather than baked into a template parameter, so
 *			subscriptions to channels with different signatures are all the same type and fit in one array. That is
 *			what the layers need: nine subscriptions across four event families, held in a single std::array.
 *
 *			Move-only, and a move empties the source: two live copies would each cancel on destruction, and the second
 *			would be cancelling a handle the dispatcher may already have reissued.
 *
 *			**The dispatcher must outlive the Subscription.** Nothing here can check that - the token holds a raw
 *			pointer, deliberately, because weak tracking would cost an allocation per subscription. In this engine the
 *			hub belongs to the Application and is destroyed after everything that subscribes to it, which is what
 *			makes the raw pointer safe.
 */
class Subscription {
public:
	Subscription() = default;

	/**
	 * @brief Adopts a handle already issued by a dispatcher
	 * @param dispatcher The dispatcher that issued the handle, which must outlive this token
	 * @param handle The token Subscribe returned
	 */
	template <typename... Args>
	Subscription(MulticastDispatcher<Args...>& dispatcher, const u32 handle):
		_dispatcher(&dispatcher),
		_handle(handle),
		_unsubscribe(+[](void* const target, const u32 subscriptionHandle) {
			static_cast<MulticastDispatcher<Args...>*>(target)->Unsubscribe(subscriptionHandle);
		}) {}

	Subscription(const Subscription&) = delete;
	Subscription& operator = (const Subscription&) = delete;

	Subscription(Subscription&& other) noexcept:
		_dispatcher(std::exchange(other._dispatcher, nullptr)),
		_handle(std::exchange(other._handle, 0)),
		_unsubscribe(std::exchange(other._unsubscribe, nullptr)) {}

	Subscription& operator = (Subscription&& other) noexcept {
		if (this == &other) [[unlikely]]
			return *this;

		// Releases what this token held before taking the other's: assigning over a live subscription must not leak it.
		Reset();

		_dispatcher = std::exchange(other._dispatcher, nullptr);
		_handle = std::exchange(other._handle, 0);
		_unsubscribe = std::exchange(other._unsubscribe, nullptr);

		return *this;
	}

	~Subscription() { Reset(); }

public:
	/**
	 * @brief Cancels the subscription now, instead of waiting for the destructor
	 * @details Safe to call on an empty token, and safe to call twice. Called from inside a handler it is deferred by
	 *			the dispatcher to the end of the dispatch, like any other Unsubscribe.
	 */
	void Reset() {
		if (_unsubscribe)
			_unsubscribe(_dispatcher, _handle);

		_dispatcher = nullptr;
		_handle = 0;
		_unsubscribe = nullptr;
	}

	/**
	 * @brief Checks whether this token still holds a live subscription
	 */
	[[nodiscard]] bool IsConnected() const { return _unsubscribe != nullptr; }

private:
	void* _dispatcher = nullptr;					///< The dispatcher that issued the handle, type erased
	u32 _handle = 0;								///< The token it issued; 0 is never handed out, so it means "empty"
	void (*_unsubscribe)(void*, u32) = nullptr;		///< Casts the dispatcher back and unsubscribes; null when empty
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
class MulticastDispatcher {
public:
	using Handle = u32;
	using DelegateType = EventDelegate<Args...>;

public:
	MulticastDispatcher() = default;
	MulticastDispatcher(const MulticastDispatcher&) = default;
	~MulticastDispatcher() = default;

	MulticastDispatcher& operator = (const MulticastDispatcher&) = default;

	// Written out instead of defaulted only for the assert: moving a dispatcher while Dispatch is walking _callbacks
	// would leave that loop iterating a moved-from vector. The handle counter is carried over (not reset) so tokens
	// handed out before the move never collide with tokens handed out after it.
	MulticastDispatcher(MulticastDispatcher&& other) noexcept:
		_callbacks(std::move(other._callbacks)),
		_pendingAdds(std::move(other._pendingAdds)),
		_pendingRemoves(std::move(other._pendingRemoves)),
		_handle(other._handle) {
		assert(not other._isDispatching && "MulticastDispatcher: moved while dispatching");
	}

	MulticastDispatcher& operator = (MulticastDispatcher&& other) noexcept {
		if (this == &other) [[unlikely]]
			return *this;

		assert(not _isDispatching && not other._isDispatching && "MulticastDispatcher: moved while dispatching");

		_callbacks = std::move(other._callbacks);
		_pendingAdds = std::move(other._pendingAdds);
		_pendingRemoves = std::move(other._pendingRemoves);
		_handle = other._handle;

		return *this;
	}

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

		if (_isDispatching) [[unlikely]]
			_pendingAdds.push_back(entry);
		else
			_callbacks.push_back(entry);

		return handle;
	}

	/**
	 * @brief Adds a subscriber and hands back a token that cancels it on destruction
	 * @param delegate The delegate to invoke on every Dispatch
	 * @return Subscription Owning token; letting it die unsubscribes
	 * @details Preferred over Subscribe wherever the subscription has an owner with a lifetime - which is every
	 *			subscriber in the engine. The raw Subscribe stays for the cases that genuinely outlive their holder.
	 */
	[[nodiscard]] Subscription SubscribeScoped(DelegateType delegate) {
		return Subscription{*this, Subscribe(delegate)};
	}

	/**
	 * @brief Removes a subscriber by its handle
	 * @param handle Token returned by Subscribe
	 * @details Deferred to the end of the dispatch if called from within a handler; unknown handles are ignored.
	 */
	void Unsubscribe(const Handle handle) {
		if (_isDispatching) [[unlikely]]
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
		if (it != _callbacks.end()) [[likely]] {
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

using Subscription = CE::Utility::Subscription;

#endif //CE_UTILITY_CALLBACK_EVENTDISPATCHER_HPP
