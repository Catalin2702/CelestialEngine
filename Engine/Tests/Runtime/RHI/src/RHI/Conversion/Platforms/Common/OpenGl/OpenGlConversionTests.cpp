//
// Module: CelestialEngine/Engine/Tests/Runtime/RHI/Conversion/Platforms/Common/OpenGl
// File: OpenGlConversionTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include <RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.hpp>
#include <RHI/Texture/I_Sampler.hpp>

#include <glad/glad.h>
#include <gtest/gtest.h>

using namespace CE::RHI;
using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the OpenGL type mappings
 */
class OpenGlTypesTest: public ::testing::Test {};

}

/**
 * @brief Test that each buffer bit matches the OpenGL constant it stands for
 * @details The enum hard-codes the GL values so the render code can build clear masks without including glad in
 *			every header: if the two ever drift apart the frame would be cleared with the wrong buffers.
 */
TEST_F(OpenGlTypesTest, BufferBit_MatchesTheOpenGlConstants) {
	EXPECT_EQ(ToInt(BufferBit::Color), GL_COLOR_BUFFER_BIT);
	EXPECT_EQ(ToInt(BufferBit::Depth), GL_DEPTH_BUFFER_BIT);
	EXPECT_EQ(ToInt(BufferBit::Stencil), GL_STENCIL_BUFFER_BIT);
}

/**
 * @brief Test that the buffer bits can be combined into a clear mask
 */
TEST_F(OpenGlTypesTest, BufferBit_CombinesIntoAClearMask) {
	const int mask = ToInt(BufferBit::Color) | ToInt(BufferBit::Depth);

	EXPECT_EQ(mask, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	EXPECT_TRUE(mask & GL_COLOR_BUFFER_BIT);
	EXPECT_FALSE(mask & GL_STENCIL_BUFFER_BIT);
}

// ============================================================================
// Shader Stage Mapping Tests
// ============================================================================

/**
 * @brief Test that the stages the OpenGL back-end compiles map onto their GL constants
 */
TEST_F(OpenGlTypesTest, ToOpenGlShaderType_MapsCompilableStages) {
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Vertex), GL_VERTEX_SHADER);
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Fragment), GL_FRAGMENT_SHADER);
}

/**
 * @brief Test that a stage without a GL mapping is reported as invalid
 */
TEST_F(OpenGlTypesTest, ToOpenGlShaderType_UnmappedStage_ReturnsInvalid) {
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Mesh), -1);
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Unknown), -1);
}

/**
 * @brief Test that the sampler filters map onto the GL filters, never onto a mipmapped one
 * @details The engine's textures have a single level: a *_MIPMAP_* minification filter on them makes the texture
 *			incomplete, and OpenGL then samples black without reporting any error.
 */
TEST_F(OpenGlTypesTest, ToOpenGl_MapsSamplerFilters) {
	EXPECT_EQ(ToOpenGl(FilterMode::Nearest), static_cast<u32>(GL_NEAREST));
	EXPECT_EQ(ToOpenGl(FilterMode::Linear), static_cast<u32>(GL_LINEAR));
}

/**
 * @brief Test that the address modes map onto the GL wrap modes
 */
TEST_F(OpenGlTypesTest, ToOpenGl_MapsSamplerAddressModes) {
	EXPECT_EQ(ToOpenGl(AddressMode::ClampToEdge), static_cast<u32>(GL_CLAMP_TO_EDGE));
	EXPECT_EQ(ToOpenGl(AddressMode::Repeat), static_cast<u32>(GL_REPEAT));
	EXPECT_EQ(ToOpenGl(AddressMode::MirrorRepeat), static_cast<u32>(GL_MIRRORED_REPEAT));
}

/**
 * @brief Test that an empty sampler descriptor converts to values glSamplerParameteri accepts
 * @details A field left out used to convert to 0: GL_INVALID_ENUM, and the sampler kept OpenGL's own mipmapped default
 *			filter, which makes a one-level texture sample black.
 */
TEST_F(OpenGlTypesTest, ToOpenGl_DefaultSamplerDescriptor_IsLinearAndClamped) {
	constexpr CE::RHI::SamplerDescriptor descriptor{};

	EXPECT_EQ(ToOpenGl(descriptor.minFilter), static_cast<u32>(GL_LINEAR));
	EXPECT_EQ(ToOpenGl(descriptor.magFilter), static_cast<u32>(GL_LINEAR));
	EXPECT_EQ(ToOpenGl(descriptor.addressU), static_cast<u32>(GL_CLAMP_TO_EDGE));
	EXPECT_EQ(ToOpenGl(descriptor.addressV), static_cast<u32>(GL_CLAMP_TO_EDGE));
}
