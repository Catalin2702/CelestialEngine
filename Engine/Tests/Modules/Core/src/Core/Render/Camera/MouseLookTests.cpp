//
// Module: CelestialEngine/Engine/Tests/Modules/Core/Render/Camera
// File: MouseLookTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-01
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Core/Render/Camera/Controller/MouseLook.hpp>

#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace {

using CE::Core::CameraLookSettings;
using CE::Core::MouseLook;

/**
 * @brief Where the camera looks: the forward axis (-Z) carried by the orientation
 */
glm::vec3 Forward(const MouseLook& look) {
	return look.GetRotation() * CE_WORLD_FORWARD;
}

}

// ============================================================================
// Direction Tests
// ============================================================================

/**
 * @brief Test that a fresh look faces down -Z, like an unrotated camera
 */
TEST(MouseLookTest, InitialState_LooksDownNegativeZ) {
	const MouseLook look{CameraLookSettings{}};

	EXPECT_TRUE(IsNearlyEqual(Forward(look).z, -1.0_f32));
}

/**
 * @brief Test that moving the mouse right turns the camera right
 * @details Screen X grows to the right while a positive yaw turns left: getting this sign wrong is the classic
 *			mouse-look bug, so it is pinned down here.
 */
TEST(MouseLookTest, MouseRight_TurnsRight) {
	MouseLook look{CameraLookSettings{.sensitivity = 1.0_f32}};
	look.Rotate(10.0_f32, 0.0_f32);

	EXPECT_GT(Forward(look).x, 0.0_f32);
	EXPECT_TRUE(IsNearlyEqual(look.GetYaw(), -10.0_f32));
}

/**
 * @brief Test that moving the mouse down looks down, and that invertY flips it
 */
TEST(MouseLookTest, MouseDown_LooksDownUnlessInverted) {
	MouseLook look{CameraLookSettings{.sensitivity = 1.0_f32}};
	look.Rotate(0.0_f32, 10.0_f32);

	EXPECT_LT(Forward(look).y, 0.0_f32);

	MouseLook inverted{CameraLookSettings{.sensitivity = 1.0_f32, .invertY = true}};
	inverted.Rotate(0.0_f32, 10.0_f32);

	EXPECT_GT(Forward(inverted).y, 0.0_f32);
}

// ============================================================================
// Constraint Tests
// ============================================================================

/**
 * @brief Test that the pitch stops at the limits instead of flipping over the pole
 */
TEST(MouseLookTest, Pitch_IsClampedToTheLimits) {
	MouseLook look{CameraLookSettings{.sensitivity = 1.0_f32}};
	look.Rotate(0.0_f32, -1000.0_f32);

	EXPECT_TRUE(IsNearlyEqual(look.GetPitch(), 89.0_f32));
}

/**
 * @brief Test that yaw and pitch together never roll the horizon
 * @details The camera right vector must stay horizontal: a y component would mean the horizon is tilted.
 */
TEST(MouseLookTest, YawAndPitch_KeepTheHorizonLevel) {
	MouseLook look{CameraLookSettings{.sensitivity = 1.0_f32}};

	for (int frame = 0; frame < 100; ++frame)
		look.Rotate(7.0_f32, -3.0_f32);

	const glm::vec3 right = look.GetRotation() * CE_WORLD_RIGHT;

	EXPECT_TRUE(IsNearlyZero(right.y));
}

/**
 * @brief Test that the yaw stays wrapped however far the mouse travels
 */
TEST(MouseLookTest, Yaw_StaysWithinOneTurn) {
	MouseLook look{CameraLookSettings{.sensitivity = 1.0_f32}};
	look.Rotate(1000.0_f32, 0.0_f32);

	EXPECT_LE(look.GetYaw(), 180.0_f32);
	EXPECT_GE(look.GetYaw(), -180.0_f32);
	EXPECT_TRUE(IsNearlyEqual(look.GetYaw(), 80.0_f32, 1.0e-4));
}

// ============================================================================
// Round-trip Tests
// ============================================================================

/**
 * @brief Test that starting from an existing orientation keeps the direction it looked in
 */
TEST(MouseLookTest, SetFromRotation_KeepsTheLookDirection) {
	MouseLook source{CameraLookSettings{.sensitivity = 1.0_f32}};
	source.Rotate(-35.0_f32, -20.0_f32);

	MouseLook look{CameraLookSettings{}};
	look.SetFromRotation(source.GetRotation());

	EXPECT_TRUE(IsNearlyEqual(look.GetYaw(), source.GetYaw(), 1.0e-4));
	EXPECT_TRUE(IsNearlyEqual(look.GetPitch(), source.GetPitch(), 1.0e-4));
}
