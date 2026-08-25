//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Window
// File: WindowPropsTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include <Types/Window/WindowProps.hpp>

#include <Types/Render/Render.hpp>

#include <gtest/gtest.h>

#include <format>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for WindowProps tests
 */
class WindowPropsTest: public ::testing::Test {};

}

// ============================================================================
// Construction Tests
// ============================================================================

/**
 * @brief Test that the default-constructed properties are empty
 */
TEST_F(WindowPropsTest, DefaultConstructed_HasEmptyDefaults) {
	const WindowProps props;

	EXPECT_TRUE(props.title.empty());
	EXPECT_EQ(props.width, 0);
	EXPECT_EQ(props.height, 0);
	EXPECT_FALSE(props.VSync);
	EXPECT_EQ(props.refreshRate, 0);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::None);
	EXPECT_EQ(props.windowApi, WindowApi::None);
}

/**
 * @brief Test that the constructor stores every field
 */
TEST_F(WindowPropsTest, Constructor_StoresEveryField) {
	const WindowProps props{"CelestialEngine", 1280, 720, true, 144, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_EQ(props.title, "CelestialEngine");
	EXPECT_EQ(props.width, 1280);
	EXPECT_EQ(props.height, 720);
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.refreshRate, 144);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

// ============================================================================
// Window API Support Tests
// ============================================================================

/**
 * @brief Test that "no API" is never reported as supported
 */
TEST_F(WindowPropsTest, IsWindowApiSupported_None_IsNotSupported) {
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::None));
}

/**
 * @brief Test that GLFW is supported on every platform
 */
TEST_F(WindowPropsTest, IsWindowApiSupported_Glfw_IsAlwaysSupported) {
	EXPECT_TRUE(IsWindowApiSupported(WindowApi::GLFW));
}

/**
 * @brief Test that the native window APIs are only supported on their own platform
 */
TEST_F(WindowPropsTest, IsWindowApiSupported_NativeApis_FollowThePlatform) {
#if CE_PLATFORM_MACOS
	EXPECT_TRUE(IsWindowApiSupported(WindowApi::Cocoa));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Win32));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::X11));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Wayland));
#elif CE_PLATFORM_WINDOWS
	EXPECT_TRUE(IsWindowApiSupported(WindowApi::Win32));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Cocoa));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::X11));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Wayland));
#else
	EXPECT_TRUE(IsWindowApiSupported(WindowApi::X11));
	EXPECT_TRUE(IsWindowApiSupported(WindowApi::Wayland));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Cocoa));
	EXPECT_FALSE(IsWindowApiSupported(WindowApi::Win32));
#endif
}

// ============================================================================
// API Compatibility Tests
// ============================================================================

/**
 * @brief Test that GLFW pairs with OpenGL
 * @details This is the cross-platform combination the engine falls back to.
 */
TEST_F(WindowPropsTest, IsGraphicsApiCompatibleWithWindowApi_GlfwAndOpenGl_AreCompatible) {
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::OpenGL, WindowApi::GLFW));
}

/**
 * @brief Test that a missing API on either side is never compatible
 */
TEST_F(WindowPropsTest, IsGraphicsApiCompatibleWithWindowApi_NoneIsNeverCompatible) {
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::None, WindowApi::GLFW));
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::OpenGL, WindowApi::None));
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::None, WindowApi::None));
}

/**
 * @brief Test that the native pairings hold on their own platform
 */
TEST_F(WindowPropsTest, IsGraphicsApiCompatibleWithWindowApi_NativePairings_FollowThePlatform) {
#if CE_PLATFORM_MACOS
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Metal, WindowApi::Cocoa));
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Vulkan, WindowApi::Cocoa));
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::OpenGL, WindowApi::Cocoa));
#elif CE_PLATFORM_WINDOWS
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::DirectX11, WindowApi::Win32));
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::DirectX12, WindowApi::Win32));
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Vulkan, WindowApi::Win32));
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Metal, WindowApi::Win32));
#else
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Vulkan, WindowApi::X11));
	EXPECT_TRUE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Vulkan, WindowApi::Wayland));
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Metal, WindowApi::X11));
#endif
}

/**
 * @brief Test that Metal is never driven through GLFW
 * @details Metal needs the Cocoa window and its MetalKit view; GLFW only carries the OpenGL context.
 */
TEST_F(WindowPropsTest, IsGraphicsApiCompatibleWithWindowApi_MetalAndGlfw_AreIncompatible) {
	EXPECT_FALSE(IsGraphicsApiCompatibleWithWindowApi(GraphicsApi::Metal, WindowApi::GLFW));
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that each window API formats as its own name
 */
TEST_F(WindowPropsTest, Formatter_FormatsWindowApiName) {
	EXPECT_EQ(std::format("{}", WindowApi::None), "None");
	EXPECT_EQ(std::format("{}", WindowApi::GLFW), "GLFW");
	EXPECT_EQ(std::format("{}", WindowApi::Cocoa), "Cocoa");
	EXPECT_EQ(std::format("{}", WindowApi::Win32), "Win32");
	EXPECT_EQ(std::format("{}", WindowApi::X11), "X11");
	EXPECT_EQ(std::format("{}", WindowApi::Wayland), "Wayland");
}
