//
// Module: Tests/Engine/Native/Apple/Metal/MetalBridge/Cocoa
// File: CocoaBridgeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <MetalBridge/Cocoa/CocoaBridge.h>

#include <gtest/gtest.h>

using namespace CE::Bridge;

/**
 * @brief Test fixture for CocoaBridge tests
 */
class CocoaBridgeTest : public ::testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}
};

// ============================================================================
// GetCocoaContentView Tests
// ============================================================================

/**
 * @brief Test that GetCocoaContentView returns nullptr when given null window
 */
TEST_F(CocoaBridgeTest, GetCocoaContentView_NullWindow_ReturnsNullptr) {
	void* result = GetCocoaContentView(nullptr);
	EXPECT_EQ(result, nullptr);
}

/**
 * @brief Test that GetCocoaContentView doesn't throw with null parameter
 */
TEST_F(CocoaBridgeTest, GetCocoaContentView_NullWindow_NoThrow) {
	EXPECT_NO_THROW(GetCocoaContentView(nullptr));
}

// ============================================================================
// SetCocoaViewLayer Tests
// ============================================================================

/**
 * @brief Test that SetCocoaViewLayer handles null view gracefully
 */
TEST_F(CocoaBridgeTest, SetCocoaViewLayer_NullView_NoThrow) {
	EXPECT_NO_THROW(SetCocoaViewLayer(nullptr, nullptr));
}

/**
 * @brief Test that SetCocoaViewLayer handles null layer gracefully
 */
TEST_F(CocoaBridgeTest, SetCocoaViewLayer_NullLayer_NoThrow) {
	// Even with a hypothetically valid view pointer, null layer should be handled
	EXPECT_NO_THROW(SetCocoaViewLayer(reinterpret_cast<void*>(0x1), nullptr));
}

/**
 * @brief Test that SetCocoaViewLayer handles both null parameters
 */
TEST_F(CocoaBridgeTest, SetCocoaViewLayer_BothNull_NoThrow) {
	EXPECT_NO_THROW(SetCocoaViewLayer(nullptr, nullptr));
}

// ============================================================================
// SetCocoaWindowContentView Tests
// ============================================================================

/**
 * @brief Test that SetCocoaWindowContentView handles null window
 */
TEST_F(CocoaBridgeTest, SetCocoaWindowContentView_NullWindow_NoThrow) {
	EXPECT_NO_THROW(SetCocoaWindowContentView(nullptr, nullptr));
}

/**
 * @brief Test that SetCocoaWindowContentView handles null view
 */
TEST_F(CocoaBridgeTest, SetCocoaWindowContentView_NullView_NoThrow) {
	EXPECT_NO_THROW(SetCocoaWindowContentView(reinterpret_cast<void*>(0x1), nullptr));
}

/**
 * @brief Test that SetCocoaWindowContentView handles both null parameters
 */
TEST_F(CocoaBridgeTest, SetCocoaWindowContentView_BothNull_NoThrow) {
	EXPECT_NO_THROW(SetCocoaWindowContentView(nullptr, nullptr));
}

// ============================================================================
// AddSubviewToContentView Tests
// ============================================================================

/**
 * @brief Test that AddSubviewToContentView handles null window
 */
TEST_F(CocoaBridgeTest, AddSubviewToContentView_NullWindow_NoThrow) {
	EXPECT_NO_THROW(AddSubviewToContentView(nullptr, nullptr));
}

/**
 * @brief Test that AddSubviewToContentView handles null subview
 */
TEST_F(CocoaBridgeTest, AddSubviewToContentView_NullSubview_NoThrow) {
	EXPECT_NO_THROW(AddSubviewToContentView(reinterpret_cast<void*>(0x1), nullptr));
}

/**
 * @brief Test that AddSubviewToContentView handles both null parameters
 */
TEST_F(CocoaBridgeTest, AddSubviewToContentView_BothNull_NoThrow) {
	EXPECT_NO_THROW(AddSubviewToContentView(nullptr, nullptr));
}

// ============================================================================
// GetDisplayRefreshRate Tests
// ============================================================================

/**
 * @brief Test that GetDisplayRefreshRate returns default value for null window
 */
TEST_F(CocoaBridgeTest, GetDisplayRefreshRate_NullWindow_ReturnsDefault) {
	int refreshRate = GetDisplayRefreshRate(nullptr);
	EXPECT_EQ(refreshRate, 60);
}

/**
 * @brief Test that GetDisplayRefreshRate doesn't throw with null parameter
 */
TEST_F(CocoaBridgeTest, GetDisplayRefreshRate_NullWindow_NoThrow) {
	EXPECT_NO_THROW(GetDisplayRefreshRate(nullptr));
}

/**
 * @brief Test that GetDisplayRefreshRate returns a positive value
 */
TEST_F(CocoaBridgeTest, GetDisplayRefreshRate_NullWindow_ReturnsPositive) {
	int refreshRate = GetDisplayRefreshRate(nullptr);
	EXPECT_GT(refreshRate, 0);
}

/**
 * @brief Test that GetDisplayRefreshRate returns a reasonable value
 */
TEST_F(CocoaBridgeTest, GetDisplayRefreshRate_NullWindow_ReturnsReasonableValue) {
	int refreshRate = GetDisplayRefreshRate(nullptr);
	// Most displays are between 30 and 240 Hz
	EXPECT_GE(refreshRate, 30);
	EXPECT_LE(refreshRate, 360);
}

// ============================================================================
// Integration Tests
// ============================================================================

/**
 * @brief Test that all Cocoa bridge functions exist and can be called
 * @details This test verifies the basic API contract and that symbols are properly linked
 */
TEST_F(CocoaBridgeTest, AllFunctions_Exist_CanBeCalled) {
	EXPECT_NO_THROW({
		GetCocoaContentView(nullptr);
		SetCocoaViewLayer(nullptr, nullptr);
		SetCocoaWindowContentView(nullptr, nullptr);
		AddSubviewToContentView(nullptr, nullptr);
		GetDisplayRefreshRate(nullptr);
	});
}

/**
 * @brief Test that multiple calls to GetDisplayRefreshRate are consistent
 */
TEST_F(CocoaBridgeTest, GetDisplayRefreshRate_MultipleCalls_ConsistentResults) {
	int refreshRate1 = GetDisplayRefreshRate(nullptr);
	int refreshRate2 = GetDisplayRefreshRate(nullptr);

	EXPECT_EQ(refreshRate1, refreshRate2);
}

