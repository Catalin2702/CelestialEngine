//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: KeyShortcut.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_CORE_INPUT_KEYSHORTCUT_HPP
#define CE_CORE_INPUT_KEYSHORTCUT_HPP

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/Var/Vars.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <span>


namespace CE::Core {

/**
 * @class KeyShortcut
 * @brief A chord of up to four keys that have to be held together, e.g. Shift+F1
 * @details A fixed array rather than a vector, so a shortcut stays a literal type: it can sit in a constexpr descriptor
 *			and be deserialised by Config like any other field. Four keys cover every real chord - three modifiers and
 *			the key they modify.
 *
 *			The order of the keys does not matter to Input: a chord is a set, and Input::IsShortcutJustPressed fires
 *			whichever of its keys goes down last.
 */
class KeyShortcut {
public:
	static constexpr std::size_t stMaxKeys = 4;

public:
	/**
	 * @brief An empty shortcut, which never fires
	 */
	constexpr KeyShortcut() = default;

	/**
	 * @brief A shortcut made of the given keys
	 * @details Implicit on purpose, so a descriptor can write `.captureShortcut = {LeftShift, F1}`.
	 */
	template<std::same_as<Types::KeyboardKeyCode>... Keys> requires (sizeof...(Keys) >= 1 and sizeof...(Keys) <= stMaxKeys)
	constexpr KeyShortcut(const Keys... keys): // NOLINT(cppcoreguidelines-explicit-constructor, misc-explicit-constructor)
		_keys{keys...},
		_count{static_cast<u8>(sizeof...(Keys))} {}

public:
	/**
	 * @brief The keys of the chord, without the unused slots
	 */
	[[nodiscard]] constexpr std::span<const Types::KeyboardKeyCode> GetKeys() const { return {_keys.data(), _count}; }

	/**
	 * @brief Whether the shortcut holds no key at all, the one that never fires
	 */
	[[nodiscard]] constexpr bool IsEmpty() const { return _count == 0; }

private:
	std::array<Types::KeyboardKeyCode, stMaxKeys> _keys{}; ///< Unused slots stay Unknown
	u8 _count = 0; ///< How many leading slots of _keys are in use
};

}

#endif //CE_CORE_INPUT_KEYSHORTCUT_HPP
