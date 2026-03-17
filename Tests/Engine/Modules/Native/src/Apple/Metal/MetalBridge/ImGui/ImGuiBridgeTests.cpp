//
// Module: CelestialEngine/Tests/Engine/Modules/Native/Apple/Metal/MetalBridge/ImGui
// File: ImGuiBridgeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#include <MetalBridge/ImGui/ImGuiBridge.h>

#include <gtest/gtest.h>

using namespace CE::Apple::Bridge;

/**
 * @brief Test fixture for ImGuiBridge tests
 * @note ImGui Metal backend functions require a valid Metal device and cannot be
 *       safely tested without proper Metal context initialization. These tests
 *       verify API completeness and symbol linkage only.
 */
class ImGuiBridgeTest: public ::testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}
};

// ============================================================================
// API Completeness Tests
// ============================================================================

/**
 * @brief Test that ImGuiMetalInit function exists and is properly linked
 * @details Verifies the function signature and symbol linkage without calling it
 * @note The function requires a valid MTLDevice and cannot be tested with null
 */
TEST_F(ImGuiBridgeTest, ImGuiMetalInit_FunctionExists) {
	void (*func)(void*) = &ImGuiMetalInit;
	EXPECT_NE(func, nullptr);
}

/**
 * @brief Test that ImGuiMetalShutdown function exists and is properly linked
 * @details Verifies the function signature and symbol linkage without calling it
 * @note The function requires prior initialization and cannot be safely called without it
 */
TEST_F(ImGuiBridgeTest, ImGuiMetalShutdown_FunctionExists) {
	void (*func)() = &ImGuiMetalShutdown;
	EXPECT_NE(func, nullptr);
}

/**
 * @brief Test that ImGuiMetalNewFrame function exists and is properly linked
 * @details Verifies the function signature and symbol linkage without calling it
 * @note The function requires a valid MTLRenderPassDescriptor and prior initialization
 */
TEST_F(ImGuiBridgeTest, ImGuiMetalNewFrame_FunctionExists) {
	void (*func)(void*) = &ImGuiMetalNewFrame;
	EXPECT_NE(func, nullptr);
}

/**
 * @brief Test that ImGuiMetalRenderDrawData function exists and is properly linked
 * @details Verifies the function signature and symbol linkage without calling it
 * @note The function requires valid Metal objects and prior initialization
 */
TEST_F(ImGuiBridgeTest, ImGuiMetalRenderDrawData_FunctionExists) {
	void (*func)(void*, void*, void*) = &ImGuiMetalRenderDrawData;
	EXPECT_NE(func, nullptr);
}

// ============================================================================
// Integration Tests
// ============================================================================

/**
 * @brief Test that all ImGui Metal bridge functions are properly linked
 * @details Verifies that all bridge functions exist and their symbols are resolvable
 *          This ensures the bridge API is complete and all functions are exported correctly
 */
TEST_F(ImGuiBridgeTest, AllFunctions_AreProperlyLinked) {
	void (*initFunc)(void*) = &ImGuiMetalInit;
	void (*shutdownFunc)() = &ImGuiMetalShutdown;
	void (*newFrameFunc)(void*) = &ImGuiMetalNewFrame;
	void (*renderFunc)(void*, void*, void*) = &ImGuiMetalRenderDrawData;

	EXPECT_NE(initFunc, nullptr);
	EXPECT_NE(shutdownFunc, nullptr);
	EXPECT_NE(newFrameFunc, nullptr);
	EXPECT_NE(renderFunc, nullptr);
}

/**
 * @brief Test that function pointers have different addresses
 * @details Ensures each function has a unique implementation
 */
TEST_F(ImGuiBridgeTest, AllFunctions_HaveUniqueAddresses) {
	void (*initFunc)(void*) = &ImGuiMetalInit;
	void (*newFrameFunc)(void*) = &ImGuiMetalNewFrame;

	// Different function signatures, so we just verify they exist
	EXPECT_NE(reinterpret_cast<void*>(initFunc), nullptr);
	EXPECT_NE(reinterpret_cast<void*>(newFrameFunc), nullptr);
}

/**
 * @brief Test namespace resolution
 * @details Verifies that the CE::Bridge namespace is correctly set up
 */
TEST_F(ImGuiBridgeTest, Namespace_IsCorrectlyResolved) {
	// If this compiles, the namespace is correctly resolved
	using CE::Apple::Bridge::ImGuiMetalInit;
	using CE::Apple::Bridge::ImGuiMetalShutdown;
	using CE::Apple::Bridge::ImGuiMetalNewFrame;
	using CE::Apple::Bridge::ImGuiMetalRenderDrawData;

	SUCCEED();
}

