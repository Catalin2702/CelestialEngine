//
// Module: CelestialEngine/Tests/Engine/Modules/Utility/Time
// File: ChronometerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Utility/Time/Chronometer.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

using namespace CE::Utility;

namespace {

/**
 * @brief Duration the timed tests sleep for
 * @details Kept small on purpose: the tests assert the measure is at least the requested sleep, never that it is
 *			close to it. sleep_for only guarantees a minimum, and the kernel adds a leeway proportional to the
 *			requested interval (timer coalescing), so any upper bound would be flaky.
 */
constexpr auto stSleepDuration = std::chrono::milliseconds(2);

/**
 * @brief Sleeps for the test duration
 */
void SleepForTestDuration() { std::this_thread::sleep_for(stSleepDuration); }

}

namespace {
/**
 * @brief Test fixture for Chronometer tests
 * @details The destructor logs the measured duration in debug builds, so the logger must outlive every chronometer.
 */
class ChronometerTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};
}

// ============================================================================
// Lifecycle Tests
// ============================================================================

/**
 * @brief Test that a chronometer starts running
 */
TEST_F(ChronometerTest, Construction_StartsRunning) {
	const Chronometer chronometer;

	EXPECT_FALSE(chronometer.IsStopped());
}

/**
 * @brief Test that Stop freezes the measure
 */
TEST_F(ChronometerTest, Stop_FreezesTheMeasure) {
	Chronometer chronometer;
	SleepForTestDuration();
	chronometer.Stop();

	ASSERT_TRUE(chronometer.IsStopped());

	const auto firstRead = chronometer.GetElapsed();
	SleepForTestDuration();
	const auto secondRead = chronometer.GetElapsed();

	EXPECT_EQ(firstRead, secondRead);
}

/**
 * @brief Test that stopping twice keeps the first measure
 * @details The destructor stops the chronometer too, so an early Stop() must not be overwritten.
 */
TEST_F(ChronometerTest, Stop_IsIdempotent) {
	Chronometer chronometer;
	chronometer.Stop();

	const auto stoppedAt = chronometer.GetElapsed();

	SleepForTestDuration();
	chronometer.Stop();

	EXPECT_EQ(chronometer.GetElapsed(), stoppedAt);
}

/**
 * @brief Test that destroying a chronometer does not throw
 */
TEST_F(ChronometerTest, Destruction_DoesNotThrow) {
	EXPECT_NO_THROW({
		const Chronometer chronometer;
		SleepForTestDuration();
	});
}

// ============================================================================
// Measurement Tests
// ============================================================================

/**
 * @brief Test that a running chronometer keeps accumulating time
 */
TEST_F(ChronometerTest, GetElapsed_WhileRunning_KeepsGrowing) {
	const Chronometer chronometer;

	const auto firstRead = chronometer.GetElapsed();
	SleepForTestDuration();
	const auto secondRead = chronometer.GetElapsed();

	EXPECT_GT(secondRead, firstRead);
	EXPECT_GE(secondRead, stSleepDuration);
}

/**
 * @brief Test that the measure covers at least the slept duration
 */
TEST_F(ChronometerTest, GetElapsed_MeasuresAtLeastTheSleptDuration) {
	Chronometer chronometer;
	SleepForTestDuration();
	chronometer.Stop();

	EXPECT_GE(chronometer.GetElapsed(), stSleepDuration);
}

/**
 * @brief Test that a chronometer stopped immediately reports a non-negative, tiny duration
 */
TEST_F(ChronometerTest, GetElapsed_WithoutWork_IsNonNegativeAndSmall) {
	Chronometer chronometer;
	chronometer.Stop();

	EXPECT_GE(chronometer.GetElapsed(), std::chrono::nanoseconds::zero());
	EXPECT_LT(chronometer.GetElapsed(), std::chrono::milliseconds(100));
}

/**
 * @brief Test that the millisecond getter reports the same measure as the nanosecond one
 */
TEST_F(ChronometerTest, GetElapsedMilliseconds_MatchesGetElapsed) {
	Chronometer chronometer;
	SleepForTestDuration();
	chronometer.Stop();

	const auto expectedMilliseconds = std::chrono::duration<double, std::milli>(chronometer.GetElapsed()).count();

	EXPECT_DOUBLE_EQ(chronometer.GetElapsedMilliseconds(), expectedMilliseconds);
}

/**
 * @brief Test that the millisecond getter keeps the sub-millisecond fraction
 * @details A chronometer stopped right away measures well under a millisecond: converting through whole
 *			milliseconds would flatten it to zero and hide everything shorter than a tick. The nanosecond measure is
 *			the reference, so the conversion has to be exact whatever its value.
 */
TEST_F(ChronometerTest, GetElapsedMilliseconds_KeepsSubMillisecondPrecision) {
	Chronometer chronometer;
	chronometer.Stop();

	const auto elapsed = chronometer.GetElapsed();
	ASSERT_LT(elapsed, std::chrono::milliseconds(1));

	const auto expectedMilliseconds = std::chrono::duration<double, std::milli>(elapsed).count();

	EXPECT_DOUBLE_EQ(chronometer.GetElapsedMilliseconds(), expectedMilliseconds);
}

// ============================================================================
// Independence Tests
// ============================================================================

/**
 * @brief Test that chronometers used in sequence measure their own scope
 */
TEST_F(ChronometerTest, MultipleChronometers_Sequential_MeasureTheirOwnScope) {
	Chronometer first;
	SleepForTestDuration();
	first.Stop();

	Chronometer second;
	second.Stop();

	EXPECT_GE(first.GetElapsed(), stSleepDuration);
	EXPECT_LT(second.GetElapsed(), first.GetElapsed());
}

/**
 * @brief Test that a nested chronometer measures less than the one that encloses it
 */
TEST_F(ChronometerTest, NestedChronometers_InnerMeasuresLessThanOuter) {
	Chronometer outer;

	std::chrono::nanoseconds innerElapsed{};
	{
		Chronometer inner;
		SleepForTestDuration();
		inner.Stop();
		innerElapsed = inner.GetElapsed();
	}

	SleepForTestDuration();
	outer.Stop();

	EXPECT_GE(innerElapsed, stSleepDuration);
	EXPECT_GT(outer.GetElapsed(), innerElapsed);
}
