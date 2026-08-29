//
// Module: CelestialEngine/Engine/Modules/Types/Compare
// File: Compare.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_TYPES_COMPARE_COMPARE_HPP
#define CE_TYPES_COMPARE_COMPARE_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Type.hpp"
#include "Types/Var/Vars.hpp"


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

/**
 * @brief Converts a CompareFunc enum value to its string representation
 * @param compareFunc The CompareFunc to convert to a string
 * @return const char* A string representation of the CompareFunc, or "Unknown" if the value is not one of the eight
 * @details Useful for logging a pipeline's depth state. A value outside the eight defined ones can only be produced
 *			by casting an out-of-range integer, since the three bits are exhaustive.
 */
CE_TYPES_API const char* ToString(CompareFunc compareFunc);

}

#endif //CE_TYPES_COMPARE_COMPARE_HPP
