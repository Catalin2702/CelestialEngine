//
// Module: CelestialEngine/Engine/Tests/Modules/Define
// File: CompilationTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

// Include the macros we want to test
#include <Define/Define.hpp>

#include <functional>
#include <type_traits>
#include <gtest/gtest.h>

namespace {

// Mock class for testing BIND_FN macros
class TestCallbackClass {
public:
	int value = 0;

	void NoParamCallback() {
		value = 42;
	}

	bool OneParamCallback(const int x) {
		value = x;
		return true;
	}

	void OneParamCallbackVoid(const std::string& str) {
		value = static_cast<int>(str.length());
	}

	// Methods that use BIND_FN macros (as they're intended to be used)
	auto GetNoParamBoundCallback() {
		return BIND_FN_NO_PARAMS(NoParamCallback);
	}

	auto GetOneParamBoundCallback() {
		return BIND_FN_ONE_PARAM(OneParamCallback);
	}

	auto GetOneParamVoidBoundCallback() {
		return BIND_FN_ONE_PARAM(OneParamCallbackVoid);
	}

	std::function<void()> GetNoParamStdFunction() {
		return BIND_FN_NO_PARAMS(NoParamCallback);
	}

	std::function<bool(int)> GetOneParamStdFunction() {
		return BIND_FN_ONE_PARAM(OneParamCallback);
	}
};

}

/**
 * @brief Compilation Tests for Define Module Macros
 * @details These tests verify that macros expand correctly at compile-time
 *			and produce the expected behavior at runtime.
 */
namespace CompilationTests {

// ============================================================================
// BIT Macro Compilation Tests
// ============================================================================

TEST(DefineCompilationTests, BIT_Macro_CompileTimeConstants) {
	// Test that BIT macro produces compile-time constants
	constexpr int bit0 = BIT(0);
	constexpr int bit1 = BIT(1);
	constexpr int bit2 = BIT(2);
	constexpr int bit7 = BIT(7);

	// Verify values
	static_assert(bit0 == 1, "BIT(0) should be 1");
	static_assert(bit1 == 2, "BIT(1) should be 2");
	static_assert(bit2 == 4, "BIT(2) should be 4");
	static_assert(bit7 == 128, "BIT(7) should be 128");

	// Runtime verification (redundant but comprehensive)
	EXPECT_EQ(bit0, 1);
	EXPECT_EQ(bit1, 2);
	EXPECT_EQ(bit2, 4);
	EXPECT_EQ(bit7, 128);
}

TEST(DefineCompilationTests, BIT_Macro_BitwiseOperations) {
	// Test that BIT macro works correctly with bitwise operations
	constexpr int flags = BIT(0) | BIT(2) | BIT(4);
	static_assert(flags == (1 | 4 | 16), "Bitwise OR should work correctly");
	static_assert(flags == 21, "Combined flags should equal 21");

	EXPECT_EQ(flags, 21);
	EXPECT_TRUE(flags & BIT(0));
	EXPECT_FALSE(flags & BIT(1));
	EXPECT_TRUE(flags & BIT(2));
}

// ============================================================================
// BIND_FN_NO_PARAMS Macro Compilation Tests
// ============================================================================

TEST(DefineCompilationTests, BIND_FN_NO_PARAMS_Macro_CreatesLambda) {
	TestCallbackClass obj;
	const auto callback = obj.GetNoParamBoundCallback();

	// Verify it's callable
	static_assert(std::is_invocable_v<decltype(callback)>, "Lambda should be invocable with no params");

	// Verify it calls the correct method
	EXPECT_EQ(obj.value, 0);
	callback();
	EXPECT_EQ(obj.value, 42);
}

TEST(DefineCompilationTests, BIND_FN_NO_PARAMS_Macro_WorksWithStdFunction) {
	TestCallbackClass obj;
	const auto callback = obj.GetNoParamStdFunction();

	EXPECT_EQ(obj.value, 0);
	callback();
	EXPECT_EQ(obj.value, 42);
}

// ============================================================================
// BIND_FN_ONE_PARAM Macro Compilation Tests
// ============================================================================

TEST(DefineCompilationTests, BIND_FN_ONE_PARAM_Macro_CreatesLambda) {
	TestCallbackClass obj;
	const auto callback = obj.GetOneParamBoundCallback();

	// Verify it's callable with one parameter
	static_assert(std::is_invocable_v<decltype(callback), int>, "Lambda should be invocable with one param");

	// Verify it calls the correct method
	EXPECT_EQ(obj.value, 0);
	const auto result = callback(99);
	EXPECT_TRUE(result);
	EXPECT_EQ(obj.value, 99);
}

TEST(DefineCompilationTests, BIND_FN_ONE_PARAM_Macro_PerfectForwarding) {
	TestCallbackClass obj;
	const auto callback = obj.GetOneParamVoidBoundCallback();

	// Test with lvalue
	const auto* str = "Hello";
	callback(str);
	EXPECT_EQ(obj.value, 5);

	// Test with rvalue
	callback("World!");
	EXPECT_EQ(obj.value, 6);
}

TEST(DefineCompilationTests, BIND_FN_ONE_PARAM_Macro_WorksWithStdFunction) {
	TestCallbackClass obj;
	const auto callback = obj.GetOneParamStdFunction();

	const auto result = callback(123);
	EXPECT_TRUE(result);
	EXPECT_EQ(obj.value, 123);
}

// ============================================================================
// Negative Compilation Tests (via static_assert)
// ============================================================================

TEST(DefineCompilationTests, TypeTraits_VerifyConstexprContext) {
	// Verify that BIT macro can be used in constexpr contexts
	constexpr auto compute_flags = [] constexpr {
		return BIT(0) | BIT(3) | BIT(5);
	};

	constexpr auto flags = compute_flags();
	static_assert(flags == (1 | 8 | 32), "Should be usable in constexpr context");
	EXPECT_EQ(flags, 41);
}

// ============================================================================
// BIND_FN_ANY_PARAMS / BIND_FN_*_ON Macro Compilation Tests
// ============================================================================

TEST(DefineCompilationTests, BIND_FN_ANY_PARAMS_Macro_ForwardsEveryArgument) {
	class MultiParamClass {
	public:
		int sum = 0;

		void Add(const int a, const int b, const int c) { sum = a + b + c; }

		auto GetBoundCallback() { return BIND_FN_ANY_PARAMS(Add); }
	};

	MultiParamClass obj;
	const auto callback = obj.GetBoundCallback();

	callback(1, 2, 3);
	EXPECT_EQ(obj.sum, 6);
}

TEST(DefineCompilationTests, BIND_FN_ONE_PARAM_ON_Macro_BindsToForeignInstance) {
	TestCallbackClass obj;
	const auto callback = BIND_FN_ONE_PARAM_ON(&obj, &TestCallbackClass::OneParamCallback);

	EXPECT_EQ(obj.value, 0);
	EXPECT_TRUE(callback(7));
	EXPECT_EQ(obj.value, 7);
}

TEST(DefineCompilationTests, BIND_FN_ANY_PARAMS_ON_Macro_BindsToForeignInstance) {
	TestCallbackClass obj;
	const auto callback = BIND_FN_ANY_PARAMS_ON(&obj, &TestCallbackClass::OneParamCallback);

	EXPECT_TRUE(callback(15));
	EXPECT_EQ(obj.value, 15);
}

}
