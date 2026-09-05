//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render
// File: PixelFormatTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Types/Render/PixelFormat.hpp>

#include <gtest/gtest.h>

#include <format>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the pixel format queries
 */
class PixelFormatTest: public ::testing::Test {};

}

// ============================================================================
// Depth Tests
// ============================================================================

/**
 * @brief Test that every Depth* format is reported as a depth format
 */
TEST_F(PixelFormatTest, IsDepthFormat_TrueForEveryDepthFormat) {
	EXPECT_TRUE(IsDepthFormat(PixelFormat::Depth16Unorm));
	EXPECT_TRUE(IsDepthFormat(PixelFormat::Depth32Float));
	EXPECT_TRUE(IsDepthFormat(PixelFormat::Depth24UnormStencil8));
	EXPECT_TRUE(IsDepthFormat(PixelFormat::Depth32FloatStencil8));
}

/**
 * @brief Test that colour formats and None are not depth formats
 */
TEST_F(PixelFormatTest, IsDepthFormat_FalseForColourFormatsAndNone) {
	EXPECT_FALSE(IsDepthFormat(PixelFormat::None));
	EXPECT_FALSE(IsDepthFormat(PixelFormat::RGBA8Unorm));
	EXPECT_FALSE(IsDepthFormat(PixelFormat::BGRA8UnormSrgb));
	EXPECT_FALSE(IsDepthFormat(PixelFormat::RGBA16Float));
	EXPECT_FALSE(IsDepthFormat(PixelFormat::R32Float));
}

/**
 * @brief Test that only the two combined formats report a stencil
 * @details A depth format is not a stencil format: Depth32Float is the default the swapchain allocates and has no
 *			stencil plane, so a pass that clears one against it is asking for something that is not there.
 */
TEST_F(PixelFormatTest, HasStencil_OnlyForTheCombinedFormats) {
	EXPECT_TRUE(HasStencil(PixelFormat::Depth24UnormStencil8));
	EXPECT_TRUE(HasStencil(PixelFormat::Depth32FloatStencil8));

	EXPECT_FALSE(HasStencil(PixelFormat::Depth16Unorm));
	EXPECT_FALSE(HasStencil(PixelFormat::Depth32Float));
	EXPECT_FALSE(HasStencil(PixelFormat::RGBA8Unorm));
	EXPECT_FALSE(HasStencil(PixelFormat::None));
}

/**
 * @brief Test that every format carrying a stencil is also a depth format
 */
TEST_F(PixelFormatTest, HasStencil_ImpliesIsDepthFormat) {
	for (const auto format: {PixelFormat::Depth24UnormStencil8, PixelFormat::Depth32FloatStencil8})
		EXPECT_TRUE(IsDepthFormat(format)) << "stencil format " << format_as(format) << " is not a depth format";
}

// ============================================================================
// Footprint Tests
// ============================================================================

/**
 * @brief Test the byte footprint of the formats the engine actually allocates
 */
TEST_F(PixelFormatTest, GetBytesPerPixel_MatchesTheFormatWidth) {
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::R8Unorm), 1u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RG8Unorm), 2u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::Depth16Unorm), 2u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RGBA8Unorm), 4u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::BGRA8Unorm), 4u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::Depth32Float), 4u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RGBA16Float), 8u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RGBA32Float), 16u);
}

/**
 * @brief Test that the sRGB variants cost the same as their linear counterparts
 * @details The transfer function is applied by the sampling hardware, not stored: an sRGB format that measured
 *			differently would mean the conversion had been baked into the layout.
 */
TEST_F(PixelFormatTest, GetBytesPerPixel_SrgbCostsTheSameAsLinear) {
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RGBA8UnormSrgb), GetBytesPerPixel(PixelFormat::RGBA8Unorm));
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::BGRA8UnormSrgb), GetBytesPerPixel(PixelFormat::BGRA8Unorm));
}

/**
 * @brief Test that the packed formats are the cheap alternatives they exist to be
 * @details RGB10A2 and RG11B10 both fit in a 32-bit word - that is their whole reason for existing next to
 *			RGBA16Float, which costs twice as much for the same dynamic range.
 */
TEST_F(PixelFormatTest, GetBytesPerPixel_PackedFormatsFitInOneWord) {
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RGB10A2Unorm), 4u);
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::RG11B10Float), 4u);
	EXPECT_LT(GetBytesPerPixel(PixelFormat::RG11B10Float), GetBytesPerPixel(PixelFormat::RGBA16Float));
}

/**
 * @brief Test that a format of None measures nothing
 */
TEST_F(PixelFormatTest, GetBytesPerPixel_NoneIsZero) {
	EXPECT_EQ(GetBytesPerPixel(PixelFormat::None), 0u);
}

// ============================================================================
// Naming Tests
// ============================================================================

/**
 * @brief Test that a format names itself, both through format_as and through std::format
 */
TEST_F(PixelFormatTest, Formatter_NamesTheFormat) {
	EXPECT_EQ(format_as(PixelFormat::BGRA8Unorm), "BGRA8Unorm");
	EXPECT_EQ(format_as(PixelFormat::None), "None");
	EXPECT_EQ(std::format("{}", PixelFormat::Depth32Float), "Depth32Float");
}

/**
 * @brief Test that a value outside the enum names itself rather than reading past the table
 */
TEST_F(PixelFormatTest, Formatter_UnknownValueIsNamedUnknown) {
	EXPECT_EQ(format_as(static_cast<PixelFormat>(200)), "Unknown");
}
