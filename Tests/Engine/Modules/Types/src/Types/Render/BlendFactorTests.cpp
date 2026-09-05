//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render
// File: BlendFactorTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include <Types/Render/BlendFactor.hpp>

#include <gtest/gtest.h>

#include <format>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the blend factor rules
 */
class BlendFactorTest: public ::testing::Test {};

}

// ============================================================================
// Destination Validity Tests
// ============================================================================

/**
 * @brief Test that SrcAlphaSaturated is the one factor no destination slot accepts
 * @details Not an engine restriction: every API rejects it there. Checking it up front is what turns a driver
 *			complaint at pipeline creation - on the backends that bother to validate - into a failure at the call
 *			site that wrote it.
 */
TEST_F(BlendFactorTest, IsValidDestinationFactor_RejectsOnlySrcAlphaSaturated) {
	EXPECT_FALSE(IsValidDestinationFactor(BlendFactor::SrcAlphaSaturated));
}

/**
 * @brief Test that every other factor is legal on the destination side
 * @details Enumerated one by one rather than looped over a range: the enum has no count sentinel, and adding a
 *			factor should make someone decide which side it belongs on rather than inherit an answer.
 */
TEST_F(BlendFactorTest, IsValidDestinationFactor_AcceptsEveryOtherFactor) {
	constexpr BlendFactor valid[]{
		BlendFactor::Zero, BlendFactor::One,
		BlendFactor::SrcColor, BlendFactor::OneMinusSrcColor,
		BlendFactor::DstColor, BlendFactor::OneMinusDstColor,
		BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha,
		BlendFactor::DstAlpha, BlendFactor::OneMinusDstAlpha,
		BlendFactor::ConstantColor, BlendFactor::OneMinusConstantColor,
		BlendFactor::ConstantAlpha, BlendFactor::OneMinusConstantAlpha,
	};

	for (const auto factor: valid)
		EXPECT_TRUE(IsValidDestinationFactor(factor)) << format_as(factor) << " should be legal on the destination side";
}

/**
 * @brief Test that the factors of straight alpha blending are all destination-legal
 * @details The default a PipelineDescriptor is built with, so if this ever failed every pipeline in the engine
 *			would be invalid.
 */
TEST_F(BlendFactorTest, IsValidDestinationFactor_AcceptsTheAlphaBlendingDefaults) {
	EXPECT_TRUE(IsValidDestinationFactor(BlendFactor::OneMinusSrcAlpha));
	EXPECT_TRUE(IsValidDestinationFactor(BlendFactor::One));
}

// ============================================================================
// Naming Tests
// ============================================================================

/**
 * @brief Test that a factor names itself, both through format_as and through std::format
 */
TEST_F(BlendFactorTest, Formatter_NamesTheFactor) {
	EXPECT_EQ(format_as(BlendFactor::OneMinusSrcAlpha), "OneMinusSrcAlpha");
	EXPECT_EQ(format_as(BlendFactor::SrcAlphaSaturated), "SrcAlphaSaturated");
	EXPECT_EQ(std::format("{}", BlendFactor::Zero), "Zero");
}

/**
 * @brief Test that a value outside the enum names itself rather than reading past the table
 */
TEST_F(BlendFactorTest, Formatter_UnknownValueIsNamedUnknown) {
	EXPECT_EQ(format_as(static_cast<BlendFactor>(200)), "Unknown");
}
