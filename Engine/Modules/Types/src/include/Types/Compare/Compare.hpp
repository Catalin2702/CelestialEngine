//
// Module: CelestialEngine/Engine/Modules/Types/Compare
// File: Compare.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_COMPARE_COMPARE_HPP
#define CE_TYPES_COMPARE_COMPARE_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Type.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum CompareFunc
 * @brief Comparison function used by the depth and stencil tests
 * @details Modelled as a set of flags over the trichotomy of a comparison: a fragment either compares Less,
 *			Equal or Greater against the value already in the buffer, and a compare function is the subset of
 *			those three outcomes that passes the test. Three independent bits therefore produce exactly eight
 *			values, which is exactly the eight comparison functions every graphics API exposes - the mapping to
 *			GL_*, MTL::CompareFunction*, VK_COMPARE_OP_* and D3D12_COMPARISON_FUNC_* is a bijection, no value is
 *			missing on either side.
 *
 *			Never (no outcome passes) and Always (every outcome passes) are the two degenerate ends of that set,
 *			which is why they are 0 and 0b111 rather than extra entries bolted on.
 */
enum class CompareFunc: u8 {
	Never = 0,							///< 0b000 - the test always fails
	Less = BIT(0),						///< 0b001 - passes when the fragment is closer
	Greater = BIT(1),					///< 0b010 - passes when the fragment is farther
	Equal = BIT(2),						///< 0b100 - passes on an exact match

	NotEqual = Less | Greater,			///< 0b011 - passes on anything but an exact match
	LessEqual = Less | Equal,			///< 0b101 - the usual depth test
	GreaterEqual = Greater | Equal,		///< 0b110 - the usual depth test under a reversed-Z projection

	Always = Less | Greater | Equal,	///< 0b111 - the test always passes (equivalent to no depth test)
};

constexpr CompareFunc operator & (CompareFunc x, CompareFunc y) {
	return static_cast<CompareFunc>(static_cast<u8>(x) & static_cast<u8>(y));
}

constexpr CompareFunc operator | (CompareFunc x, CompareFunc y) {
	return static_cast<CompareFunc>(static_cast<u8>(x) | static_cast<u8>(y));
}

constexpr CompareFunc operator ^ (CompareFunc x, CompareFunc y) {
	return static_cast<CompareFunc>(static_cast<u8>(x) ^ static_cast<u8>(y));
}

/// Inverts a compare function: the outcomes that used to fail are the ones that now pass. Never becomes Always,
/// Less becomes GreaterEqual, and so on. The mask keeps the result inside the three meaningful bits.
constexpr CompareFunc operator ~ (CompareFunc x) {
	return static_cast<CompareFunc>(~static_cast<u8>(x) & static_cast<u8>(CompareFunc::Always));
}

/// Note that this is always false when y is Never: Never is the empty set, so it shares no outcome with anything.
constexpr bool HasAnyFlags(const CompareFunc x, const CompareFunc y) {
	return (x & y) != CompareFunc::Never;
}

CE_TYPES_API u32 ToOpenGl(CompareFunc compareFunc);

/**
 * @brief Names a CompareFunc, for fmt/spdlog and - through the formatter below - for std::format
 * @param compareFunc The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 * @details A value outside the eight can only come from casting an out-of-range integer: the three bits are
 *			exhaustive.
 */
constexpr std::string_view format_as(const CompareFunc compareFunc) {
	switch (compareFunc) {
		case CompareFunc::Never: return "Never";
		case CompareFunc::Less: return "Less";
		case CompareFunc::Greater: return "Greater";
		case CompareFunc::Equal: return "Equal";
		case CompareFunc::NotEqual: return "NotEqual";
		case CompareFunc::LessEqual: return "LessEqual";
		case CompareFunc::GreaterEqual: return "GreaterEqual";
		case CompareFunc::Always: return "Always";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::CompareFunc>: std::formatter<std::string_view> {
	auto format(const CE::Types::CompareFunc value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_COMPARE_COMPARE_HPP
