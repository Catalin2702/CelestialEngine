//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Input
// File: KeyShortcutTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Core/Input/KeyShortcut.hpp>

#include <Types/Types.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>

using CE::Core::KeyShortcut;
using CE::Types::KeyboardKeyCode;

namespace {

// A shortcut has to stay usable in a constexpr descriptor: checked at compile time, where the guarantee lives.
constexpr KeyShortcut stShiftF1{KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1};
static_assert(stShiftF1.GetKeys().size() == 2);
static_assert(not stShiftF1.IsEmpty());
static_assert(KeyShortcut{}.IsEmpty());

}

// ============================================================================
// Construction Tests
// ============================================================================

/**
 * @brief Test that a default shortcut holds no key
 */
TEST(KeyShortcutTest, DefaultConstructed_IsEmpty) {
	constexpr KeyShortcut shortcut{};

	EXPECT_TRUE(shortcut.IsEmpty());
	EXPECT_TRUE(shortcut.GetKeys().empty());
}

/**
 * @brief Test that the keys come back in the order they were given, without the unused slots
 */
TEST(KeyShortcutTest, GetKeys_ReturnsOnlyTheGivenKeysInOrder) {
	constexpr KeyShortcut shortcut{KeyboardKeyCode::LeftControl, KeyboardKeyCode::LeftShift, KeyboardKeyCode::S};
	const auto keys = shortcut.GetKeys();
	constexpr std::array expected{KeyboardKeyCode::LeftControl, KeyboardKeyCode::LeftShift, KeyboardKeyCode::S};

	EXPECT_TRUE(std::ranges::equal(keys, expected));
	EXPECT_TRUE(std::ranges::none_of(keys, [](const KeyboardKeyCode key) { return key == KeyboardKeyCode::Unknown; }));
}

/**
 * @brief Test that the four slots can all be used
 */
TEST(KeyShortcutTest, Constructor_AcceptsTheMaximumNumberOfKeys) {
	constexpr KeyShortcut shortcut{
		KeyboardKeyCode::LeftControl, KeyboardKeyCode::LeftAlt, KeyboardKeyCode::LeftShift, KeyboardKeyCode::Escape
	};

	EXPECT_EQ(shortcut.GetKeys().size(), KeyShortcut::stMaxKeys);
}

/**
 * @brief Test that a shortcut can be written as a braced list, the way a descriptor field initialises it
 */
TEST(KeyShortcutTest, BracedList_ConvertsImplicitly) {
	const KeyShortcut shortcut = {KeyboardKeyCode::LeftShift, KeyboardKeyCode::F1};

	EXPECT_EQ(shortcut.GetKeys().size(), 2u);
}
