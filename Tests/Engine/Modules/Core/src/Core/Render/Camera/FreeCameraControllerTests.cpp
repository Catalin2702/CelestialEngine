//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Camera
// File: FreeCameraControllerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Core/Core.hpp>

#include "TestEventHub.hpp"

#include <Events/Events.hpp>
#include <Tools/Tools.hpp>
#include <Types/Types.hpp>

#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


using CE::Core::Camera;
using CE::Core::CameraControllerDescriptor;
using CE::Core::CameraDescriptor;
using CE::Core::CameraFreeControllerDescriptor;
using CE::Core::CameraLookTrigger;
using CE::Core::CameraProjectionDescriptor;
using CE::Core::FreeCameraController;
using CE::Core::Input;
using CE::Types::CursorMode;
using CE::Types::KeyboardKeyCode;
using CE::Types::MouseButtonCode;

namespace Events = CE::Events;

namespace {

/**
 * @brief Drives a FreeCameraController through the real input state, fed by a hub like the platform layer would
 * @details The controller reads Input statically, so the fixture owns the whole chain: Input, a GLFW hub to push
 *			events into, and an orthographic camera to read the pose back from. No window: the cursor mode is only
 *			recorded, which is all the controller looks at.
 */
class FreeCameraControllerTest: public ::testing::Test {
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
	 * @brief A free controller with the given tuning; sensitivity 1 makes one point of motion one degree
	 */
	static FreeCameraController MakeController(const CameraFreeControllerDescriptor& descriptor = {.move = {}, .look = {.sensitivity = 1.0f}}) {
		return FreeCameraController{CameraControllerDescriptor{.descriptor = descriptor}};
	}

	/**
	 * @brief Ends the frame the way the application does: the controller has run, the transitions are cleared
	 */
	static void EndFrame() { Input::EndFrame(); }

	void PressKey(const KeyboardKeyCode key) {
		Events::KeyPressedEvent event{key, 0};
		_hub.DispatchKeyPressedEvent(event);
	}

	void ReleaseKey(const KeyboardKeyCode key) {
		Events::KeyReleasedEvent event{key};
		_hub.DispatchKeyReleasedEvent(event);
	}

	void PressButton(const MouseButtonCode button) {
		Events::MouseButtonPressedEvent event{button};
		_hub.DispatchMouseButtonPressedEvent(event);
	}

	void MoveMouse(const f32 x, const f32 y) {
		Events::MouseMovedEvent event{x, y};
		_hub.DispatchMouseMovedEvent(event);
	}

	void ScrollWheel(const f32 yOffset) {
		Events::MouseWheelScrolledEvent event{0.0f, yOffset};
		_hub.DispatchMouseWheelScrolledEvent(event);
	}

	/**
	 * @brief Produces one frame of horizontal mouse motion: a baseline frame first, then the move itself
	 */
	void MoveMouseRight(const f32 points) {
		MoveMouse(0.0f, 0.0f);
		EndFrame();
		MoveMouse(points, 0.0f);
	}

	/**
	 * @brief Captures the cursor with the default Shift+F1, through one controller update
	 */
	void CaptureWith(FreeCameraController& controller) {
		PressKey(KeyboardKeyCode::LeftShift);
		PressKey(KeyboardKeyCode::F1);
		controller.OnUpdate(_camera, 0.0f);
		ReleaseKey(KeyboardKeyCode::F1);
		ReleaseKey(KeyboardKeyCode::LeftShift);
		EndFrame();
	}

protected:
	CE::Tests::TestEventHubDispatcher _hub;

	Camera _camera{
		CameraDescriptor{.position = {0.0f, 0.0f, 0.0f}},
		CameraProjectionDescriptor{
			.type = CE::Types::CameraProjection::Orthographic,
			.convention = CE::Types::ClipConvention::ZeroToOne
		}
	};
};

}

// ============================================================================
// Capture Tests
// ============================================================================

/**
 * @brief Test that the capture shortcut toggles the cursor mode, once per press
 */
TEST_F(FreeCameraControllerTest, CaptureShortcut_TogglesTheCursorMode) {
	auto controller = MakeController();
	controller.OnAttach(_camera);

	CaptureWith(controller);
	EXPECT_TRUE(Input::IsCursorCaptured());

	CaptureWith(controller);
	EXPECT_FALSE(Input::IsCursorCaptured());
}

/**
 * @brief Test that holding the shortcut does not flip the capture on every frame
 */
TEST_F(FreeCameraControllerTest, HeldShortcut_TogglesOnlyOnce) {
	auto controller = MakeController();
	controller.OnAttach(_camera);

	PressKey(KeyboardKeyCode::LeftShift);
	PressKey(KeyboardKeyCode::F1);
	for (int frame = 0; frame < 5; ++frame) {
		controller.OnUpdate(_camera, 0.016f);
		EndFrame();
	}

	EXPECT_TRUE(Input::IsCursorCaptured());
}

/**
 * @brief Test that detaching releases a capture this controller made
 */
TEST_F(FreeCameraControllerTest, OnDetach_ReleasesItsOwnCapture) {
	auto controller = MakeController();
	controller.OnAttach(_camera);
	CaptureWith(controller);

	controller.OnDetach();

	EXPECT_EQ(Input::GetCursorMode(), CursorMode::Normal);
}

/**
 * @brief Test that detaching leaves alone a capture someone else made
 */
TEST_F(FreeCameraControllerTest, OnDetach_LeavesSomeoneElsesCapture) {
	auto controller = MakeController();
	controller.OnAttach(_camera);
	Input::SetCursorMode(CursorMode::Captured);

	controller.OnDetach();

	EXPECT_TRUE(Input::IsCursorCaptured());
}

// ============================================================================
// Look Tests
// ============================================================================

/**
 * @brief Test that with the default trigger the free cursor does not turn the camera
 */
TEST_F(FreeCameraControllerTest, CursorCapturedTrigger_IgnoresMotionWhileTheCursorIsFree) {
	auto controller = MakeController();
	controller.OnAttach(_camera);
	const glm::vec3 before = _camera.GetForward();

	MoveMouseRight(30.0f);
	controller.OnUpdate(_camera, 0.016f);

	EXPECT_FLOAT_EQ(_camera.GetForward().x, before.x);
	EXPECT_FLOAT_EQ(_camera.GetForward().z, before.z);
}

/**
 * @brief Test that once captured, moving the mouse right turns the camera right
 */
TEST_F(FreeCameraControllerTest, CursorCapturedTrigger_TurnsWhileCaptured) {
	auto controller = MakeController();
	controller.OnAttach(_camera);
	CaptureWith(controller);

	MoveMouseRight(30.0f);
	controller.OnUpdate(_camera, 0.016f);

	EXPECT_GT(_camera.GetForward().x, 0.0f);
}

/**
 * @brief Test that the ButtonHeld trigger turns the camera only while the look button is held
 */
TEST_F(FreeCameraControllerTest, ButtonHeldTrigger_TurnsOnlyWithTheButtonDown) {
	auto controller = MakeController({.move = {}, .look = {.sensitivity = 1.0f}, .lookTrigger = CameraLookTrigger::ButtonHeld});
	controller.OnAttach(_camera);

	MoveMouseRight(30.0f);
	controller.OnUpdate(_camera, 0.016f);
	EXPECT_NEAR(_camera.GetForward().x, 0.0f, 1.0e-6f);

	PressButton(MouseButtonCode::Right);
	EndFrame();
	MoveMouse(60.0f, 0.0f);
	controller.OnUpdate(_camera, 0.016f);

	EXPECT_GT(_camera.GetForward().x, 0.0f);
}

// ============================================================================
// Move Tests
// ============================================================================

/**
 * @brief Test that W flies forward at move.speed units per second
 */
TEST_F(FreeCameraControllerTest, W_FliesForwardAtTheDescriptorSpeed) {
	auto controller = MakeController({.move = {.speed = 2.0f}, .look = {}});
	controller.OnAttach(_camera);

	PressKey(KeyboardKeyCode::W);
	controller.OnUpdate(_camera, 0.5f);

	// An unrotated camera looks down -Z: one unit after half a second at two units per second.
	EXPECT_NEAR(_camera.GetPosition().z, -1.0f, 1.0e-5f);
}

/**
 * @brief Test that a wheel notch scales the cruise speed by speedScrollStep
 */
TEST_F(FreeCameraControllerTest, ScrollWheel_ScalesTheSpeed) {
	auto controller = MakeController({.move = {.speed = 1.0f}, .look = {}, .speedScrollStep = 2.0f});
	controller.OnAttach(_camera);

	ScrollWheel(1.0f);
	PressKey(KeyboardKeyCode::W);
	controller.OnUpdate(_camera, 1.0f);

	EXPECT_NEAR(_camera.GetPosition().z, -2.0f, 1.0e-5f);
}

/**
 * @brief Test that with an acceleration time the camera does not reach full speed on the first frame
 */
TEST_F(FreeCameraControllerTest, Acceleration_EasesIntoTheTargetSpeed) {
	auto controller = MakeController({.move = {.speed = 4.0f, .acceleration = 1.0f}, .look = {}});
	controller.OnAttach(_camera);

	PressKey(KeyboardKeyCode::W);
	controller.OnUpdate(_camera, 0.25f);

	// A quarter of the way to 4 units per second after a quarter of a second: 1 unit/s for 0.25 s.
	EXPECT_NEAR(_camera.GetPosition().z, -0.25f, 1.0e-5f);
}
