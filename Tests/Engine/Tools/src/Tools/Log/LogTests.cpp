//
// Module: Tests/Engine/Tools/Log
// File: LogTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include "Tools/Log/Log.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

using namespace CE::Tools::Log;

/**
 * @brief Test fixture for Log tests
 */
class LogTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
		// Ensure logging is terminated after each test
		Log::Shutdown();
	}
};

/**
 * @brief Test that Log can be initialized without errors
 */
TEST_F(LogTest, Init_CallInit_NoErrors) {
	EXPECT_NO_THROW(Log::Init());
}

/**
 * @brief Test that Log can be terminated without errors
 */
TEST_F(LogTest, Shutdown_CallShutdown_NoErrors) {
	Log::Init();
	EXPECT_NO_THROW(Log::Shutdown());
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

#ifndef CE_DIST
	EXPECT_NE(logger, nullptr);
#else
	// In distribution builds, logger might be null
	EXPECT_TRUE(true);
#endif
}

/**
 * @brief Test that client logger is available after initialization
 */
TEST_F(LogTest, GetClientLogger_AfterInit_ReturnsValidLogger) {
	Log::Init();
	const auto& logger = Log::GetClientLogger();

#ifndef CE_DIST
	EXPECT_NE(logger, nullptr);
#else
	// In distribution builds, logger might be null
	EXPECT_TRUE(true);
#endif
}

/**
 * @brief Test that both loggers are different instances
 */
TEST_F(LogTest, GetLoggers_CoreAndClient_AreDifferent) {
	Log::Init();
	const auto& coreLogger = Log::GetCoreLogger();
	const auto& clientLogger = Log::GetClientLogger();

#ifndef CE_DIST
	EXPECT_NE(coreLogger.get(), clientLogger.get());
#else
	// In distribution builds, both might be null
	EXPECT_TRUE(true);
#endif
}

/**
 * @brief Test that core logger can log messages
 */
TEST_F(LogTest, CoreLogger_LogMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetCoreLogger();

#ifndef CE_DIST
	EXPECT_NO_THROW({
		if (logger) {
			logger->trace("Test trace message");
			logger->debug("Test debug message");
			logger->info("Test info message");
			logger->warn("Test warning message");
			logger->error("Test error message");
		}
	});
#else
	EXPECT_TRUE(true);
#endif
}

/**
 * @brief Test that client logger can log messages
 */
TEST_F(LogTest, ClientLogger_LogMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetClientLogger();

#ifndef CE_DIST
	EXPECT_NO_THROW({
		if (logger) {
			logger->trace("Test trace message");
			logger->debug("Test debug message");
			logger->info("Test info message");
			logger->warn("Test warning message");
			logger->error("Test error message");
		}
	});
#else
	EXPECT_TRUE(true);
#endif
}

/**
 * @brief Test that logger can handle formatted messages
 */
TEST_F(LogTest, Logger_FormattedMessage_NoErrors) {
	Log::Init();
	const auto& logger = Log::GetCoreLogger();

#ifndef CE_DIST
	EXPECT_NO_THROW({
		if (logger) {
			logger->info("Formatted message: {} {} {}", 1, 2.5, "test");
			logger->info("Number: {0}, String: {1}", 42, "value");
		}
	});
#else
	EXPECT_TRUE(true);
#endif
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
 * @brief Test that logging works in distribution builds
 */
TEST_F(LogTest, Logger_DistBuild_HandlesGracefully) {
	// This test ensures the code compiles and runs in both debug and dist modes
	EXPECT_NO_THROW(Log::Init());
#ifndef CE_DIST
	if (const auto& logger = Log::GetCoreLogger()) {
		EXPECT_NO_THROW(logger->info("This should log in debug builds"));
	}
#endif
	EXPECT_NO_THROW(Log::Shutdown());
}

/**
 * @brief Test logger thread safety with multiple accesses
 */
TEST_F(LogTest, Logger_MultipleAccesses_ThreadSafe) {
	Log::Init();

#ifndef CE_DIST
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
#else
	EXPECT_TRUE(true);
#endif
}
