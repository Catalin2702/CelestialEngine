//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Command
// File: ViewportTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Core/Render/Command/Viewport.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

using CE::Core::Viewport;
using CE::Types::GraphicsApi;

namespace {

/**
 * @brief Test fixture for the viewport conversion
 * @details A Viewport is pure arithmetic over the numbers it is handed - no device, no encoder - so the one rule it
 *			carries, "who flips the origin", is fully testable headlessly. The log is brought up because the
 *			constructor warns on the two cases that cannot be converted.
 */
class ViewportTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

// ============================================================================
// Origin Conversion Tests
// ============================================================================

/**
 * @brief Test that a Metal viewport keeps the caller's top-left rectangle untouched
 */
TEST_F(ViewportTest, Metal_KeepsTheTopLeftRectangle) {
	const Viewport viewport{GraphicsApi::Metal, 10.f, 20.f, 300.f, 200.f, 720.f};

	EXPECT_FLOAT_EQ(viewport.x, 10.f);
	EXPECT_FLOAT_EQ(viewport.y, 20.f);
	EXPECT_FLOAT_EQ(viewport.width, 300.f);
	EXPECT_FLOAT_EQ(viewport.height, 200.f);
}

/**
 * @brief Test that an OpenGL viewport measures y from the bottom of the target instead
 * @details 720 - 20 - 200 = 500: the same rectangle, expressed from the other end. Only y moves - a flipped width or
 *			height would be a different rectangle, not a different origin.
 */
TEST_F(ViewportTest, OpenGl_FlipsTheOriginAgainstTheTargetHeight) {
	const Viewport viewport{GraphicsApi::OpenGL, 10.f, 20.f, 300.f, 200.f, 720.f};

	EXPECT_FLOAT_EQ(viewport.x, 10.f);
	EXPECT_FLOAT_EQ(viewport.y, 500.f);
	EXPECT_FLOAT_EQ(viewport.width, 300.f);
	EXPECT_FLOAT_EQ(viewport.height, 200.f);
}

/**
 * @brief Test that a full-target OpenGL viewport comes out at the origin
 * @details The case the renderer actually builds every frame, and the one where a wrong flip is invisible: a
 *			full-target rectangle is its own mirror image, so it has to be checked alongside an off-centre one.
 */
TEST_F(ViewportTest, OpenGl_FullTargetLandsAtTheOrigin) {
	const Viewport viewport{GraphicsApi::OpenGL, 0.f, 0.f, 1280.f, 720.f, 720.f};

	EXPECT_FLOAT_EQ(viewport.x, 0.f);
	EXPECT_FLOAT_EQ(viewport.y, 0.f);
}

/**
 * @brief Test that flipping twice returns the original rectangle
 */
TEST_F(ViewportTest, OpenGl_FlipIsItsOwnInverse) {
	constexpr auto targetHeight = 720.f;
	const Viewport once{GraphicsApi::OpenGL, 0.f, 20.f, 300.f, 200.f, targetHeight};
	const Viewport twice{GraphicsApi::OpenGL, 0.f, once.y, 300.f, 200.f, targetHeight};

	EXPECT_FLOAT_EQ(twice.y, 20.f);
}

// ============================================================================
// Degenerate Input Tests
// ============================================================================

/**
 * @brief Test that an OpenGL viewport with no target height is left unflipped rather than made negative
 * @details The flip needs the target's height; without it the only choices are a wrong answer and the caller's own
 *			numbers. It keeps the caller's, and warns.
 */
TEST_F(ViewportTest, OpenGl_WithoutATargetHeight_LeavesTheRectangleUnflipped) {
	const Viewport viewport{GraphicsApi::OpenGL, 10.f, 20.f, 300.f, 200.f, 0.f};

	EXPECT_FLOAT_EQ(viewport.y, 20.f);
}

/**
 * @brief Test that GraphicsApi::None passes the rectangle through untouched
 */
TEST_F(ViewportTest, None_PassesTheRectangleThrough) {
	const Viewport viewport{GraphicsApi::None, 10.f, 20.f, 300.f, 200.f, 720.f};

	EXPECT_FLOAT_EQ(viewport.y, 20.f);
}

// ============================================================================
// Identity Tests
// ============================================================================

/**
 * @brief Test that a viewport remembers which backend it was converted for
 * @details The whole reason GetGraphicsApi exists: the members alone cannot say whether y has been flipped, so an
 *			encoder handed a viewport built for another API has to be able to assert.
 */
TEST_F(ViewportTest, RemembersTheApiItWasBuiltFor) {
	EXPECT_EQ(Viewport(GraphicsApi::Metal, 0.f, 0.f, 1.f, 1.f, 1.f).GetGraphicsApi(), GraphicsApi::Metal);
	EXPECT_EQ(Viewport(GraphicsApi::OpenGL, 0.f, 0.f, 1.f, 1.f, 1.f).GetGraphicsApi(), GraphicsApi::OpenGL);
}

/**
 * @brief Test that a default-constructed viewport is empty and bound to no backend
 */
TEST_F(ViewportTest, DefaultConstructed_IsEmptyAndApiLess) {
	constexpr Viewport viewport{};

	EXPECT_FLOAT_EQ(viewport.width, 0.f);
	EXPECT_FLOAT_EQ(viewport.height, 0.f);
	EXPECT_FLOAT_EQ(viewport.minDepth, 0.f);
	EXPECT_FLOAT_EQ(viewport.maxDepth, 1.f);
	EXPECT_EQ(viewport.GetGraphicsApi(), GraphicsApi::None);
}

// ============================================================================
// Depth Range Tests
// ============================================================================

/**
 * @brief Test that the depth range defaults to 0..1 and is carried through unchanged
 * @details Never flipped, whatever the backend: the origin conversion is about the target's pixels, and depth is not
 *			one of them. A reversed-Z pass asks for 1..0 and must get exactly that.
 */
TEST_F(ViewportTest, DepthRange_IsCarriedThroughOnEveryBackend) {
	const Viewport standard{GraphicsApi::OpenGL, 0.f, 0.f, 1.f, 1.f, 1.f};
	EXPECT_FLOAT_EQ(standard.minDepth, 0.f);
	EXPECT_FLOAT_EQ(standard.maxDepth, 1.f);

	const Viewport reversed{GraphicsApi::OpenGL, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f};
	EXPECT_FLOAT_EQ(reversed.minDepth, 1.f);
	EXPECT_FLOAT_EQ(reversed.maxDepth, 0.f);
}
