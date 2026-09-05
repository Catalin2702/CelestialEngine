//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Buffer
// File: BufferLayoutTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Core/Render/Buffer/I_Buffer.hpp>

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

using CE::Core::BufferElement;
using CE::Core::BufferLayout;
using CE::Types::ShaderDataType;

namespace {

/**
 * @brief Test fixture for the vertex buffer layout
 * @details The layout is the one place the CPU's idea of a vertex and the GPU's have to agree, and it is plain
 *			arithmetic: no device is involved in computing an offset. When it is wrong nothing errors - OpenGL reads
 *			the wrong bytes and Metal draws nothing at all - which is exactly why it is worth pinning down here.
 */
class BufferLayoutTest: public ::testing::Test {};

}

// ============================================================================
// Element Tests
// ============================================================================

/**
 * @brief Test that an element derives its size and component count from its type
 */
TEST_F(BufferLayoutTest, Element_DerivesSizeAndComponentCountFromItsType) {
	const BufferElement position{ShaderDataType::Float3, "inputPosition"};

	EXPECT_EQ(position.type, ShaderDataType::Float3);
	EXPECT_EQ(position.size, 3 * sizeof(f32));
	EXPECT_EQ(position.componentCount, 3u);
	EXPECT_EQ(position.name, "inputPosition");
	EXPECT_FALSE(position.normalized);
}

/**
 * @brief Test that a matrix element is measured as its whole footprint
 * @details A mat4 is four attribute slots on every backend, but one element here: the layout describes bytes, and
 *			the splitting into slots is the pipeline's job.
 */
TEST_F(BufferLayoutTest, Element_MeasuresAMatrixAsItsWholeFootprint) {
	const BufferElement transform{ShaderDataType::Mat4, "inputTransform"};

	EXPECT_EQ(transform.size, 16 * sizeof(f32));
	EXPECT_EQ(transform.componentCount, 16u);
}

// ============================================================================
// Layout Tests
// ============================================================================

/**
 * @brief Test that an empty layout has a zero stride and no elements
 */
TEST_F(BufferLayoutTest, Empty_HasNoElementsAndAZeroStride) {
	const BufferLayout layout{};

	EXPECT_TRUE(layout.GetElements().empty());
	EXPECT_EQ(layout.GetStride(), 0u);
	EXPECT_EQ(layout.begin(), layout.end());
}

/**
 * @brief Test that offsets run in declaration order and the stride is their total
 * @details This is the engine's real vertex format - position then colour, 28 bytes - and the declaration order is
 *			load-bearing twice over: element i becomes attribute i, so a reorder here silently rebinds every shader
 *			input.
 */
TEST_F(BufferLayoutTest, Offsets_FollowDeclarationOrderAndSumToTheStride) {
	const BufferLayout layout{
		{ShaderDataType::Float3, "inputPosition"},
		{ShaderDataType::Float4, "inputColor"}
	};

	const auto& elements = layout.GetElements();
	ASSERT_EQ(elements.size(), 2u);

	EXPECT_EQ(elements[0].offset, 0u);
	EXPECT_EQ(elements[1].offset, 3 * sizeof(f32));
	EXPECT_EQ(layout.GetStride(), 7 * sizeof(f32));
}

/**
 * @brief Test that a single-element layout has its stride equal to that element
 */
TEST_F(BufferLayoutTest, SingleElement_StrideEqualsTheElementSize) {
	const BufferLayout layout{{ShaderDataType::Float2, "inputTexCoord"}};

	EXPECT_EQ(layout.GetElements().front().offset, 0u);
	EXPECT_EQ(layout.GetStride(), 2 * sizeof(f32));
}

/**
 * @brief Test that the layout packs tightly, with no padding inserted between elements
 * @details Deliberate, and it has to stay deliberate: the buffer is uploaded as a flat array of floats, so the day
 *			an element wants alignment the upload has to learn about it at the same time.
 */
TEST_F(BufferLayoutTest, ElementsArePackedTightly) {
	const BufferLayout layout{
		{ShaderDataType::Float, "a"},
		{ShaderDataType::Float3, "b"},
		{ShaderDataType::Int2, "c"}
	};

	const auto& elements = layout.GetElements();
	ASSERT_EQ(elements.size(), 3u);

	u32 running = 0;
	for (const auto& element: elements) {
		EXPECT_EQ(element.offset, running);
		running += element.size;
	}

	EXPECT_EQ(layout.GetStride(), running);
}

/**
 * @brief Test that iterating the layout visits every element in order
 */
TEST_F(BufferLayoutTest, IteratesInDeclarationOrder) {
	const BufferLayout layout{
		{ShaderDataType::Float3, "inputPosition"},
		{ShaderDataType::Float4, "inputColor"}
	};

	std::vector<std::string_view> names;
	for (const auto& element: layout)
		names.push_back(element.name);

	ASSERT_EQ(names.size(), 2u);
	EXPECT_EQ(names[0], "inputPosition");
	EXPECT_EQ(names[1], "inputColor");
}

/**
 * @brief Test that an element declared normalized keeps that flag
 */
TEST_F(BufferLayoutTest, NormalizedFlagSurvivesTheLayout) {
	const BufferLayout layout{{ShaderDataType::Float4, "inputColor", true}};

	EXPECT_TRUE(layout.GetElements().front().normalized);
}
