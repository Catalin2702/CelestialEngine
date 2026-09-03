//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Input
// File: InputTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include <Core/Input/Input.hpp>

#include <Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/WindowEvent.hpp>
#include <Tools/Tools.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>

#include <gtest/gtest.h>


using CE::Core::GlfwEventHubDispatcher;
using CE::Core::Input;
using CE::Types::KeyboardKeyCode;
using CE::Types::MouseButtonCode;

namespace Events = CE::Events;

namespace {

/**
 * @brief Test fixture for the event-driven input state
 * @details The input system no longer polls the OS: it subscribes to the event hub and is fed exclusively by the
 *			events the hub multicasts. The tests therefore drive a real hub instance and assert on what the Input
 *			queries report, which is the same path the engine uses at runtime, without needing a window.
 */
class InputTest: public ::testing::Test {
protected:
	void SetUp() override {
		CE::Tools::Log::Init();

		Input::Init();
		Input::SubscribeToHub(_hub.keyboardEventHub, _hub.mouseEventHub, _hub.windowEventHub);
	}

	void TearDown() override {
		Input::UnsubscribeFromHub(_hub.keyboardEventHub, _hub.mouseEventHub, _hub.windowEventHub);
		Input::Shutdown();

		CE::Tools::Log::Shutdown();
	}

	/**
	 * @brief Pushes a key press through the hub, as the platform layer would
	 */
	void PressKey(const KeyboardKeyCode key, const int repeatCount = 0) {
		Events::KeyPressedEvent event{key, repeatCount};
		_hub.DispatchKeyPressedEvent(event);
	}

	/**
	 * @brief Pushes a key release through the hub
	 */
	void ReleaseKey(const KeyboardKeyCode key) {
		Events::KeyReleasedEvent event{key};
		_hub.DispatchKeyReleasedEvent(event);
	}

	/**
	 * @brief Pushes a mouse button press through the hub
	 */
	void PressButton(const MouseButtonCode button) {
		Events::MouseButtonPressedEvent event{button};
		_hub.DispatchMouseButtonPressedEvent(event);
	}

	/**
	 * @brief Pushes a mouse button release through the hub
	 */
	void ReleaseButton(const MouseButtonCode button) {
		Events::MouseButtonReleasedEvent event{button};
		_hub.DispatchMouseButtonReleasedEvent(event);
	}

	/**
	 * @brief Pushes a cursor move through the hub
	 */
	void MoveMouse(const f32 x, const f32 y) {
		Events::MouseMovedEvent event{x, y};
		_hub.DispatchMouseMovedEvent(event);
	}

	/**
	 * @brief Pushes a wheel scroll through the hub
	 */
	void ScrollWheel(const f32 xOffset, const f32 yOffset) {
		Events::MouseWheelScrolledEvent event{xOffset, yOffset};
		_hub.DispatchMouseWheelScrolledEvent(event);
	}

	/**
	 * @brief Pushes a window focus change through the hub
	 */
	void ChangeFocus(const bool focused) {
		Events::WindowFocusEvent event{focused};
		_hub.DispatchWindowFocusEvent(event);
	}

protected:
	GlfwEventHubDispatcher _hub;
};

}

// ============================================================================
// Keyboard Tests
// ============================================================================

/**
 * @brief Test that no key is reported as pressed before any event arrives
 */
TEST_F(InputTest, InitialState_NoKeyIsPressed) {
	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsKeyJustPressed(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
}

/**
 * @brief Test that a key press event marks the key as down and just-pressed
 */
TEST_F(InputTest, KeyPressedEvent_MarksKeyDownAndJustPressed) {
	PressKey(KeyboardKeyCode::A);

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_TRUE(Input::IsKeyJustPressed(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
}

/**
 * @brief Test that a held key stays down across frames but is just-pressed only in the first one
 */
TEST_F(InputTest, HeldKey_StaysDownButIsJustPressedOnlyOnce) {
	PressKey(KeyboardKeyCode::W);

	EXPECT_TRUE(Input::IsKeyJustPressed(KeyboardKeyCode::W));

	Input::EndFrame();

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::W));
	EXPECT_FALSE(Input::IsKeyJustPressed(KeyboardKeyCode::W));
}

/**
 * @brief Test that an auto-repeat press does not produce a new just-pressed
 * @details The platform sends repeats while a key is held; only the initial transition counts as "just pressed".
 */
TEST_F(InputTest, RepeatedKeyPress_DoesNotProduceNewJustPressed) {
	PressKey(KeyboardKeyCode::W);
	Input::EndFrame();

	PressKey(KeyboardKeyCode::W, 1);

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::W));
	EXPECT_FALSE(Input::IsKeyJustPressed(KeyboardKeyCode::W));
}

/**
 * @brief Test that a key release clears the down state and reports just-released
 */
TEST_F(InputTest, KeyReleasedEvent_ClearsDownAndReportsJustReleased) {
	PressKey(KeyboardKeyCode::A);
	Input::EndFrame();

	ReleaseKey(KeyboardKeyCode::A);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_TRUE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
}

/**
 * @brief Test that the just-released flag is cleared by the end of the frame
 */
TEST_F(InputTest, EndFrame_ClearsJustReleased) {
	PressKey(KeyboardKeyCode::A);
	ReleaseKey(KeyboardKeyCode::A);

	Input::EndFrame();

	EXPECT_FALSE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
}

/**
 * @brief Test that keys are tracked independently of each other
 */
TEST_F(InputTest, Keys_AreTrackedIndependently) {
	PressKey(KeyboardKeyCode::A);
	PressKey(KeyboardKeyCode::D);
	ReleaseKey(KeyboardKeyCode::A);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::D));
}

/**
 * @brief Test that an unknown key code is ignored instead of corrupting the state
 */
TEST_F(InputTest, UnknownKeyCode_IsIgnored) {
	PressKey(KeyboardKeyCode::Unknown);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::Unknown));
}

/**
 * @brief Test that the highest tracked key code is handled
 * @details The key bitset is sized on the GLFW range: the highest code (RightSuper, 347) must still fit.
 */
TEST_F(InputTest, HighestKeyCode_IsTracked) {
	PressKey(KeyboardKeyCode::RightSuper);

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::RightSuper));
}

// ============================================================================
// Mouse Button Tests
// ============================================================================

/**
 * @brief Test that a button press marks the button down and just-pressed
 */
TEST_F(InputTest, MouseButtonPressedEvent_MarksButtonDownAndJustPressed) {
	PressButton(MouseButtonCode::Left);

	EXPECT_TRUE(Input::IsMouseButtonPressed(MouseButtonCode::Left));
	EXPECT_TRUE(Input::IsMouseButtonJustPressed(MouseButtonCode::Left));
	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Right));
}

/**
 * @brief Test that a button release clears the down state and reports just-released
 */
TEST_F(InputTest, MouseButtonReleasedEvent_ClearsDownAndReportsJustReleased) {
	PressButton(MouseButtonCode::Right);
	Input::EndFrame();

	ReleaseButton(MouseButtonCode::Right);

	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Right));
	EXPECT_TRUE(Input::IsMouseButtonJustReleased(MouseButtonCode::Right));
}

/**
 * @brief Test that a held button is just-pressed only in the frame it went down
 */
TEST_F(InputTest, HeldMouseButton_IsJustPressedOnlyOnce) {
	PressButton(MouseButtonCode::Left);
	Input::EndFrame();

	PressButton(MouseButtonCode::Left);

	EXPECT_TRUE(Input::IsMouseButtonPressed(MouseButtonCode::Left));
	EXPECT_FALSE(Input::IsMouseButtonJustPressed(MouseButtonCode::Left));
}

/**
 * @brief Test that an unknown button code is ignored
 */
TEST_F(InputTest, UnknownMouseButtonCode_IsIgnored) {
	PressButton(MouseButtonCode::Unknown);

	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Unknown));
}

/**
 * @brief Test that the wheel pseudo-buttons do not disturb the real buttons
 * @details Scrolling is reported through the scroll deltas; the wheel button codes fall inside the tracked range,
 *			so what matters is that they stay on their own slot instead of aliasing a real button.
 */
TEST_F(InputTest, WheelPseudoButtons_DoNotAliasRealButtons) {
	PressButton(MouseButtonCode::WheelY);

	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Left));
	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Right));
	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Middle));
}

// ============================================================================
// Cursor Position Tests
// ============================================================================

/**
 * @brief Test that the cursor position starts at the origin
 */
TEST_F(InputTest, InitialState_CursorIsAtOrigin) {
	EXPECT_FLOAT_EQ(Input::GetMouseX(), 0.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseY(), 0.0f);
}

/**
 * @brief Test that a mouse move updates the tracked position
 */
TEST_F(InputTest, MouseMovedEvent_UpdatesPosition) {
	MoveMouse(120.5f, 340.25f);

	EXPECT_FLOAT_EQ(Input::GetMouseX(), 120.5f);
	EXPECT_FLOAT_EQ(Input::GetMouseY(), 340.25f);

	const auto [x, y] = Input::GetMouseXY();

	EXPECT_FLOAT_EQ(x, 120.5f);
	EXPECT_FLOAT_EQ(y, 340.25f);
}

/**
 * @brief Test that a drag updates the position too
 * @details While a button is held the platform reports drags instead of moves; the tracked position must follow
 *			them, otherwise the cursor would appear frozen for the whole drag.
 */
TEST_F(InputTest, MouseDraggedEvent_UpdatesPosition) {
	Events::MouseDraggedEvent event{MouseButtonCode::Left, 42.0f, 84.0f};
	_hub.DispatchMouseDraggedEvent(event);

	EXPECT_FLOAT_EQ(Input::GetMouseX(), 42.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseY(), 84.0f);
}

/**
 * @brief Test that the cursor position survives the end of the frame
 */
TEST_F(InputTest, EndFrame_KeepsCursorPosition) {
	MoveMouse(10.0f, 20.0f);
	Input::EndFrame();

	EXPECT_FLOAT_EQ(Input::GetMouseX(), 10.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseY(), 20.0f);
}

// ============================================================================
// Scroll Tests
// ============================================================================

/**
 * @brief Test that scroll offsets accumulate within a frame
 */
TEST_F(InputTest, ScrollEvents_AccumulateWithinTheFrame) {
	ScrollWheel(1.0f, 2.0f);
	ScrollWheel(0.5f, -1.0f);

	EXPECT_FLOAT_EQ(Input::GetScrollDeltaX(), 1.5f);
	EXPECT_FLOAT_EQ(Input::GetScrollDeltaY(), 1.0f);
}

/**
 * @brief Test that the scroll deltas are reset at the end of the frame
 * @details Scroll is a per-frame delta, not a state: it must not leak into the next frame.
 */
TEST_F(InputTest, EndFrame_ResetsScrollDeltas) {
	ScrollWheel(1.0f, 2.0f);
	Input::EndFrame();

	EXPECT_FLOAT_EQ(Input::GetScrollDeltaX(), 0.0f);
	EXPECT_FLOAT_EQ(Input::GetScrollDeltaY(), 0.0f);
}

// ============================================================================
// Focus Tests
// ============================================================================

/**
 * @brief Test that losing focus releases every held key and button
 * @details A key released while another window has focus never reaches us, so it would otherwise stay stuck down.
 */
TEST_F(InputTest, FocusLost_ReleasesHeldKeysAndButtons) {
	PressKey(KeyboardKeyCode::A);
	PressButton(MouseButtonCode::Left);
	Input::EndFrame();

	ChangeFocus(false);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButtonCode::Left));
	EXPECT_TRUE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
	EXPECT_TRUE(Input::IsMouseButtonJustReleased(MouseButtonCode::Left));
}

/**
 * @brief Test that gaining focus leaves the input state untouched
 */
TEST_F(InputTest, FocusGained_LeavesStateUntouched) {
	PressKey(KeyboardKeyCode::A);
	Input::EndFrame();

	ChangeFocus(true);

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_FALSE(Input::IsKeyJustReleased(KeyboardKeyCode::A));
}

// ============================================================================
// Subscription Tests
// ============================================================================

/**
 * @brief Test that the input state stops tracking once unsubscribed from the hub
 */
TEST_F(InputTest, UnsubscribeFromHub_StopsTrackingEvents) {
	Input::UnsubscribeFromHub(_hub.keyboardEventHub, _hub.mouseEventHub, _hub.windowEventHub);

	PressKey(KeyboardKeyCode::A);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));

	// Re-subscribe so the fixture teardown unsubscribes a live subscription
	Input::SubscribeToHub(_hub.keyboardEventHub, _hub.mouseEventHub, _hub.windowEventHub);
}

/**
 * @brief Test that the raw platform callbacks feed the input state through the hub
 * @details This is the full runtime path: GLFW callback → hub Receive → engine event → multicast → input state.
 */
TEST_F(InputTest, RawHubCallbacks_FeedTheInputState) {
	constexpr int glfwKeyA = 65;			// GLFW_KEY_A
	constexpr int glfwPress = 1;			// GLFW_PRESS
	constexpr int glfwRelease = 0;			// GLFW_RELEASE
	constexpr int glfwMouseButtonLeft = 0;	// GLFW_MOUSE_BUTTON_LEFT

	_hub.ReceiveKeyEvent(glfwKeyA, glfwPress, 0, 0);
	_hub.ReceiveMouseButtonEvent(glfwMouseButtonLeft, glfwPress, 0);
	_hub.ReceiveMousePositionEvent(15.0, 25.0);
	_hub.ReceiveMouseWheelScrollEvent(0.0, 3.0);

	EXPECT_TRUE(Input::IsKeyPressed(KeyboardKeyCode::A));
	EXPECT_TRUE(Input::IsMouseButtonPressed(MouseButtonCode::Left));
	EXPECT_FLOAT_EQ(Input::GetMouseX(), 15.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseY(), 25.0f);
	EXPECT_FLOAT_EQ(Input::GetScrollDeltaY(), 3.0f);

	_hub.ReceiveKeyEvent(glfwKeyA, glfwRelease, 0, 0);

	EXPECT_FALSE(Input::IsKeyPressed(KeyboardKeyCode::A));
}
