//
// Module: CelestialEngine/Tests/Engine/Modules/Utility/Delegate
// File: DispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Utility/Delegate/Dispatcher.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

namespace {

/**
 * @brief Counting listener used as dispatcher subscriber
 */
class Listener {
public:
	explicit Listener(std::vector<int>* order = nullptr, const int id = 0): _order(order), _id(id) {}

	void OnEvent() {
		++calls;
		if (_order)
			_order->push_back(_id);
	}

	void OnValue(const int value) {
		++calls;
		lastValue = value;
	}

	[[nodiscard]] int Compute(const int value) const { return value * 2; }

public:
	int calls = 0;
	int lastValue = 0;

private:
	std::vector<int>* _order;						///< Optional shared log recording the invocation order
	int _id;										///< Identifier pushed into the invocation order log
};

}

namespace {

/**
 * @brief Test fixture for dispatcher tests
 * @details The dispatchers log through the engine logger (e.g. when rebinding an already bound delegate), so the
 *			logger must be alive for the whole test.
 */
class DispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

// ============================================================================
// UnicastDispatcher Tests
// ============================================================================

/**
 * @brief Test that a fresh unicast dispatcher has no listener
 */
TEST_F(DispatcherTest, Unicast_DefaultConstructed_IsNotBound) {
	constexpr UnicastDispatcher<> dispatcher;

	EXPECT_FALSE(dispatcher.IsBound());
}

/**
 * @brief Test that Bind attaches a listener and Dispatch invokes it
 */
TEST_F(DispatcherTest, Unicast_Bind_InvokesListenerOnDispatch) {
	Listener listener;
	UnicastDispatcher<> dispatcher;

	dispatcher.Bind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&listener));

	EXPECT_TRUE(dispatcher.IsBound());

	dispatcher.Dispatch();
	dispatcher.Dispatch();

	EXPECT_EQ(listener.calls, 2);
}

/**
 * @brief Test that dispatching forwards the arguments
 */
TEST_F(DispatcherTest, Unicast_Dispatch_ForwardsArguments) {
	Listener listener;
	UnicastDispatcher<int> dispatcher;

	dispatcher.Bind(EventDelegate<int>::FromMethod<Listener, &Listener::OnValue>(&listener));
	dispatcher.Dispatch(123);

	EXPECT_EQ(listener.lastValue, 123);
}

/**
 * @brief Test that dispatching while unbound is a no-op
 */
TEST_F(DispatcherTest, Unicast_Dispatch_WhileUnbound_IsNoOp) {
	constexpr UnicastDispatcher<int> dispatcher;

	EXPECT_NO_THROW(dispatcher.Dispatch(1));
}

/**
 * @brief Test that Bind refuses to overwrite an already bound listener
 * @details The unicast dispatcher models a single owner of a native callback: a second Bind is a programming error
 *			and is rejected (with a warning) rather than silently stealing the slot.
 */
TEST_F(DispatcherTest, Unicast_Bind_Twice_KeepsFirstListener) {
	Listener first;
	Listener second;
	UnicastDispatcher<> dispatcher;

	dispatcher.Bind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&first));
	dispatcher.Bind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&second));

	dispatcher.Dispatch();

	EXPECT_EQ(first.calls, 1);
	EXPECT_EQ(second.calls, 0);
}

/**
 * @brief Test that Unbind clears the listener
 */
TEST_F(DispatcherTest, Unicast_Unbind_ClearsListener) {
	Listener listener;
	UnicastDispatcher<> dispatcher;

	dispatcher.Bind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&listener));
	dispatcher.Dispatch();

	dispatcher.Unbind();

	EXPECT_FALSE(dispatcher.IsBound());

	dispatcher.Dispatch();

	EXPECT_EQ(listener.calls, 1);
}

/**
 * @brief Test that Unbind with a replacement rebinds instead of clearing
 */
TEST_F(DispatcherTest, Unicast_Unbind_WithReplacement_Rebinds) {
	Listener first;
	Listener second;
	UnicastDispatcher<> dispatcher;

	dispatcher.Bind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&first));
	dispatcher.Unbind(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&second));

	EXPECT_TRUE(dispatcher.IsBound());

	dispatcher.Dispatch();

	EXPECT_EQ(first.calls, 0);
	EXPECT_EQ(second.calls, 1);
}

// ============================================================================
// CallbackDispatcher Tests
// ============================================================================

/**
 * @brief Test that a fresh callback dispatcher has no callback
 */
TEST_F(DispatcherTest, Callback_DefaultConstructed_IsNotBound) {
	constexpr CallbackDispatcher<int, int> dispatcher;

	EXPECT_FALSE(dispatcher.IsBound());
}

/**
 * @brief Test that Execute returns the callback's result
 */
TEST_F(DispatcherTest, Callback_Execute_ReturnsResult) {
	const Listener listener;
	CallbackDispatcher<int, int> dispatcher;

	dispatcher.Bind(CallbackDelegate<int, int>::FromConstMethod<Listener, &Listener::Compute>(&listener));

	EXPECT_TRUE(dispatcher.IsBound());
	EXPECT_EQ(dispatcher.Execute(21), 42);
}

/**
 * @brief Test that executing an unbound callback throws
 * @details Unlike a fire-and-forget event, a missing result cannot be silently ignored.
 */
TEST_F(DispatcherTest, Callback_Execute_WhileUnbound_Throws) {
	CallbackDispatcher<int, int> dispatcher;

	EXPECT_THROW(dispatcher.Execute(1), std::runtime_error);
}

/**
 * @brief Test that Unbind clears the callback
 */
TEST_F(DispatcherTest, Callback_Unbind_ClearsCallback) {
	Listener listener;
	CallbackDispatcher<int, int> dispatcher;

	dispatcher.Bind(CallbackDelegate<int, int>::FromConstMethod<Listener, &Listener::Compute>(&listener));
	dispatcher.Unbind();

	EXPECT_FALSE(dispatcher.IsBound());
	EXPECT_THROW(dispatcher.Execute(1), std::runtime_error);
}

// ============================================================================
// MulticastDispatcher Tests
// ============================================================================

/**
 * @brief Test that every subscriber receives the dispatch
 */
TEST_F(DispatcherTest, Multicast_Dispatch_NotifiesEverySubscriber) {
	Listener first;
	Listener second;
	MulticastDispatcher<> dispatcher;

	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&first));
	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&second));

	dispatcher.Dispatch();

	EXPECT_EQ(first.calls, 1);
	EXPECT_EQ(second.calls, 1);
}

/**
 * @brief Test that subscribers are notified in subscription order
 * @details Order matters in the engine: the input state subscribes first so that layers reading the input during
 *			their own handler already see the updated state.
 */
TEST_F(DispatcherTest, Multicast_Dispatch_PreservesSubscriptionOrder) {
	std::vector<int> order;
	Listener first{&order, 1};
	Listener second{&order, 2};
	Listener third{&order, 3};
	MulticastDispatcher<> dispatcher;

	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&first));
	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&second));
	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&third));

	dispatcher.Dispatch();

	EXPECT_EQ(order, (std::vector{1, 2, 3}));
}

/**
 * @brief Test that dispatching without subscribers is a no-op
 */
TEST_F(DispatcherTest, Multicast_Dispatch_WithoutSubscribers_IsNoOp) {
	MulticastDispatcher<int> dispatcher;

	EXPECT_NO_THROW(dispatcher.Dispatch(1));
}

/**
 * @brief Test that Subscribe hands out distinct handles
 */
TEST_F(DispatcherTest, Multicast_Subscribe_ReturnsDistinctHandles) {
	Listener listener;
	MulticastDispatcher<> dispatcher;

	const auto first = dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&listener));
	const auto second = dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&listener));

	EXPECT_NE(first, second);
}

/**
 * @brief Test that an unsubscribed listener stops receiving dispatches
 */
TEST_F(DispatcherTest, Multicast_Unsubscribe_StopsNotifyingListener) {
	Listener kept;
	Listener removed;
	MulticastDispatcher<> dispatcher;

	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&kept));
	const auto handle = dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&removed));

	dispatcher.Unsubscribe(handle);
	dispatcher.Dispatch();

	EXPECT_EQ(kept.calls, 1);
	EXPECT_EQ(removed.calls, 0);
}

/**
 * @brief Test that unsubscribing the first of several subscribers leaves the others attached
 */
TEST_F(DispatcherTest, Multicast_Unsubscribe_FirstSubscriber_KeepsOthers) {
	Listener first;
	Listener second;
	Listener third;
	MulticastDispatcher<> dispatcher;

	const auto handle = dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&first));
	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&second));
	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&third));

	dispatcher.Unsubscribe(handle);
	dispatcher.Dispatch();

	EXPECT_EQ(first.calls, 0);
	EXPECT_EQ(second.calls, 1);
	EXPECT_EQ(third.calls, 1);
}

/**
 * @brief Test that unsubscribing an unknown handle is ignored
 */
TEST_F(DispatcherTest, Multicast_Unsubscribe_UnknownHandle_IsIgnored) {
	Listener listener;
	MulticastDispatcher<> dispatcher;

	dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&listener));

	EXPECT_NO_THROW(dispatcher.Unsubscribe(9999));

	dispatcher.Dispatch();

	EXPECT_EQ(listener.calls, 1);
}

/**
 * @brief Test that arguments are forwarded to every subscriber
 */
TEST_F(DispatcherTest, Multicast_Dispatch_ForwardsArguments) {
	Listener first;
	Listener second;
	MulticastDispatcher<int> dispatcher;

	dispatcher.Subscribe(EventDelegate<int>::FromMethod<Listener, &Listener::OnValue>(&first));
	dispatcher.Subscribe(EventDelegate<int>::FromMethod<Listener, &Listener::OnValue>(&second));

	dispatcher.Dispatch(7);

	EXPECT_EQ(first.lastValue, 7);
	EXPECT_EQ(second.lastValue, 7);
}

/**
 * @brief Test that subscriptions made from inside a handler are deferred to the next dispatch
 * @details Reentrancy safety: the subscriber vector must not be mutated while it is being iterated.
 */
TEST_F(DispatcherTest, Multicast_SubscribeDuringDispatch_IsDeferred) {
	Listener added;
	MulticastDispatcher<> dispatcher;

	struct Subscriber {
		MulticastDispatcher<>* dispatcher = nullptr;
		Listener* added = nullptr;

		void OnEvent() {
			if (subscribed)
				return;

			subscribed = true;
			dispatcher->Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(added));
		}

		bool subscribed = false;
	} subscriber{.dispatcher = &dispatcher, .added = &added};

	dispatcher.Subscribe(EventDelegate<>::FromMethod<Subscriber, &Subscriber::OnEvent>(&subscriber));

	dispatcher.Dispatch();
	EXPECT_EQ(added.calls, 0);	// added during the dispatch: not invoked for that event

	dispatcher.Dispatch();
	EXPECT_EQ(added.calls, 1);	// flushed after the first dispatch: invoked from the next one on
}

/**
 * @brief Test that unsubscriptions made from inside a handler are applied after the dispatch
 */
TEST_F(DispatcherTest, Multicast_UnsubscribeDuringDispatch_IsDeferred) {
	Listener target;
	MulticastDispatcher<> dispatcher;
	const auto handle = dispatcher.Subscribe(EventDelegate<>::FromMethod<Listener, &Listener::OnEvent>(&target));

	const struct Unsubscriber {
		MulticastDispatcher<>* dispatcher = nullptr;
		u32 handle = 0;

		void OnEvent() const { dispatcher->Unsubscribe(handle); }
	} unsubscriber{.dispatcher = &dispatcher, .handle = handle};

	dispatcher.Subscribe(EventDelegate<>::FromConstMethod<Unsubscriber, &Unsubscriber::OnEvent>(&unsubscriber));

	dispatcher.Dispatch();
	EXPECT_EQ(target.calls, 1);	// still subscribed for the dispatch it was removed in

	dispatcher.Dispatch();
	EXPECT_EQ(target.calls, 1);	// removal flushed: no further invocations
}
