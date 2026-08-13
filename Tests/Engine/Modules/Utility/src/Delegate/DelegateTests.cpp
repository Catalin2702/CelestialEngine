//
// Module: CelestialEngine/Tests/Engine/Modules/Utility/Delegate
// File: DelegateTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Utility/Delegate/Delegate.hpp>

#include <gtest/gtest.h>

#include <string>

using CE::Utility::Delegate;

namespace {

int g_freeFunctionCalls = 0;

void FreeVoidFunction() { ++g_freeFunctionCalls; }

int FreeAddFunction(const int a, const int b) { return a + b; }

/**
 * @brief Instance used to exercise method / const-method binding
 */
class DelegateTarget {
public:
	void Increment() { ++calls; }
	void SetValue(const int value) { this->value = value; }
	[[nodiscard]] int GetValue() const { return value; }
	[[nodiscard]] int Multiply(const int factor) const { return value * factor; }

public:
	int calls = 0;
	int value = 0;
};

/**
 * @brief Test fixture for Delegate tests
 */
class DelegateTest: public ::testing::Test {
protected:
	void SetUp() override { g_freeFunctionCalls = 0; }
};

}

// ============================================================================
// Validity Tests
// ============================================================================

/**
 * @brief Test that a default-constructed delegate is not valid
 */
TEST_F(DelegateTest, DefaultConstructed_IsNotValid) {
	const EventDelegate<> delegate;

	EXPECT_FALSE(delegate.IsValid());
}

/**
 * @brief Test that a bound delegate is valid
 */
TEST_F(DelegateTest, FromFunction_IsValid) {
	const auto delegate = EventDelegate<>::FromFunction<&FreeVoidFunction>();

	EXPECT_TRUE(delegate.IsValid());
}

// ============================================================================
// Free Function Binding Tests
// ============================================================================

/**
 * @brief Test that a delegate bound to a free function invokes it
 */
TEST_F(DelegateTest, FromFunction_InvokesFreeFunction) {
	const auto delegate = EventDelegate<>::FromFunction<&FreeVoidFunction>();

	delegate();
	delegate();

	EXPECT_EQ(g_freeFunctionCalls, 2);
}

/**
 * @brief Test that a delegate returning a value forwards both arguments and result
 */
TEST_F(DelegateTest, FromFunction_ForwardsArgumentsAndReturnValue) {
	const auto delegate = CallbackDelegate<int, int, int>::FromFunction<&FreeAddFunction>();

	EXPECT_EQ(delegate(2, 3), 5);
	EXPECT_EQ(delegate(-4, 4), 0);
}

// ============================================================================
// Method Binding Tests
// ============================================================================

/**
 * @brief Test that a delegate bound to a method invokes it on the bound instance
 */
TEST_F(DelegateTest, FromMethod_InvokesMethodOnInstance) {
	DelegateTarget target;
	const auto delegate = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&target);

	delegate();
	delegate();
	delegate();

	EXPECT_EQ(target.calls, 3);
}

/**
 * @brief Test that a delegate bound to a method forwards its argument
 */
TEST_F(DelegateTest, FromMethod_ForwardsArgument) {
	DelegateTarget target;
	const auto delegate = EventDelegate<int>::FromMethod<DelegateTarget, &DelegateTarget::SetValue>(&target);

	delegate(77);

	EXPECT_EQ(target.value, 77);
}

/**
 * @brief Test that two delegates bound to different instances stay independent
 */
TEST_F(DelegateTest, FromMethod_DifferentInstances_AreIndependent) {
	DelegateTarget first;
	DelegateTarget second;

	const auto firstDelegate = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&first);
	const auto secondDelegate = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&second);

	firstDelegate();
	secondDelegate();
	secondDelegate();

	EXPECT_EQ(first.calls, 1);
	EXPECT_EQ(second.calls, 2);
}

// ============================================================================
// Const Method Binding Tests
// ============================================================================

/**
 * @brief Test that a delegate bound to a const method reads the instance state
 */
TEST_F(DelegateTest, FromConstMethod_InvokesConstMethod) {
	DelegateTarget target;
	target.value = 21;

	const auto delegate = CallbackDelegate<int>::FromConstMethod<DelegateTarget, &DelegateTarget::GetValue>(&target);

	EXPECT_EQ(delegate(), 21);
}

/**
 * @brief Test that a delegate bound to a const method forwards its argument
 */
TEST_F(DelegateTest, FromConstMethod_ForwardsArgument) {
	DelegateTarget target;
	target.value = 6;

	const auto delegate = CallbackDelegate<int, int>::FromConstMethod<DelegateTarget, &DelegateTarget::Multiply>(&target);

	EXPECT_EQ(delegate(7), 42);
}

// ============================================================================
// Comparison Tests
// ============================================================================

/**
 * @brief Test that two delegates bound to the same instance and method compare equal
 */
TEST_F(DelegateTest, OperatorEquals_SameInstanceAndMethod_AreEqual) {
	DelegateTarget target;

	const auto first = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&target);
	const auto second = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&target);

	EXPECT_TRUE(first == second);
}

/**
 * @brief Test that delegates bound to different instances compare unequal
 */
TEST_F(DelegateTest, OperatorEquals_DifferentInstances_AreNotEqual) {
	DelegateTarget first;
	DelegateTarget second;

	const auto firstDelegate = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&first);
	const auto secondDelegate = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&second);

	EXPECT_FALSE(firstDelegate == secondDelegate);
}

/**
 * @brief Test that a copied delegate keeps invoking the same target
 */
TEST_F(DelegateTest, CopiedDelegate_KeepsSameTarget) {
	DelegateTarget target;

	const auto original = EventDelegate<>::FromMethod<DelegateTarget, &DelegateTarget::Increment>(&target);
	const auto copy = original;   // NOLINT(performance-unnecessary-copy-initialization)

	copy();

	EXPECT_TRUE(original == copy);
	EXPECT_EQ(target.calls, 1);
}
