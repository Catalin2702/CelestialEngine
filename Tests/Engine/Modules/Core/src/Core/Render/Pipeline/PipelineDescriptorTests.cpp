//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Pipeline
// File: PipelineDescriptorTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Core/Render/Pipeline/PipelineDescriptor.hpp>

#include <gtest/gtest.h>

using CE::Core::BlendState;
using CE::Core::DepthState;
using CE::Core::PipelineDescriptor;
using CE::Core::RenderTargetFormats;
using CE::Types::BlendFactor;
using CE::Types::CompareFunc;
using CE::Types::CullMode;
using CE::Types::FrontFace;
using CE::Types::PixelFormat;
using CE::Types::PrimitiveTopology;

namespace {

/**
 * @brief Test fixture for the pipeline descriptor's defaults and validation
 * @details The descriptor is inert data: compiling it needs a device, but what it says before anyone compiles it is
 *			pure value semantics, and its defaults are what every call site inherits by not writing a field.
 */
class PipelineDescriptorTest: public ::testing::Test {};

}

// ============================================================================
// Blend State Tests
// ============================================================================

/**
 * @brief Test that the default blend state is straight alpha blending
 * @details Colour uses SrcAlpha / OneMinusSrcAlpha; alpha uses One / OneMinusSrcAlpha, and the two differ on
 *			purpose - reusing the colour factors on the alpha channel multiplies the incoming alpha by itself.
 */
TEST_F(PipelineDescriptorTest, BlendState_DefaultsToStraightAlphaBlending) {
	constexpr BlendState blend{};

	EXPECT_TRUE(blend.enabled);
	EXPECT_EQ(blend.sourceColor, BlendFactor::SrcAlpha);
	EXPECT_EQ(blend.destinationColor, BlendFactor::OneMinusSrcAlpha);
	EXPECT_EQ(blend.sourceAlpha, BlendFactor::One);
	EXPECT_EQ(blend.destinationAlpha, BlendFactor::OneMinusSrcAlpha);
}

/**
 * @brief Test that the default blend state passes its own validation
 */
TEST_F(PipelineDescriptorTest, BlendState_DefaultDestinationFactorsAreValid) {
	EXPECT_TRUE(BlendState{}.HasValidDestinationFactors());
}

/**
 * @brief Test that SrcAlphaSaturated is rejected on either destination slot
 * @details The one factor no API accepts on that side. Both slots are checked, because a state that is only wrong
 *			on the alpha channel fails just as hard and is far easier to miss.
 */
TEST_F(PipelineDescriptorTest, BlendState_RejectsSrcAlphaSaturatedOnEitherDestination) {
	BlendState colorSide{};
	colorSide.destinationColor = BlendFactor::SrcAlphaSaturated;
	EXPECT_FALSE(colorSide.HasValidDestinationFactors());

	BlendState alphaSide{};
	alphaSide.destinationAlpha = BlendFactor::SrcAlphaSaturated;
	EXPECT_FALSE(alphaSide.HasValidDestinationFactors());
}

/**
 * @brief Test that SrcAlphaSaturated is accepted on the source side
 */
TEST_F(PipelineDescriptorTest, BlendState_AllowsSrcAlphaSaturatedOnTheSourceSide) {
	BlendState blend{};
	blend.sourceColor = BlendFactor::SrcAlphaSaturated;
	blend.sourceAlpha = BlendFactor::SrcAlphaSaturated;

	EXPECT_TRUE(blend.HasValidDestinationFactors());
}

// ============================================================================
// Depth State Tests
// ============================================================================

/**
 * @brief Test that depth testing and writing are on by default, with the conventional comparison
 */
TEST_F(PipelineDescriptorTest, DepthState_DefaultsToTestingAndWritingWithLess) {
	constexpr DepthState depth{};

	EXPECT_TRUE(depth.testEnabled);
	EXPECT_TRUE(depth.writeEnabled);
	EXPECT_EQ(depth.compareFunc, CompareFunc::Less);
}

// ============================================================================
// Render Target Format Tests
// ============================================================================

/**
 * @brief Test that a descriptor declares one colour attachment by default
 * @details Every backend but OpenGL rejects a pipeline whose declared formats disagree with the pass it is used
 *			against, so this default is a claim about the swapchain: BGRA8 is what CAMetalLayer and DXGI prefer, and
 *			a GLFW window hands back RGBA8 instead - which is why the call sites ask rather than assume.
 */
TEST_F(PipelineDescriptorTest, RenderTargetFormats_DefaultToOneBgraColourAndFloatDepth) {
	constexpr RenderTargetFormats formats{};

	EXPECT_EQ(formats.colorCount, 1u);
	EXPECT_EQ(formats.colors[0], PixelFormat::BGRA8Unorm);
	EXPECT_EQ(formats.depth, PixelFormat::Depth32Float);
}

// ============================================================================
// Descriptor Tests
// ============================================================================

/**
 * @brief Test that a fresh descriptor has no shaders and no vertex layout
 * @details Both are required to compile, and neither has a sensible default - the empty layout in particular is the
 *			one that draws nothing on Metal while looking fine on OpenGL, because GL reads the format off the vertex
 *			array instead of the pipeline.
 */
TEST_F(PipelineDescriptorTest, DefaultConstructed_CarriesNoShadersAndAnEmptyLayout) {
	const PipelineDescriptor descriptor{};

	EXPECT_EQ(descriptor.vertexShader, nullptr);
	EXPECT_EQ(descriptor.fragmentShader, nullptr);
	EXPECT_TRUE(descriptor.vertexLayout.GetElements().empty());
	EXPECT_EQ(descriptor.vertexLayout.GetStride(), 0u);
}

/**
 * @brief Test the rasterizer defaults: triangles, back faces culled, counter-clockwise front
 */
TEST_F(PipelineDescriptorTest, DefaultConstructed_RasterizesTrianglesWithBackFaceCulling) {
	const PipelineDescriptor descriptor{};

	EXPECT_EQ(descriptor.topology, PrimitiveTopology::TriangleList);
	EXPECT_EQ(descriptor.cullMode, CullMode::Back);
	EXPECT_EQ(descriptor.frontFace, FrontFace::CounterClockwise);
}
