//
// Module: CelestialEngine/Tests/Engine/Modules/Utility/Config
// File: ConfigTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Utility/Config/Config.hpp>

#include <Types/Render/Render.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>

using CE::Utility::Config;
using CE::Types::GraphicsApi;
using CE::Types::WindowApi;
using CE::Types::WindowProps;

namespace {

/**
 * @brief Test fixture for Config tests
 * @details Config is a process-wide singleton created lazily by the first StSetWindowProps call: the window and the
 *			render context read their configuration from it instead of receiving it as a constructor argument.
 *			Each test therefore starts by writing a known configuration.
 */
class ConfigTest: public ::testing::Test {
protected:
	void SetUp() override {
		Config::StSetWindowProps(WindowProps{"ConfigTest", 800, 600, false, 0, GraphicsApi::OpenGL, WindowApi::GLFW});
	}
};

}

/**
 * @brief Test that the window properties written are the ones read back
 */
TEST_F(ConfigTest, StSetWindowProps_StoresProperties) {
	const auto& props = Config::StGetWindowProps();

	EXPECT_EQ(props.title, "ConfigTest");
	EXPECT_EQ(props.width, 800);
	EXPECT_EQ(props.height, 600);
	EXPECT_FALSE(props.VSync);
	EXPECT_EQ(props.refreshRate, 0);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test that a second write replaces the stored properties
 */
TEST_F(ConfigTest, StSetWindowProps_OverwritesPreviousProperties) {
	Config::StSetWindowProps(WindowProps{"Replaced", 1920, 1080, true, 144, GraphicsApi::Metal, WindowApi::Cocoa});

	const auto& props = Config::StGetWindowProps();

	EXPECT_EQ(props.title, "Replaced");
	EXPECT_EQ(props.width, 1920);
	EXPECT_EQ(props.height, 1080);
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.refreshRate, 144);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test that StGet always returns the same instance
 */
TEST_F(ConfigTest, StGet_ReturnsSameInstance) {
	const auto* first = &Config::StGet();
	const auto* second = &Config::StGet();

	EXPECT_EQ(first, second);
}

/**
 * @brief Test that StGetWindowProps reads from the singleton instance
 */
TEST_F(ConfigTest, StGetWindowProps_ReadsFromSingletonInstance) {
	Config::StSetWindowProps(WindowProps{"Shared", 640, 480, false, 60, GraphicsApi::OpenGL, WindowApi::GLFW});

	const auto* firstRead = &Config::StGetWindowProps();
	const auto* secondRead = &Config::StGetWindowProps();

	EXPECT_EQ(firstRead, secondRead);
	EXPECT_EQ(firstRead->title, "Shared");
}
