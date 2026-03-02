//
// Module: Tests/Engine/Modules/Tools/Manipulation
// File: StringManipulationTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Tools/Manipulation/String.hpp>

#include <gtest/gtest.h>

using namespace CE::Tools::Manipulation;

/**
 * @brief Test fixture for String Manipulation tests
 */
class StringManipulationTest: public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {}
};

/**
 * @brief Test that uppercase letters are converted to lowercase
 */
TEST_F(StringManipulationTest, ToLowerCase_UppercaseString_ReturnsLowercase) {
	constexpr std::string input = "HELLO WORLD";
	constexpr std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that lowercase string remains unchanged
 */
TEST_F(StringManipulationTest, ToLowerCase_LowercaseString_RemainsUnchanged) {
	constexpr std::string input = "hello world";
	constexpr std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that mixed case string is converted to lowercase
 */
TEST_F(StringManipulationTest, ToLowerCase_MixedCaseString_ReturnsLowercase) {
	constexpr std::string input = "HeLLo WoRLd";
	constexpr std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that empty string remains empty
 */
TEST_F(StringManipulationTest, ToLowerCase_EmptyString_ReturnsEmpty) {
	constexpr std::string input;
	constexpr std::string expected;
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that numbers and special characters remain unchanged
 */
TEST_F(StringManipulationTest, ToLowerCase_NumbersAndSpecialChars_RemainsUnchanged) {
	constexpr std::string input = "ABC123!@#xyz";
	constexpr std::string expected = "abc123!@#xyz";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that only alphabetic characters are affected
 */
TEST_F(StringManipulationTest, ToLowerCase_AlphanumericString_OnlyLettersConverted) {
	constexpr std::string input = "Test123String456";
	constexpr std::string expected = "test123string456";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test with single character uppercase
 */
TEST_F(StringManipulationTest, ToLowerCase_SingleCharUppercase_ReturnsLowercase) {
	constexpr std::string input = "A";
	constexpr std::string expected = "a";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test with single character lowercase
 */
TEST_F(StringManipulationTest, ToLowerCase_SingleCharLowercase_RemainsUnchanged) {
	constexpr std::string input = "a";
	constexpr std::string expected = "a";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}
