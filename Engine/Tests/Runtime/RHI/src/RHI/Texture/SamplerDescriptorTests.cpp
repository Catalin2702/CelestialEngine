//
// Module: CelestialEngine/Engine/Tests/Runtime/RHI/Texture
// File: SamplerDescriptorTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include <RHI/Texture/I_Sampler.hpp>

#include <gtest/gtest.h>

#include <format>

using CE::RHI::SamplerDescriptor;
using CE::Types::AddressMode;
using CE::Types::FilterMode;

namespace {

/**
 * @brief Test fixture for the sampler descriptor's defaults
 * @details Every field has a real value: there is no "unset" mode for a backend to interpret its own way, so a
 *			descriptor that leaves a field out samples the same on Metal and OpenGL.
 */
class SamplerDescriptorTest: public ::testing::Test {};

}

/**
 * @brief Test that an empty descriptor filters linearly
 */
TEST_F(SamplerDescriptorTest, Defaults_FilterLinearly) {
	constexpr SamplerDescriptor descriptor{};

	EXPECT_EQ(descriptor.minFilter, FilterMode::Linear);
	EXPECT_EQ(descriptor.magFilter, FilterMode::Linear);
}

/**
 * @brief Test that an empty descriptor clamps to the edge
 * @details Not repeat: with a linear filter, repeat blends the opposite edge into the border texels of a single image.
 */
TEST_F(SamplerDescriptorTest, Defaults_ClampToTheEdge) {
	constexpr SamplerDescriptor descriptor{};

	EXPECT_EQ(descriptor.addressU, AddressMode::ClampToEdge);
	EXPECT_EQ(descriptor.addressV, AddressMode::ClampToEdge);
}

/**
 * @brief Test that every filter and address mode has its own name
 */
TEST_F(SamplerDescriptorTest, Modes_FormatByName) {
	EXPECT_EQ(std::format("{}", FilterMode::Nearest), "Nearest");
	EXPECT_EQ(std::format("{}", FilterMode::Linear), "Linear");
	EXPECT_EQ(std::format("{}", AddressMode::ClampToEdge), "ClampToEdge");
	EXPECT_EQ(std::format("{}", AddressMode::Repeat), "Repeat");
	EXPECT_EQ(std::format("{}", AddressMode::MirrorRepeat), "MirrorRepeat");
}
