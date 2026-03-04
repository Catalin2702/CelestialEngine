//
// Module: CelestialEngine/Tests/Engine/Modules/Tools/ImGui
// File: ImGuiTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Tools/ImGui/ImGui.hpp>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

using namespace CE::Tools::ImGui;

/**
 * @brief Test fixture for ImGui tests
 */
class ImGuiTest: public ::testing::Test {
protected:
	void SetUp() override {
		// Setup code if needed
	}

	void TearDown() override {
		// Cleanup code if needed
	}
};

/**
 * @brief Test navigation keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_NavigationKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT), ImGuiKey_LeftArrow);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT), ImGuiKey_RightArrow);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_UP), ImGuiKey_UpArrow);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_DOWN), ImGuiKey_DownArrow);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_PAGE_UP), ImGuiKey_PageUp);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_PAGE_DOWN), ImGuiKey_PageDown);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_HOME), ImGuiKey_Home);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_END), ImGuiKey_End);
}

/**
 * @brief Test editing keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_EditingKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_INSERT), ImGuiKey_Insert);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_DELETE), ImGuiKey_Delete);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_BACKSPACE), ImGuiKey_Backspace);
}

/**
 * @brief Test special keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_SpecialKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_SPACE), ImGuiKey_Space);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_ENTER), ImGuiKey_Enter);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_ESCAPE), ImGuiKey_Escape);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_TAB), ImGuiKey_Tab);
}

/**
 * @brief Test alphanumeric keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_AlphanumericKeys_MapsCorrectly) {
	// Test numbers
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_0), ImGuiKey_0);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_1), ImGuiKey_1);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_5), ImGuiKey_5);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_9), ImGuiKey_9);

	// Test letters
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_A), ImGuiKey_A);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_Z), ImGuiKey_Z);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_M), ImGuiKey_M);
}

/**
 * @brief Test function keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_FunctionKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_F1), ImGuiKey_F1);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_F2), ImGuiKey_F2);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_F5), ImGuiKey_F5);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_F12), ImGuiKey_F12);
}

/**
 * @brief Test modifier keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_ModifierKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT_SHIFT), ImGuiKey_LeftShift);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT_SHIFT), ImGuiKey_RightShift);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT_CONTROL), ImGuiKey_LeftCtrl);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT_CONTROL), ImGuiKey_RightCtrl);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT_ALT), ImGuiKey_LeftAlt);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT_ALT), ImGuiKey_RightAlt);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT_SUPER), ImGuiKey_LeftSuper);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT_SUPER), ImGuiKey_RightSuper);
}

/**
 * @brief Test punctuation keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_PunctuationKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_APOSTROPHE), ImGuiKey_Apostrophe);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_COMMA), ImGuiKey_Comma);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_MINUS), ImGuiKey_Minus);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_PERIOD), ImGuiKey_Period);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_SLASH), ImGuiKey_Slash);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_SEMICOLON), ImGuiKey_Semicolon);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_EQUAL), ImGuiKey_Equal);
}

/**
 * @brief Test bracket keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_BracketKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_LEFT_BRACKET), ImGuiKey_LeftBracket);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_RIGHT_BRACKET), ImGuiKey_RightBracket);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_BACKSLASH), ImGuiKey_Backslash);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_GRAVE_ACCENT), ImGuiKey_GraveAccent);
}

/**
 * @brief Test lock keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_LockKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_CAPS_LOCK), ImGuiKey_CapsLock);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_SCROLL_LOCK), ImGuiKey_ScrollLock);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_NUM_LOCK), ImGuiKey_NumLock);
}

/**
 * @brief Test special function keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_SpecialFunctionKeys_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_PRINT_SCREEN), ImGuiKey_PrintScreen);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_PAUSE), ImGuiKey_Pause);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_MENU), ImGuiKey_Menu);
}

/**
 * @brief Test keypad number keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_KeypadNumbers_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_0), ImGuiKey_Keypad0);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_1), ImGuiKey_Keypad1);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_2), ImGuiKey_Keypad2);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_3), ImGuiKey_Keypad3);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_4), ImGuiKey_Keypad4);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_5), ImGuiKey_Keypad5);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_6), ImGuiKey_Keypad6);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_7), ImGuiKey_Keypad7);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_8), ImGuiKey_Keypad8);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_9), ImGuiKey_Keypad9);
}

/**
 * @brief Test keypad operation keys mapping
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_KeypadOperations_MapsCorrectly) {
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_DECIMAL), ImGuiKey_KeypadDecimal);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_DIVIDE), ImGuiKey_KeypadDivide);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_MULTIPLY), ImGuiKey_KeypadMultiply);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_SUBTRACT), ImGuiKey_KeypadSubtract);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_ADD), ImGuiKey_KeypadAdd);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_ENTER), ImGuiKey_KeypadEnter);
	EXPECT_EQ(GlfwKeyToImGuiKey(GLFW_KEY_KP_EQUAL), ImGuiKey_KeypadEqual);
}

/**
 * @brief Test unmapped key returns ImGuiKey_None
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_UnmappedKey_ReturnsNone) {
	// Test with an invalid/unmapped key code
	EXPECT_EQ(GlfwKeyToImGuiKey(-1), ImGuiKey_None);
	EXPECT_EQ(GlfwKeyToImGuiKey(9999), ImGuiKey_None);
}

/**
 * @brief Test all alphabet keys are mapped
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_AllAlphabetKeys_Mapped) {
	// Test all alphabet keys from A to Z
	for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key) {
		const ImGuiKey imguiKey = GlfwKeyToImGuiKey(key);
		EXPECT_NE(imguiKey, ImGuiKey_None) << "Key " << key << " should be mapped";
	}
}

/**
 * @brief Test all number keys are mapped
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_AllNumberKeys_Mapped) {
	// Test all number keys from 0 to 9
	for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key) {
		const ImGuiKey imguiKey = GlfwKeyToImGuiKey(key);
		EXPECT_NE(imguiKey, ImGuiKey_None) << "Number key " << key << " should be mapped";
	}
}

/**
 * @brief Test all function keys are mapped
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_AllFunctionKeys_Mapped) {
	// Test all function keys from F1 to F12
	for (int key = GLFW_KEY_F1; key <= GLFW_KEY_F12; ++key) {
		const ImGuiKey imguiKey = GlfwKeyToImGuiKey(key);
		EXPECT_NE(imguiKey, ImGuiKey_None) << "Function key " << key << " should be mapped";
	}
}

/**
 * @brief Test commonly used keys in typical applications
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_CommonKeys_AllMapped) {
	// Test a collection of commonly used keys
	const int commonKeys[] = {
		GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_TAB, GLFW_KEY_BACKSPACE,
		GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL,
		GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN
	};

	for (const int key : commonKeys) {
		const ImGuiKey imguiKey = GlfwKeyToImGuiKey(key);
		EXPECT_NE(imguiKey, ImGuiKey_None) << "Common key " << key << " should be mapped";
	}
}

/**
 * @brief Test consistency of mapping (same input produces same output)
 */
TEST_F(ImGuiTest, GlfwKeyToImGuiKey_Consistency_SameInputSameOutput) {
	constexpr int testKey = GLFW_KEY_A;
	const ImGuiKey result1 = GlfwKeyToImGuiKey(testKey);
	const ImGuiKey result2 = GlfwKeyToImGuiKey(testKey);

	EXPECT_EQ(result1, result2);
}

