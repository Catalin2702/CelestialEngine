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
