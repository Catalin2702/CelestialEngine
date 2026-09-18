//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Command
// File: ViewportTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#include <Core/Render/Command/Viewport.hpp>
#include <Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

using CE::Core::ToOpenGlViewport;
using CE::Core::Viewport;

namespace {

/**
 * @brief Test fixture for the viewport and the one conversion a backend applies to it
 * @details A Viewport is a plain rectangle now, and turning it round for a bottom-left backend is pure arithmetic
 *			over the numbers it is handed - no device, no context - so both are fully testable headlessly. The log is
 *			brought up because the conversion warns on the one input it cannot convert.
 */
class ViewportTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

// ============================================================================
// Identity Tests
// ============================================================================

/**
 * @brief Test that a viewport holds exactly the rectangle it was given, in the caller's own convention
 * @details The whole point of the struct being plain: it names no backend and converts nothing, so what a caller
 *			writes into it is what any encoder reads back out.
 */
TEST_F(ViewportTest, HoldsTheRectangleItWasGiven) {
	constexpr Viewport viewport{.x = 10.f, .y = 20.f, .width = 300.f, .height = 200.f};

	EXPECT_FLOAT_EQ(viewport.x, 10.f);
	EXPECT_FLOAT_EQ(viewport.y, 20.f);
	EXPECT_FLOAT_EQ(viewport.width, 300.f);
	EXPECT_FLOAT_EQ(viewport.height, 200.f);
}

/**
 * @brief Test that a default-constructed viewport is empty and covers the whole depth range
 */
TEST_F(ViewportTest, DefaultConstructed_IsEmptyOverTheWholeDepthRange) {
	constexpr Viewport viewport{};

	EXPECT_FLOAT_EQ(viewport.x, 0.f);
	EXPECT_FLOAT_EQ(viewport.y, 0.f);
	EXPECT_FLOAT_EQ(viewport.width, 0.f);
	EXPECT_FLOAT_EQ(viewport.height, 0.f);
	EXPECT_FLOAT_EQ(viewport.minDepth, 0.f);
	EXPECT_FLOAT_EQ(viewport.maxDepth, 1.f);
}

// ============================================================================
// Origin Conversion Tests
// ============================================================================

/**
 * @brief Test that the conversion measures y from the bottom of the target instead of the top
 * @details 720 - 20 - 200 = 500: the same rectangle, expressed from the other end. Only y moves - a flipped width or
 *			height would be a different rectangle, not a different origin.
 */
TEST_F(ViewportTest, ToOpenGl_FlipsTheOriginAgainstTheTargetHeight) {
	constexpr Viewport viewport{.x = 10.f, .y = 20.f, .width = 300.f, .height = 200.f};

	const auto converted = ToOpenGlViewport(viewport, 720);

	EXPECT_FLOAT_EQ(converted.x, 10.f);
	EXPECT_FLOAT_EQ(converted.y, 500.f);
	EXPECT_FLOAT_EQ(converted.width, 300.f);
	EXPECT_FLOAT_EQ(converted.height, 200.f);
}

/**
 * @brief Test that a full-target rectangle comes out at the origin
 * @details The case the renderer actually builds every frame, and the one where a wrong flip is invisible: a
 *			full-target rectangle is its own mirror image, so it has to be checked alongside an off-centre one.
 */
TEST_F(ViewportTest, ToOpenGl_FullTargetLandsAtTheOrigin) {
	constexpr Viewport viewport{.x = 0.f, .y = 0.f, .width = 1280.f, .height = 720.f};

	const auto converted = ToOpenGlViewport(viewport, 720);

	EXPECT_FLOAT_EQ(converted.x, 0.f);
	EXPECT_FLOAT_EQ(converted.y, 0.f);
}

/**
 * @brief Test that converting twice returns the original rectangle
 */
TEST_F(ViewportTest, ToOpenGl_FlipIsItsOwnInverse) {
	constexpr Viewport viewport{.x = 0.f, .y = 20.f, .width = 300.f, .height = 200.f};

	const auto once = ToOpenGlViewport(viewport, 720);
	const auto twice = ToOpenGlViewport(once, 720);

	EXPECT_FLOAT_EQ(twice.y, 20.f);
}

/**
 * @brief Test that the depth range is carried through the conversion untouched
 * @details Never flipped: the origin conversion is about the target's pixels, and depth is not one of them. A
 *			reversed-Z pass asks for 1..0 and must get exactly that.
 */
TEST_F(ViewportTest, ToOpenGl_CarriesTheDepthRangeThroughUnchanged) {
	constexpr Viewport reversed{.x = 0.f, .y = 0.f, .width = 1.f, .height = 1.f, .minDepth = 1.f, .maxDepth = 0.f};

	const auto converted = ToOpenGlViewport(reversed, 720);

	EXPECT_FLOAT_EQ(converted.minDepth, 1.f);
	EXPECT_FLOAT_EQ(converted.maxDepth, 0.f);
}

// ============================================================================
// Degenerate Input Tests
// ============================================================================

/**
 * @brief Test that a conversion with no target height leaves the rectangle unflipped rather than making it negative
 * @details The flip needs the target's height; without it the only choices are a wrong answer and the caller's own
 *			numbers. It keeps the caller's, and warns.
 */
TEST_F(ViewportTest, ToOpenGl_WithoutATargetHeight_LeavesTheRectangleUnflipped) {
	constexpr Viewport viewport{.x = 10.f, .y = 20.f, .width = 300.f, .height = 200.f};

	const auto converted = ToOpenGlViewport(viewport, 0);

	EXPECT_FLOAT_EQ(converted.x, 10.f);
	EXPECT_FLOAT_EQ(converted.y, 20.f);
	EXPECT_FLOAT_EQ(converted.width, 300.f);
	EXPECT_FLOAT_EQ(converted.height, 200.f);
}
