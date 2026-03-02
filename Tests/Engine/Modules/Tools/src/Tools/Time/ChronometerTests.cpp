//
// Module: Tests/Engine/Modules/Tools/Time
// File: ChronometerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Tools/Log/Log.hpp>
#include <Tools/Time/Chronometer.hpp>

#include <chrono>
#include <thread>
#include <gtest/gtest.h>

using namespace CE::Tools::Time;

/**
 * @brief Test fixture for Chronometer tests
 */
class ChronometerTest: public ::testing::Test {
protected:
	void SetUp() override {
		// Initialize logging for Chronometer output
		// Logger cleanup is now handled internally by Log::Init()
		CE::Tools::Log::Log::Init();
	}

	void TearDown() override {
		// Clean up logging system
		// Logger cleanup is now handled internally by Log::Shutdown()
		CE::Tools::Log::Log::Shutdown();
	}
};

/**
 * @brief Test that Chronometer can be constructed and destroyed
 */
TEST_F(ChronometerTest, Constructor_ValidConstruction_Succeeds) {
	EXPECT_NO_THROW({
		Chronometer timer;
	});
}

/**
 * @brief Test that Chronometer measures time for a short duration
 */
TEST_F(ChronometerTest, Chronometer_ShortDuration_MeasuresTime) {
	EXPECT_NO_THROW({
		Chronometer timer;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});
}

/**
 * @brief Test that Chronometer measures time for a longer duration
 */
TEST_F(ChronometerTest, Chronometer_LongerDuration_MeasuresTime) {
	EXPECT_NO_THROW({
		Chronometer timer;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});
}

/**
 * @brief Test that multiple Chronometers can be used in sequence
 */
TEST_F(ChronometerTest, MultipleChronometers_Sequential_WorksCorrectly) {
	EXPECT_NO_THROW({
		{
			Chronometer timer1;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		{
			Chronometer timer2;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});
}

/**
 * @brief Test that Chronometer works in nested scopes
 */
TEST_F(ChronometerTest, Chronometer_NestedScopes_WorksCorrectly) {
	EXPECT_NO_THROW({
		Chronometer outerTimer;
		{
			Chronometer innerTimer;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});
}

/**
 * @brief Test that Chronometer handles zero duration
 */
TEST_F(ChronometerTest, Chronometer_ZeroDuration_HandlesGracefully) {
	EXPECT_NO_THROW({
		Chronometer timer;
		// No sleep, immediate destruction
	});
}

/**
 * @brief Test that Chronometer works with function scope
 */
TEST_F(ChronometerTest, Chronometer_FunctionScope_MeasuresCorrectly) {
	auto testFunction = []() {
		Chronometer timer;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	};

	EXPECT_NO_THROW(testFunction());
}

