//
// Module: CelestialEngine/Tests/Engine/Modules/Types/KeyCode/Platforms/Mac/Cocoa
// File: CocoaTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Types/KeyCode/Platforms/Mac/Cocoa/Cocoa.hpp>

#include <gtest/gtest.h>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the AppKit key-code and button-number conversions
 * @details Compiled only with this window backend (see Tests/Engine/Modules/Types/CMakeLists.txt).
 */
class CocoaKeyCodeTest: public ::testing::Test {};

}

// ============================================================================
// Keyboard Mapping Tests
// ============================================================================

/**
 * @brief Test that macOS virtual key codes map onto the engine key codes
 */
TEST_F(CocoaKeyCodeTest, KeyboardKeyCodeFromCocoa_MapsKnownKeys) {
	EXPECT_EQ(KeyboardKeyCodeFromCocoa(0), KeyboardKeyCode::A);
	EXPECT_EQ(KeyboardKeyCodeFromCocoa(13), KeyboardKeyCode::W);
	EXPECT_EQ(KeyboardKeyCodeFromCocoa(1), KeyboardKeyCode::S);
}

/**
 * @brief Test that an unmapped macOS key code becomes Unknown
 */
TEST_F(CocoaKeyCodeTest, KeyboardKeyCodeFromCocoa_UnmappedKey_ReturnsUnknown) {
	EXPECT_EQ(KeyboardKeyCodeFromCocoa(9999), KeyboardKeyCode::Unknown);
}

/**
 * @brief Test that the Cocoa mapping round trips
 */
TEST_F(CocoaKeyCodeTest, CocoaMapping_RoundTrips) {
	for (const auto key: {KeyboardKeyCode::A, KeyboardKeyCode::W, KeyboardKeyCode::S, KeyboardKeyCode::D,
			KeyboardKeyCode::Space, KeyboardKeyCode::Escape}) {
		EXPECT_EQ(KeyboardKeyCodeFromCocoa(CocoaKeyCodeFromKeyboard(key)), key) << "Key: " << format_as(key);
	}
}

// ============================================================================
// Mouse Button Mapping Tests
// ============================================================================

/**
 * @brief Test that macOS button numbers map onto the engine button codes
 */
TEST_F(CocoaKeyCodeTest, MouseButtonKeyCodeFromCocoa_MapsKnownButtons) {
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(0), MouseButtonCode::Left);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(1), MouseButtonCode::Right);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(2), MouseButtonCode::Middle);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(7), MouseButtonCode::Unknown);
}

/**
 * @brief Test that the Cocoa mapping round trips for the three main buttons
 */
TEST_F(CocoaKeyCodeTest, CocoaMapping_RoundTripsForMainButtons) {
	for (const auto button: {MouseButtonCode::Left, MouseButtonCode::Right, MouseButtonCode::Middle}) {
		EXPECT_EQ(MouseButtonKeyCodeFromCocoa(CocoaButtonNumberFromMouseButton(button)), button)
			<< "Button: " << format_as(button);
	}
}

/**
 * @brief Test that an unknown button has no macOS button number
 */
TEST_F(CocoaKeyCodeTest, CocoaButtonNumberFromMouseButton_Unknown_ReturnsInvalid) {
	EXPECT_EQ(CocoaButtonNumberFromMouseButton(MouseButtonCode::Unknown), -1);
}
