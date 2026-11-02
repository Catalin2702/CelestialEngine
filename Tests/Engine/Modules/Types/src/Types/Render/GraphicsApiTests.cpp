//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render
// File: GraphicsApiTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Types/Render/GraphicsApi.hpp>

#include <gtest/gtest.h>

#include <format>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for GraphicsApi tests
 */
class GraphicsApiTest: public ::testing::Test {};

}

// ============================================================================
// Support Tests
// ============================================================================

/**
 * @brief Test that "no API" is never reported as supported
 */
TEST_F(GraphicsApiTest, IsGraphicsApiSupported_None_IsNotSupported) {
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::None));
}

/**
 * @brief Test that the cross-platform APIs are supported everywhere
 */
TEST_F(GraphicsApiTest, IsGraphicsApiSupported_CrossPlatformApis_AreSupported) {
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::OpenGL));
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::Vulkan));
}

/**
 * @brief Test that the platform-exclusive APIs are only supported on their own platform
 */
TEST_F(GraphicsApiTest, IsGraphicsApiSupported_PlatformExclusiveApis_FollowThePlatform) {
#if CE_PLATFORM_MACOS
	EXPECT_TRUE(IsGraphicsApiSupported(GraphicsApi::Metal));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX11));
	EXPECT_FALSE(IsGraphicsApiSupported(GraphicsApi::DirectX12));
#elif CE_PLATFORM_WINDOWS
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
// Build Selection Tests
// ============================================================================

/**
 * @brief Test that the engine reports exactly the backends CE_RENDER_BACKENDS compiled in
 * @details Checked against the same CE_RENDER_* macros the test is compiled with: the answer comes from the engine
 *			library, so this also catches a library and a client that disagree on the selection.
 */
TEST_F(GraphicsApiTest, IsGraphicsApiCompiled_MatchesTheBuildSelection) {
	EXPECT_EQ(IsGraphicsApiCompiled(GraphicsApi::OpenGL), static_cast<bool>(CE_RENDER_OPENGL));
	EXPECT_EQ(IsGraphicsApiCompiled(GraphicsApi::Metal), static_cast<bool>(CE_RENDER_METAL));
	EXPECT_EQ(IsGraphicsApiCompiled(GraphicsApi::Vulkan), static_cast<bool>(CE_RENDER_VULKAN));
	EXPECT_EQ(IsGraphicsApiCompiled(GraphicsApi::DirectX11), static_cast<bool>(CE_RENDER_DIRECTX11));
	EXPECT_EQ(IsGraphicsApiCompiled(GraphicsApi::DirectX12), static_cast<bool>(CE_RENDER_DIRECTX12));
}

/**
 * @brief Test that neither "no API" nor a combination of APIs counts as a compiled backend
 */
TEST_F(GraphicsApiTest, IsGraphicsApiCompiled_NoneAndCombinations_AreNotBackends) {
	EXPECT_FALSE(IsGraphicsApiCompiled(GraphicsApi::None));
	EXPECT_FALSE(IsGraphicsApiCompiled(GraphicsApi::OpenGL | GraphicsApi::Metal));
}

/**
 * @brief Test that the default API is the first entry of CE_RENDER_BACKENDS, and so always compiled in and supported
 */
TEST_F(GraphicsApiTest, GetDefaultGraphicsApi_IsCompiledAndSupported) {
	const auto api = GetDefaultGraphicsApi();

	EXPECT_EQ(api, GraphicsApi::CE_RENDER_DEFAULT_API);
	EXPECT_TRUE(IsGraphicsApiCompiled(api));
	EXPECT_TRUE(IsGraphicsApiSupported(api));
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that each API formats as its own name
 */
TEST_F(GraphicsApiTest, Formatter_FormatsApiName) {
	EXPECT_EQ(std::format("{}", GraphicsApi::None), "None");
	EXPECT_EQ(std::format("{}", GraphicsApi::OpenGL), "OpenGL");
	EXPECT_EQ(std::format("{}", GraphicsApi::Metal), "Metal");
	EXPECT_EQ(std::format("{}", GraphicsApi::Vulkan), "Vulkan");
	EXPECT_EQ(std::format("{}", GraphicsApi::DirectX11), "DirectX11");
	EXPECT_EQ(std::format("{}", GraphicsApi::DirectX12), "DirectX12");
}
