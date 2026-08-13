//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render
// File: RenderTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Types/Render/Render.hpp>

#include <gtest/gtest.h>

#include <format>
#include <sstream>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for GraphicsApi tests
 */
class RenderTypesTest: public ::testing::Test {};

}

// ============================================================================
// Support Tests
// ============================================================================

/**
 * @brief Test that "no API" is never reported as supported
 */
TEST_F(RenderTypesTest, IsGraphicsApiSupported_None_IsNotSupported) {
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::None));
}

/**
 * @brief Test that the cross-platform APIs are supported everywhere
 */
TEST_F(RenderTypesTest, IsGraphicsApiSupported_CrossPlatformApis_AreSupported) {
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::OpenGL));
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::Vulkan));
}

/**
 * @brief Test that the platform-exclusive APIs are only supported on their own platform
 */
TEST_F(RenderTypesTest, IsGraphicsApiSupported_PlatformExclusiveApis_FollowThePlatform) {
#ifdef CE_PLATFORM_MACOS
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::Metal));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX11));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX12));
#elifdef CE_PLATFORM_WINDOWS
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::Metal));
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::DirectX11));
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::DirectX12));
#else
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::Metal));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX11));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX12));
#endif
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that each API formats as its own name
 */
TEST_F(RenderTypesTest, Formatter_FormatsApiName) {
	EXPECT_EQ(std::format("{}", GraphicsApi::None), "None");
	EXPECT_EQ(std::format("{}", GraphicsApi::OpenGL), "OpenGL");
	EXPECT_EQ(std::format("{}", GraphicsApi::Metal), "Metal");
	EXPECT_EQ(std::format("{}", GraphicsApi::Vulkan), "Vulkan");
	EXPECT_EQ(std::format("{}", GraphicsApi::DirectX11), "DirectX11");
	EXPECT_EQ(std::format("{}", GraphicsApi::DirectX12), "DirectX12");
}

/**
 * @brief Test that streaming an API writes its name
 */
TEST_F(RenderTypesTest, StreamOperator_WritesApiName) {
	std::ostringstream stream;
	stream << GraphicsApi::Metal;

	EXPECT_EQ(stream.str(), "Metal");
}
