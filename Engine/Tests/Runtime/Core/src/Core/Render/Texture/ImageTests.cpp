//
// Module: CelestialEngine/Engine/Tests/Runtime/Core/Render/Texture
// File: ImageTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include <Core/Render/Texture/Image.hpp>
#include <IO/FileSystem/FileSystem.hpp>
#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <filesystem>
#include <format>
#include <span>
#include <string_view>

using namespace CE;
using IO::FileSystem;
using IO::MountAccess;

namespace fs = std::filesystem;

namespace {

/// A 2x1 PNG, RGB without alpha: a red texel then a green one. Small enough to keep inline, and three channels on
/// purpose, so the test sees the decoder expand it to four.
constexpr std::array<unsigned char, 72> TwoTexelRgbPng{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x7B, 0x40, 0xE8, 0xDD, 0x00, 0x00, 0x00,
	0x0F, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0x63, 0xF8, 0xCF, 0xC0, 0xC0, 0xF0, 0x9F, 0x01, 0x00, 0x07, 0xFF,
	0x01, 0xFF, 0x01, 0x7F, 0x89, 0xA7, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82,
};

/**
 * @brief Test fixture for Image
 * @details Every test mounts its own temporary directory as "imagetest" and writes the files it decodes through the
 *			file system, the road Image::Load reads them back by. Image::Load logs its failures, so the logger is up.
 */
class ImageTest: public ::testing::Test {
protected:
	void SetUp() override {
		Tools::Log::Init();

		const auto* const test = ::testing::UnitTest::GetInstance()->current_test_info();
		_directory = fs::temp_directory_path() / std::format("CE_ImageTests_{}", test->name());
		fs::remove_all(_directory);
		fs::create_directories(_directory);

		ASSERT_TRUE(FileSystem::Mount("imagetest", _directory, MountAccess::ReadWrite).has_value());
	}

	void TearDown() override {
		FileSystem::Unmount("imagetest");
		fs::remove_all(_directory);

		Tools::Log::Shutdown();
	}

	static void Write(const std::string_view path, const std::span<const std::byte> bytes) {
		ASSERT_TRUE(FileSystem::WriteBytes(path, bytes).has_value());
	}

protected:
	fs::path _directory;
};

}

/**
 * @brief Test that a PNG is decoded into tightly packed RGBA8 rows, top row first
 * @details The file has three channels: the decoder must still hand back four, with an opaque alpha, because RGBA8 is
 *			the one format the upload path accepts.
 */
TEST_F(ImageTest, Load_DecodesAPngIntoRgba8) {
	Write("imagetest://Two.png", std::as_bytes(std::span{TwoTexelRgbPng}));

	const auto image = Core::Image::Load("imagetest://Two.png");

	ASSERT_TRUE(image.has_value());
	EXPECT_EQ(image->width, 2u);
	EXPECT_EQ(image->height, 1u);
	EXPECT_EQ(image->format, Types::PixelFormat::RGBA8Unorm);
	ASSERT_EQ(image->pixels.size(), 2u * 1u * 4u);

	constexpr std::array<unsigned char, 8> expected{255, 0, 0, 255, 0, 255, 0, 255};
	for (size_t i = 0; i < expected.size(); ++i)
		EXPECT_EQ(std::to_integer<unsigned char>(image->pixels[i]), expected[i]) << "byte " << i;
}

/**
 * @brief Test that a file which is not an image gives nothing, not an exception
 */
TEST_F(ImageTest, Load_NotAnImage_ReturnsNothing) {
	constexpr std::string_view text = "not an image";
	Write("imagetest://Text.png", std::as_bytes(std::span{text}));

	EXPECT_FALSE(Core::Image::Load("imagetest://Text.png").has_value());
}

/**
 * @brief Test that a missing file gives nothing, not an exception
 */
TEST_F(ImageTest, Load_MissingFile_ReturnsNothing) {
	EXPECT_FALSE(Core::Image::Load("imagetest://Missing.png").has_value());
}

/**
 * @brief Test that a path through a mount that does not exist gives nothing
 */
TEST_F(ImageTest, Load_UnknownMount_ReturnsNothing) {
	EXPECT_FALSE(Core::Image::Load("nowhere://Logo.png").has_value());
}
