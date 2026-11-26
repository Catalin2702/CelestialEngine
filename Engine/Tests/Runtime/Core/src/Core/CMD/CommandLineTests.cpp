//
// Module: CelestialEngine/Engine/Tests/Runtime/Core/CMD
// File: CommandLineTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <Core/CMD/CommandLine.hpp>
#include <Core/Config/Config.hpp>

#include <Tools/Tools.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace CE::Core;
using namespace CE::Tools;
using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for InitFromCommandLine
 * @details Config is process-wide: each test starts from a known value of it, so the order the tests run in does not
 *			matter.
 */
class CommandLineTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
		Config::SetWindowProps(WindowProps{"Before", 640, 480, false, 0, GetDefaultGraphicsApi(), GetDefaultWindowApi(GetDefaultGraphicsApi())});
	}

	void TearDown() override { Log::Shutdown(); }
};

}

/**
 * @brief Test that the parsed window options end up in Config, titled with the name main() passes
 */
TEST_F(CommandLineTest, InitFromCommandLine_ValidArguments_StoresWindowPropsInConfig) {
	const char* argv[] = {"program", "-w", "800", "-h", "600"};

	InitFromCommandLine(5, argv, "Application Name");

	const auto props = Config::GetWindowProps();
	EXPECT_EQ(props.title, "Application Name");
	EXPECT_EQ(props.width, 800);
	EXPECT_EQ(props.height, 600);
	EXPECT_EQ(props.graphicsApi, GetDefaultGraphicsApi());
}

/**
 * @brief Test that a graphics API this build lacks throws, and leaves Config as it was
 * @details Vulkan is not implemented yet, so no build compiles it in (CERenderBackends.cmake refuses it).
 */
TEST_F(CommandLineTest, InitFromCommandLine_GraphicsApiNotCompiled_ThrowsAndStoresNothing) {
	const char* argv[] = {"program", "-g", "vulkan"};

	EXPECT_THROW(InitFromCommandLine(3, argv, "Rejected"), std::runtime_error);
	EXPECT_EQ(Config::GetWindowProps().title, "Before");
}

/**
 * @brief Test that an invalid option value throws, and leaves Config as it was
 */
TEST_F(CommandLineTest, InitFromCommandLine_InvalidValue_ThrowsAndStoresNothing) {
	const char* argv[] = {"program", "-w", "wide"};

	EXPECT_THROW(InitFromCommandLine(3, argv, "Rejected"), std::runtime_error);
	EXPECT_EQ(Config::GetWindowProps().width, 640);
}

/**
 * @brief Test that the command line cannot change the title
 * @details "-t" is no longer an option: the window is always titled with the application's display name.
 */
TEST_F(CommandLineTest, InitFromCommandLine_TitleOnCommandLine_KeepsApplicationName) {
	const char* argv[] = {"program", "-t", "FromArgs"};

	InitFromCommandLine(3, argv, "Application Name");

	EXPECT_EQ(Config::GetWindowProps().title, "Application Name");
}
