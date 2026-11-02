//
// Module: CelestialEngine/Engine/Tests/Modules/Tools/Log
// File: LogTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

using namespace CE::Tools;

namespace {

/**
 * @brief Test fixture for Log tests
 */
class LogTest: public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {}
};

}

/**
 * @brief Test that Log can be initialized and shutdown without errors
 */
TEST_F(LogTest, Init_CallInitAndShutdown_NoErrors) {
	EXPECT_NO_THROW({
		Log::Init();
		Log::Shutdown();
	});
}

TEST_F(LogTest, Init_CallInitMultipleTimes_NoErrors) {
	EXPECT_NO_THROW({
		Log::Init();
		Log::Init();
		Log::Init();
	});
	Log::Shutdown();
}

/**
 * @brief Test that Log can be shutdown without initialization and does not throw errors
 */
TEST_F(LogTest, Init_CallShutdownMultipleTimes_NoErrors) {
	Log::Init();
	EXPECT_NO_THROW({
		Log::Shutdown();
		Log::Shutdown();
		Log::Shutdown();
	});
}

/**
 * @brief Test that Log can be shutdown multiple times without initialization and does not throw errors
 */
TEST_F(LogTest, Init_CallShutdownMultipleTimesWithoutInit_NoErrors) {
	EXPECT_NO_THROW({
		Log::Shutdown();
		Log::Shutdown();
		Log::Shutdown();
	});
}

/**
 * @brief Test that Init and Terminate can be called multiple times
 */
TEST_F(LogTest, InitShutdown_MultipleCalls_NoErrors) {
	EXPECT_NO_THROW({
		Log::Init();
		Log::Shutdown();
		Log::Init();
		Log::Shutdown();
	});
}

/**
 * @brief Test that core logger is available after initialization
 */
TEST_F(LogTest, GetCoreLogger_AfterInit_ReturnsValidLogger) {
	Log::Init();
	const auto& logger = Log::GetCoreLogger();

	EXPECT_NE(logger, nullptr);
	Log::Shutdown();
}

/**
 * @brief Test that client logger is available after initialization
 */
TEST_F(LogTest, GetClientLogger_AfterInit_ReturnsValidLogger) {
	Log::Init();
	const auto& logger = Log::GetClientLogger();

	EXPECT_NE(logger, nullptr);
	Log::Shutdown();
}

/**
 * @brief Test that both loggers are different instances
 */
TEST_F(LogTest, GetLoggers_CoreAndClient_AreDifferent) {
	Log::Init();
	const auto& coreLogger = Log::GetCoreLogger();
	const auto& clientLogger = Log::GetClientLogger();

	EXPECT_NE(coreLogger, clientLogger);
	Log::Shutdown();
}

/**
 * @brief Test that core logger can log messages
 */
TEST_F(LogTest, CoreLogger_LogMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetCoreLogger();

	EXPECT_NO_THROW({
		if (logger) {
			logger->trace("Test trace message");
			logger->debug("Test debug message");
			logger->info("Test info message");
			logger->warn("Test warning message");
			logger->error("Test error message");
		}
	});
}

/**
 * @brief Test that client logger can log messages
 */
TEST_F(LogTest, ClientLogger_LogMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetClientLogger();

	EXPECT_NO_THROW({
		if (logger) {
			logger->trace("Test trace message");
			logger->debug("Test debug message");
			logger->info("Test info message");
			logger->warn("Test warning message");
			logger->error("Test error message");
		}
	});
}

/**
 * @brief Test that logger can handle formatted messages
 */
TEST_F(LogTest, Logger_FormattedMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetCoreLogger();

	EXPECT_NO_THROW({
		if (logger) {
			logger->info("Formatted message: {} {} {}", 1, 2.5, "test");
			logger->info("Number: {0}, String: {1}", 42, "value");
		}
	});
}

/**
 * @brief Test terminate without init
 */
TEST_F(LogTest, Shutdown_WithoutInit_NoErrors) {
	EXPECT_NO_THROW(Log::Shutdown());
}

/**
 * @brief Test getting logger before init
 */
TEST_F(LogTest, GetLogger_BeforeInit_ReturnsLogger) {
	// Should not crash even if called before Init
	EXPECT_NO_THROW({
		const auto& coreLogger = Log::GetCoreLogger();
		const auto& clientLogger = Log::GetClientLogger();
		(void)coreLogger;
		(void)clientLogger;
	});
}

/**
 * @brief Test that every configuration has loggers, and that Dist keeps only the critical level
 * @details Dist used to run with no logger at all; it now writes CRITICAL to stderr and to a file, so both loggers exist
 *			there too - set to critical, while Debug and Release log everything the macros let through.
 */
TEST_F(LogTest, Loggers_LevelFollowsTheConfiguration) {
	Log::Init();

	const auto& coreLogger = Log::GetCoreLogger();
	const auto& clientLogger = Log::GetClientLogger();
	ASSERT_NE(coreLogger, nullptr);
	ASSERT_NE(clientLogger, nullptr);

	constexpr auto expected = CE_DIST ? spdlog::level::critical : spdlog::level::trace;
	EXPECT_EQ(coreLogger->level(), expected);
	EXPECT_EQ(clientLogger->level(), expected);

	Log::Shutdown();
}

/**
 * @brief Test that the critical path used by the terminate handler logs without throwing, in every configuration
 */
TEST_F(LogTest, LogCoreCritical_AfterInit_NoErrors) {
	Log::Init();

	EXPECT_NO_THROW(Log::LogCoreCritical("LogTest: critical message, expected in the test output"));
	EXPECT_NO_THROW(Log::Flush());

	Log::Shutdown();
}

/**
 * @brief Test that the critical path is a no-op, not a crash, before Init
 * @details Asserted in Debug as a caller's bug, so only checked where asserts are compiled out.
 */
#if not CE_DEBUG
TEST_F(LogTest, LogCoreCritical_BeforeInit_DoesNothing) {
	Log::Shutdown();

	EXPECT_NO_THROW(Log::LogCoreCritical("never written"));
}
#endif

/**
 * @brief Test logger thread safety with multiple accesses
 */
TEST_F(LogTest, Logger_MultipleAccesses_ThreadSafe) {
	Log::Init();

	for (int i = 0; i < 10; ++i) {
		const auto& coreLogger = Log::GetCoreLogger();
		const auto& clientLogger = Log::GetClientLogger();

		if (coreLogger) {
			EXPECT_NO_THROW(coreLogger->info("Core message {}", i));
		}
		if (clientLogger) {
			EXPECT_NO_THROW(clientLogger->info("Client message {}", i));
		}
	}
}

/**
 * @brief Test that every logging macro expands and runs in every configuration
 * @details The levels a configuration removes expand to an empty statement, arguments included, so this mostly checks
 *			that the macros stay well-formed in all three. CE_CORE_* is available here because the tests link
 *			CE_Internal, as the engine modules do.
 */
TEST_F(LogTest, Macros_ExpandInEveryConfiguration) {
	Log::Init();

	EXPECT_NO_THROW({
		CE_TRACE("LogTest: client trace {}", 1);
		CE_INFO("LogTest: client info {}", 2);
		CE_WARN("LogTest: client warning {}", 3);
		CE_ERROR("LogTest: client error {}", 4);
		CE_CRITICAL("LogTest: client critical {}, expected in the test output", 5);

		CE_CORE_TRACE("LogTest: core trace {}", 1);
		CE_CORE_INFO("LogTest: core info {}", 2);
		CE_CORE_WARN("LogTest: core warning {}", 3);
		CE_CORE_ERROR("LogTest: core error {}", 4);
		CE_CORE_CRITICAL("LogTest: core critical {}, expected in the test output", 5);
	});

	Log::Shutdown();
}
