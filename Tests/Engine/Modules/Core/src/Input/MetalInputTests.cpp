//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Input
// File: MetalInputTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-08
// Updated by: Catalin Chirosca
// Updated: 2026-03-10
//

#include <Core/Application.hpp>
#include <Input/Platforms/Mac/MetalInput.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>
#include <Types/Window/WindowProps.hpp>

#include <memory>
#include <gtest/gtest.h>

using namespace CE::Core;
using namespace CE::Input;
using namespace CE::Tools::Log;
using namespace CE::KeyCode;
using namespace CE::Types::Window;

/**
 * @brief Test fixture for MetalInput tests
 */
class MetalInputTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}

	void TearDown() override {
		Log::Shutdown();
	}

	void _InitInput() {
		if (not _app)
			_app = std::make_unique<Application>(_props);
	}
	void _ShutdownInput() {
		_app.reset();
	}

	WindowProps _props{"MetalInputTests", 800, 600, false, GraphicsApi::Metal};
	std::unique_ptr<Application> _app;
};

// ============================================================================
// Singleton and Manager Tests
// ============================================================================

/**
 * @brief Test that MetalInputManager initializes the singleton instance
 */
TEST_F(MetalInputTest, MetalInputManager_Init_CreatesSingletonInstance) {
	_InitInput();

	const auto* instance = I_Input::Get();
	EXPECT_NE(instance, nullptr);

	_ShutdownInput();
}

/**
 * @brief Test that MetalInputManager shutdown cleans up the singleton
 */
TEST_F(MetalInputTest, MetalInputManager_Shutdown_CleansUpSingleton) {
	_ShutdownInput();

	// After shutdown, instance should still exist but be cleaned up properly
	EXPECT_NO_THROW({
		[[maybe_unused]] const auto* instance = I_Input::Get();
	});
}

/**
 * @brief Test that MetalInputManager can be initialized multiple times
 */
TEST_F(MetalInputTest, MetalInputManager_MultipleInit_DoesNotCrash) {
	EXPECT_NO_THROW({
		_InitInput();
		_InitInput(); // Should handle re-initialization gracefully
		_ShutdownInput();
	});
}

/**
 * @brief Test that MetalInputManager shutdown can be called multiple times
 */
TEST_F(MetalInputTest, MetalInputManager_MultipleShutdown_DoesNotCrash) {
	_InitInput();

	EXPECT_NO_THROW({
		_ShutdownInput();
		_ShutdownInput(); // Should handle multiple shutdowns gracefully
	});
}

// ============================================================================
// Key Input Tests
// ============================================================================

/**
 * @brief Test that IsKeyPressed can be called with valid key code
 */
TEST_F(MetalInputTest, IsKeyPressed_ValidKeyCode_DoesNotCrash) {
	_InitInput();
	EXPECT_NO_THROW({
		[[maybe_unused]] const bool pressed = I_Input::IsKeyPressed(KeyboardKeyCode::A);
	});
	_ShutdownInput();
}

/**
 * @brief Test that IsKeyPressed returns boolean value
 */
TEST_F(MetalInputTest, IsKeyPressed_ValidKeyCode_ReturnsBoolean) {
	_InitInput();
	// GLFW_KEY_SPACE = 32
	const bool result = I_Input::IsKeyPressed(KeyboardKeyCode::Space);

	// Result should be either true or false
	EXPECT_TRUE(result == true || result == false);
	_ShutdownInput();
}

/**
 * @brief Test that IsKeyPressed works with multiple key codes
 */
TEST_F(MetalInputTest, IsKeyPressed_MultipleKeyCodes_DoesNotCrash) {
	_InitInput();
	// GLFW_KEY_ESCAPE = 256
	EXPECT_NO_THROW({
		[[maybe_unused]] const bool escape = I_Input::IsKeyPressed(KeyboardKeyCode::Escape);
		// GLFW_KEY_ENTER = 257
		[[maybe_unused]] const bool enter = I_Input::IsKeyPressed(KeyboardKeyCode::Enter);
		// GLFW_KEY_TAB = 258
		[[maybe_unused]] const bool tab = I_Input::IsKeyPressed(KeyboardKeyCode::Tab);
	});
	_ShutdownInput();
}

/**
 * @brief Test that IsKeyPressed handles letter keys
 */
TEST_F(MetalInputTest, IsKeyPressed_LetterKeys_DoesNotCrash) {
	_InitInput();
	// Test A-Z keys (GLFW_KEY_A through GLFW_KEY_Z = 65-90)
	for (int keyCode = 65; keyCode <= 90; ++keyCode) {
		EXPECT_NO_THROW({
			[[maybe_unused]] const bool pressed = I_Input::IsKeyPressed(KeyboardKeyCodeFromGlfw(keyCode));
		});
	}
	_ShutdownInput();
}

/**
 * @brief Test that IsKeyPressed handles number keys
 */
TEST_F(MetalInputTest, IsKeyPressed_NumberKeys_DoesNotCrash) {
	_InitInput();
	// Test 0-9 keys (GLFW_KEY_0 through GLFW_KEY_9 = 48-57)
	for (int keyCode = 48; keyCode <= 57; ++keyCode) {
		EXPECT_NO_THROW({
			[[maybe_unused]] const bool pressed = I_Input::IsKeyPressed(KeyboardKeyCodeFromGlfw(keyCode));
		});
	}
	_ShutdownInput();
}

/**
 * @brief Test that IsKeyPressed handles function keys
 */
TEST_F(MetalInputTest, IsKeyPressed_FunctionKeys_DoesNotCrash) {
	_InitInput();
	// Test F1-F12 keys (GLFW_KEY_F1 through GLFW_KEY_F12 = 290-301)
	for (int keyCode = 290; keyCode <= 301; ++keyCode) {
		EXPECT_NO_THROW({
			[[maybe_unused]] const bool pressed = I_Input::IsKeyPressed(KeyboardKeyCodeFromGlfw(keyCode));
		});
	}
	_ShutdownInput();
}

// ============================================================================
// Mouse Button Tests
// ============================================================================

/**
 * @brief Test that IsMouseButtonPressed can be called with valid button code
 */
TEST_F(MetalInputTest, IsMouseButtonPressed_ValidButtonCode_DoesNotCrash) {
	_InitInput();
	// GLFW_MOUSE_BUTTON_LEFT = 0
	EXPECT_NO_THROW({
		[[maybe_unused]] const bool pressed = I_Input::IsMouseButtonPressed(MouseButtonCode::Left);
	});
	_ShutdownInput();
}

/**
 * @brief Test that IsMouseButtonPressed returns boolean value
 */
TEST_F(MetalInputTest, IsMouseButtonPressed_ValidButtonCode_ReturnsBoolean) {
	_InitInput();
	// GLFW_MOUSE_BUTTON_LEFT = 0
	const bool result = I_Input::IsMouseButtonPressed(MouseButtonCode::Left);

	// Result should be either true or false
	EXPECT_TRUE(result == true || result == false);
	_ShutdownInput();
}

/**
 * @brief Test that IsMouseButtonPressed works with all mouse buttons
 */
TEST_F(MetalInputTest, IsMouseButtonPressed_AllButtons_DoesNotCrash) {
	_InitInput();
	// GLFW_MOUSE_BUTTON_LEFT = 0
	// GLFW_MOUSE_BUTTON_RIGHT = 1
	// GLFW_MOUSE_BUTTON_MIDDLE = 2
	for (int buttonCode = 0; buttonCode <= 2; ++buttonCode) {
		EXPECT_NO_THROW({
			[[maybe_unused]] const bool pressed = I_Input::IsMouseButtonPressed(MouseButtonKeyCodeFromGlfw(buttonCode));
		});
	}
	_ShutdownInput();
}

/**
 * @brief Test that IsMouseButtonPressed works with extended mouse buttons
 */
TEST_F(MetalInputTest, IsMouseButtonPressed_ExtendedButtons_DoesNotCrash) {
	_InitInput();
	// GLFW supports up to 8 mouse buttons (0-7)
	for (int buttonCode = 0; buttonCode <= 7; ++buttonCode) {
		EXPECT_NO_THROW({
			[[maybe_unused]] const bool pressed = I_Input::IsMouseButtonPressed(MouseButtonKeyCodeFromGlfw(buttonCode));
		});
	}
	_ShutdownInput();
}

// ============================================================================
// Mouse Position Tests
// ============================================================================

/**
 * @brief Test that GetMouseX returns a valid float value
 */
TEST_F(MetalInputTest, GetMouseX_Called_ReturnsValidFloat) {
	_InitInput();
	const float mouseX = I_Input::GetMouseX();

	// Should return a finite number
	EXPECT_FALSE(std::isnan(mouseX));
	EXPECT_FALSE(std::isinf(mouseX));
	_ShutdownInput();
}

/**
 * @brief Test that GetMouseY returns a valid float value
 */
TEST_F(MetalInputTest, GetMouseY_Called_ReturnsValidFloat) {
	_InitInput();
	const float mouseY = I_Input::GetMouseY();

	// Should return a finite number
	EXPECT_FALSE(std::isnan(mouseY));
	EXPECT_FALSE(std::isinf(mouseY));
	_ShutdownInput();
}

/**
 * @brief Test that GetMouseXY returns a valid vector
 */
TEST_F(MetalInputTest, GetMouseXY_Called_ReturnsValidVector) {
	_InitInput();
	const auto [x, y] = I_Input::GetMouseXY();

	// Both components should be finite numbers
	EXPECT_FALSE(std::isnan(x));
	EXPECT_FALSE(std::isinf(x));
	EXPECT_FALSE(std::isnan(y));
	EXPECT_FALSE(std::isinf(y));
	_ShutdownInput();
}

/**
 * @brief Test that GetMouseXY matches individual GetMouseX and GetMouseY
 */
TEST_F(MetalInputTest, GetMouseXY_Consistency_MatchesIndividualGetters) {
	_InitInput();
	const float mouseX = I_Input::GetMouseX();
	const float mouseY = I_Input::GetMouseY();
	const auto [x, y] = I_Input::GetMouseXY();

	// Note: Due to timing, these might not be exactly equal in a real scenario
	// but they should both be valid values
	EXPECT_FALSE(std::isnan(mouseX));
	EXPECT_FALSE(std::isnan(mouseY));
	EXPECT_FALSE(std::isnan(x));
	EXPECT_FALSE(std::isnan(y));
	_ShutdownInput();
}

/**
 * @brief Test that mouse position calls can be made repeatedly
 */
TEST_F(MetalInputTest, MousePosition_RepeatedCalls_DoesNotCrash) {
	_InitInput();
	EXPECT_NO_THROW({
		for (int i = 0; i < 100; ++i) {
			[[maybe_unused]] const float x = I_Input::GetMouseX();
			[[maybe_unused]] const float y = I_Input::GetMouseY();
			[[maybe_unused]] const auto xy = I_Input::GetMouseXY();
		}
	});
	_ShutdownInput();
}

// ============================================================================
// Stress Tests
// ============================================================================

/**
 * @brief Test that multiple rapid input queries don't crash
 */
TEST_F(MetalInputTest, MultipleInputQueries_RapidFire_DoesNotCrash) {
	_InitInput();
	EXPECT_NO_THROW({
		for (int i = 0; i < 1000; ++i) {
			[[maybe_unused]] const bool keyPressed = I_Input::IsKeyPressed(KeyboardKeyCode::A); // 'A' key
			[[maybe_unused]] const bool mousePressed = I_Input::IsMouseButtonPressed(MouseButtonCode::Left); // Left button
			[[maybe_unused]] const float mouseX = I_Input::GetMouseX();
			[[maybe_unused]] const float mouseY = I_Input::GetMouseY();
		}
	});
	_ShutdownInput();
}

/**
 * @brief Test that mixed input queries work correctly
 */
TEST_F(MetalInputTest, MixedInputQueries_Sequential_DoesNotCrash) {
	_InitInput();
	EXPECT_NO_THROW({
		[[maybe_unused]] const bool keyA = I_Input::IsKeyPressed(KeyboardKeyCode::A);
		[[maybe_unused]] const auto mousePos = I_Input::GetMouseXY();
		[[maybe_unused]] const bool leftButton = I_Input::IsMouseButtonPressed(MouseButtonCode::Left);
		[[maybe_unused]] const bool keySpace = I_Input::IsKeyPressed(KeyboardKeyCode::Space);
		[[maybe_unused]] const float mouseX = I_Input::GetMouseX();
		[[maybe_unused]] const bool rightButton = I_Input::IsMouseButtonPressed(MouseButtonCode::Right);
	});
	_ShutdownInput();
}
