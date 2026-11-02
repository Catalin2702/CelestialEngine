//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Input
// File: InputTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Core/Input/Input.hpp>

#include "TestEventHub.hpp"
#include <Core/Window/I_Window.hpp>
#include <Events/Events.hpp>
#include <Tools/Tools.hpp>
#include <Types/Types.hpp>

#include <gtest/gtest.h>

#include <array>
#include <span>
#include <string>
#include <vector>


using CE::Core::Input;
using CE::Core::KeyShortcut;
using CE::Types::CursorMode;
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
	CE::Tests::TestEventHubDispatcher _hub;
};

/**
 * @brief A window that only records the cursor modes it is told to apply
 * @details Lets the tests check what Input asks of the window - how often, and with which mode - without a real
 *			windowing backend. Everything else is an inert stub.
 */
class RecordingWindow final: public CE::Core::I_Window {
public:
	void Show() override {}
	void ConnectToEventHub(CE::Core::I_EventHubDispatcher&) override {}
	void Miniaturize() override {}
	void Deminiaturize() override {}
	void ToggleFullScreen() override {}
	void SetWindowSize(u32, u32) override {}
	void SetTitle(std::string_view) override {}

	void SetCursorMode(const CursorMode cursorMode) override {
		appliedModes.push_back(cursorMode);
		_cursorMode = cursorMode;
	}

	[[nodiscard]] std::pair<u32, u32> GetWindowSize() const override { return {0, 0}; }
	[[nodiscard]] std::string GetTitle() const override { return {}; }
	[[nodiscard]] CursorMode GetCursorMode() const override { return _cursorMode; }
	[[nodiscard]] std::pair<u32, u32> GetFrameSize() const override { return {0, 0}; }
	[[nodiscard]] f32 GetContentScale() const override { return 1.0f; }
	[[nodiscard]] u32 GetRefreshRate() const override { return 0; }
	[[nodiscard]] void* GetNativeWindow() const override { return nullptr; }
	[[nodiscard]] CE::Types::WindowApi GetWindowApi() const override { return CE::Types::WindowApi::None; }

public:
	std::vector<CursorMode> appliedModes;	///< Every mode SetCursorMode received, in order

private:
	CursorMode _cursorMode = CursorMode::Normal;
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
// Mouse Delta Tests
// ============================================================================

/**
 * @brief Test that the first cursor position produces no motion
 * @details The cursor was never seen before, so measuring from the origin would report a jump nobody made.
 */
TEST_F(InputTest, FirstMouseMove_ProducesNoDelta) {
	MoveMouse(300.0f, 200.0f);

	const auto [dx, dy] = Input::GetMouseDeltaXY();

	EXPECT_FLOAT_EQ(dx, 0.0f);
	EXPECT_FLOAT_EQ(dy, 0.0f);
}

/**
 * @brief Test that the delta is measured from the position at the end of the previous frame
 * @details Several moves within one frame add up to the net motion of that frame.
 */
TEST_F(InputTest, MouseDelta_IsMotionSinceThePreviousFrame) {
	MoveMouse(100.0f, 100.0f);
	Input::EndFrame();

	MoveMouse(105.0f, 98.0f);
	MoveMouse(110.0f, 95.0f);

	EXPECT_FLOAT_EQ(Input::GetMouseDeltaX(), 10.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseDeltaY(), -5.0f);
}

/**
 * @brief Test that a frame without motion reports a zero delta
 */
TEST_F(InputTest, EndFrame_ResetsMouseDelta) {
	MoveMouse(100.0f, 100.0f);
	Input::EndFrame();
	MoveMouse(120.0f, 130.0f);
	Input::EndFrame();

	EXPECT_FLOAT_EQ(Input::GetMouseDeltaX(), 0.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseDeltaY(), 0.0f);
}

/**
 * @brief Test that the first position after a focus loss produces no motion
 * @details The cursor moved elsewhere while the window was unfocused; that travel is not motion of this window.
 */
TEST_F(InputTest, FocusLost_FirstMouseMoveAfterwardsProducesNoDelta) {
	MoveMouse(100.0f, 100.0f);
	Input::EndFrame();

	ChangeFocus(false);
	Input::EndFrame();
	ChangeFocus(true);
	MoveMouse(800.0f, 600.0f);

	EXPECT_FLOAT_EQ(Input::GetMouseDeltaX(), 0.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseDeltaY(), 0.0f);
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

#if CE_WINDOW_GLFW
/**
 * @brief Test that the raw platform callbacks feed the input state through the hub
 * @details This is the full runtime path: GLFW callback → hub Receive → engine event → multicast → input state. GLFW's
 *			raw callbacks only exist where the GLFW backend does (CE_WINDOW_GLFW).
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
#endif

// ============================================================================
// Shortcut Tests
// ============================================================================

/**
 * @brief Test that a chord is held only while every one of its keys is
 */
TEST_F(InputTest, AreKeysPressed_TrueOnlyWhileEveryKeyIsHeld) {
	PressKey(KeyboardKeyCode::LeftControl);

	EXPECT_FALSE(Input::AreKeysPressed(KeyboardKeyCode::LeftControl, KeyboardKeyCode::S));

	PressKey(KeyboardKeyCode::S);

	EXPECT_TRUE(Input::AreKeysPressed(KeyboardKeyCode::LeftControl, KeyboardKeyCode::S));

	ReleaseKey(KeyboardKeyCode::LeftControl);

	EXPECT_FALSE(Input::AreKeysPressed(KeyboardKeyCode::LeftControl, KeyboardKeyCode::S));
}

/**
 * @brief Test that a shortcut fires on the frame its last key goes down, and not again while it stays held
 */
TEST_F(InputTest, ShortcutJustPressed_FiresOncePerPress) {
	PressKey(KeyboardKeyCode::LeftShift);
	Input::EndFrame();

	EXPECT_FALSE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));

	PressKey(KeyboardKeyCode::F1);

	EXPECT_TRUE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));

	Input::EndFrame();

	EXPECT_FALSE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
	EXPECT_TRUE(Input::AreKeysPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
}

/**
 * @brief Test that the order the keys go down in does not matter
 */
TEST_F(InputTest, ShortcutJustPressed_IgnoresTheKeyOrder) {
	PressKey(KeyboardKeyCode::F1);
	Input::EndFrame();
	PressKey(KeyboardKeyCode::LeftShift);

	EXPECT_TRUE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
}

/**
 * @brief Test that a stored shortcut answers like the keys it holds, and that an empty one never fires
 */
TEST_F(InputTest, KeyShortcut_MatchesItsKeysAndEmptyNeverFires) {
	constexpr KeyShortcut shortcut{KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1};
	constexpr KeyShortcut empty{};

	PressKey(KeyboardKeyCode::LeftShift);
	PressKey(KeyboardKeyCode::F1);

	EXPECT_TRUE(Input::IsShortcutJustPressed(shortcut));
	EXPECT_FALSE(Input::IsShortcutJustPressed(empty));
	EXPECT_FALSE(Input::AreKeysPressed(empty.GetKeys()));
}

// ============================================================================
// Cursor Mode Tests
// ============================================================================

/**
 * @brief Test that the cursor starts free, and that capturing it is reported
 */
TEST_F(InputTest, CursorMode_StartsNormalAndReportsTheCapture) {
	EXPECT_EQ(Input::GetCursorMode(), CursorMode::Normal);

	Input::SetCursorMode(CursorMode::Captured);

	EXPECT_TRUE(Input::IsCursorCaptured());
}

/**
 * @brief Test that switching mode does not turn the jump between real and virtual position into motion
 */
TEST_F(InputTest, SetCursorMode_FirstMoveAfterTheSwitchProducesNoDelta) {
	MoveMouse(100.0f, 100.0f);
	Input::EndFrame();

	Input::SetCursorMode(CursorMode::Captured);
	MoveMouse(500.0f, 300.0f);

	EXPECT_FLOAT_EQ(Input::GetMouseDeltaX(), 0.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseDeltaY(), 0.0f);

	Input::EndFrame();
	MoveMouse(510.0f, 295.0f);

	EXPECT_FLOAT_EQ(Input::GetMouseDeltaX(), 10.0f);
	EXPECT_FLOAT_EQ(Input::GetMouseDeltaY(), -5.0f);
}

/**
 * @brief Test that losing focus gives the cursor back, so it is never left frozen over another application
 */
TEST_F(InputTest, FocusLost_ReleasesTheCursor) {
	Input::SetCursorMode(CursorMode::Captured);
	ChangeFocus(false);

	EXPECT_EQ(Input::GetCursorMode(), CursorMode::Normal);
}

// ============================================================================
// Shortcut Edge Case Tests
// ============================================================================

/**
 * @brief Test that the span overload answers like the variadic one
 */
TEST_F(InputTest, AreKeysPressed_SpanOverloadMatchesVariadic) {
	PressKey(KeyboardKeyCode::LeftAlt);
	PressKey(KeyboardKeyCode::Q);

	const std::array chord{KeyboardKeyCode::LeftAlt, KeyboardKeyCode::Q};

	EXPECT_TRUE(Input::AreKeysPressed(std::span<const KeyboardKeyCode>{chord}));
	EXPECT_EQ(Input::AreKeysPressed(std::span<const KeyboardKeyCode>{chord}), Input::AreKeysPressed(KeyboardKeyCode::LeftAlt, KeyboardKeyCode::Q));
}

/**
 * @brief Test that holding extra keys does not prevent a shortcut from firing
 */
TEST_F(InputTest, ShortcutJustPressed_ExtraHeldKeysDoNotBlockIt) {
	PressKey(KeyboardKeyCode::LeftControl);
	PressKey(KeyboardKeyCode::LeftShift);
	PressKey(KeyboardKeyCode::F1);

	EXPECT_TRUE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
}

/**
 * @brief Test that the shortcut fires again after one of its keys is released and pressed again
 */
TEST_F(InputTest, ShortcutJustPressed_FiresAgainAfterARepress) {
	PressKey(KeyboardKeyCode::LeftShift);
	PressKey(KeyboardKeyCode::F1);
	Input::EndFrame();

	ReleaseKey(KeyboardKeyCode::F1);
	Input::EndFrame();
	PressKey(KeyboardKeyCode::F1);

	EXPECT_TRUE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
}

/**
 * @brief Test that an OS key repeat does not count as a new press of the shortcut
 */
TEST_F(InputTest, ShortcutJustPressed_IgnoresKeyRepeats) {
	PressKey(KeyboardKeyCode::LeftShift);
	PressKey(KeyboardKeyCode::F1);
	Input::EndFrame();

	PressKey(KeyboardKeyCode::F1, 1);

	EXPECT_FALSE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1));
}

/**
 * @brief Test that a shortcut naming an Unknown key can never fire
 */
TEST_F(InputTest, ShortcutWithUnknownKey_NeverFires) {
	PressKey(KeyboardKeyCode::LeftShift);

	EXPECT_FALSE(Input::IsShortcutJustPressed(KeyboardKeyCode::LeftShift, KeyboardKeyCode::Unknown));
}

// ============================================================================
// Cursor Mode Window Tests
// ============================================================================

/**
 * @brief Test that a bound window is told about every real change, and only once per change
 */
TEST_F(InputTest, SetCursorMode_AppliesEachChangeToTheBoundWindowOnce) {
	RecordingWindow window;
	Input::BindWindow(&window);

	Input::SetCursorMode(CursorMode::Captured);
	Input::SetCursorMode(CursorMode::Captured);
	Input::SetCursorMode(CursorMode::Normal);

	EXPECT_EQ(window.appliedModes, (std::vector{CursorMode::Captured, CursorMode::Normal}));

	Input::BindWindow(nullptr);
}

/**
 * @brief Test that losing focus releases the cursor on the window too, not only in the recorded state
 */
TEST_F(InputTest, FocusLost_ReleasesTheCursorOnTheBoundWindow) {
	RecordingWindow window;
	Input::BindWindow(&window);
	Input::SetCursorMode(CursorMode::Captured);

	ChangeFocus(false);

	ASSERT_FALSE(window.appliedModes.empty());
	EXPECT_EQ(window.appliedModes.back(), CursorMode::Normal);

	Input::BindWindow(nullptr);
}

/**
 * @brief Test that losing focus with a free cursor does not bother the window
 */
TEST_F(InputTest, FocusLost_WithAFreeCursorLeavesTheWindowAlone) {
	RecordingWindow window;
	Input::BindWindow(&window);

	ChangeFocus(false);

	EXPECT_TRUE(window.appliedModes.empty());

	Input::BindWindow(nullptr);
}

/**
 * @brief Test that unbinding the window stops forwarding modes to it
 */
TEST_F(InputTest, BindWindowNull_StopsForwardingModes) {
	RecordingWindow window;
	Input::BindWindow(&window);
	Input::BindWindow(nullptr);

	Input::SetCursorMode(CursorMode::Captured);

	EXPECT_TRUE(window.appliedModes.empty());
	EXPECT_TRUE(Input::IsCursorCaptured());
}

/**
 * @brief Test that Unknown is refused instead of being recorded or forwarded to the window
 */
TEST_F(InputTest, SetCursorMode_RejectsUnknown) {
	RecordingWindow window;
	Input::BindWindow(&window);
	Input::SetCursorMode(CursorMode::Captured);

	Input::SetCursorMode(CursorMode::Unknown);

	EXPECT_TRUE(Input::IsCursorCaptured());
	EXPECT_EQ(window.appliedModes, (std::vector{CursorMode::Captured}));

	Input::BindWindow(nullptr);
}
