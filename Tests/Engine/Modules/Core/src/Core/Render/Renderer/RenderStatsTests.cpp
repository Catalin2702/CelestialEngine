//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Renderer
// File: RenderStatsTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Core/Render/Renderer/RenderStats.hpp>

#include <gtest/gtest.h>

using CE::Core::RenderStats;

namespace {

/**
 * @brief Test fixture for the per-frame counters
 */
class RenderStatsTest: public ::testing::Test {};

}

/**
 * @brief Test that a fresh RenderStats counts nothing
 * @details The renderer resets rather than reassigns at the top of a frame, so the default has to be the empty
 *			frame - a non-zero default would be added to every frame's real total.
 */
TEST_F(RenderStatsTest, DefaultConstructed_CountsNothing) {
	constexpr RenderStats stats{};

	EXPECT_EQ(stats.drawCalls, 0u);
	EXPECT_EQ(stats.passes, 0u);
	EXPECT_EQ(stats.indices, 0u);
}

/**
 * @brief Test that Reset clears every counter, not only the ones the renderer happens to touch
 */
TEST_F(RenderStatsTest, Reset_ClearsEveryCounter) {
	RenderStats stats{};
	stats.drawCalls = 12;
	stats.passes = 3;
	stats.indices = 4096;

	stats.Reset();

	EXPECT_EQ(stats.drawCalls, 0u);
	EXPECT_EQ(stats.passes, 0u);
	EXPECT_EQ(stats.indices, 0u);
}

/**
 * @brief Test that the index counter is wide enough for a frame a 32-bit one would wrap
 * @details Indices are counted across every draw of a frame, and a scene of a few million triangles passes 4
 *			billion indices in well under a minute of accumulated frames if the type is ever narrowed.
 */
TEST_F(RenderStatsTest, IndexCounter_HoldsMoreThanAThirtyTwoBitCount) {
	RenderStats stats{};
	stats.indices = 5'000'000'000ull;

	EXPECT_EQ(stats.indices, 5'000'000'000ull);
}

/**
 * @brief Test that stats are copyable, which is how a finished frame is published
 * @details The renderer keeps two: one accumulating and one published. EndFrame copies the first into the second so
 *			a UI reading it sees the same numbers all through the next frame.
 */
TEST_F(RenderStatsTest, IsCopyable_SoAFinishedFrameCanBePublished) {
	RenderStats inFlight{};
	inFlight.drawCalls = 7;
	inFlight.passes = 2;
	inFlight.indices = 900;

	const RenderStats published = inFlight;
	inFlight.Reset();

	EXPECT_EQ(published.drawCalls, 7u);
	EXPECT_EQ(published.passes, 2u);
	EXPECT_EQ(published.indices, 900u);
	EXPECT_EQ(inFlight.drawCalls, 0u);
}
