//
// Module: CelestialEngine/Tests/Engine/Modules/Tools/Manipulation
// File: StringManipulationTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#include <Utility/Manipulation/String.hpp>

#include <gtest/gtest.h>

using namespace CE::Utility::Manipulation;

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
	const std::string input = "HELLO WORLD";
	const std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that lowercase string remains unchanged
 */
TEST_F(StringManipulationTest, ToLowerCase_LowercaseString_RemainsUnchanged) {
	const std::string input = "hello world";
	const std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that mixed case string is converted to lowercase
 */
TEST_F(StringManipulationTest, ToLowerCase_MixedCaseString_ReturnsLowercase) {
	const std::string input = "HeLLo WoRLd";
	const std::string expected = "hello world";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that empty string remains empty
 */
TEST_F(StringManipulationTest, ToLowerCase_EmptyString_ReturnsEmpty) {
	const std::string input;
	const std::string expected;
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that numbers and special characters remain unchanged
 */
TEST_F(StringManipulationTest, ToLowerCase_NumbersAndSpecialChars_RemainsUnchanged) {
	const std::string input = "ABC123!@#xyz";
	const std::string expected = "abc123!@#xyz";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test that only alphabetic characters are affected
 */
TEST_F(StringManipulationTest, ToLowerCase_AlphanumericString_OnlyLettersConverted) {
	const std::string input = "Test123String456";
	const std::string expected = "test123string456";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test with single character uppercase
 */
TEST_F(StringManipulationTest, ToLowerCase_SingleCharUppercase_ReturnsLowercase) {
	const std::string input = "A";
	const std::string expected = "a";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

/**
 * @brief Test with single character lowercase
 */
TEST_F(StringManipulationTest, ToLowerCase_SingleCharLowercase_RemainsUnchanged) {
	const std::string input = "a";
	const std::string expected = "a";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

TEST_F(StringManipulationTest, ToLowerCase_ConstCharPointer_ReturnsLowercase) {
	const char* input = "CONSTANT STRING";
	const std::string expected = "constant string";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}

TEST_F(StringManipulationTest, ToLowerCase_ConstCharPointerWithNumbers_ReturnsLowercase) {
	const char* input = "STRING123WITH456NUMBERS";
	const std::string expected = "string123with456numbers";
	const std::string result = ToLowerCase(input);

	EXPECT_EQ(result, expected);
}
