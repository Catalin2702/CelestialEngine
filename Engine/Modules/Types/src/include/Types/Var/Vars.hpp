//
// Module: CelestialEngine/Engine/Modules/Types/Var
// File: Vars.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#pragma once

#ifndef CE_TYPES_VAR_VARS_HPP
#define CE_TYPES_VAR_VARS_HPP

#include <glm/glm.hpp>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;

using u16 = uint16_t;
using i16 = int16_t;

using u32 = uint32_t;
using i32 = int32_t;

using u64 = uint64_t;
using i64 = int64_t;

using byte = unsigned char;

using f32 = float;
using f64 = double;
using f128 = long double;

constexpr auto operator ""_u32(const u64 x) { return static_cast<u32>(x); }

constexpr auto operator ""_i32(const u64 x) { return static_cast<i32>(x); }

constexpr auto operator ""_u64(const u64 x) { return static_cast<u64>(x); }

constexpr auto operator ""_i64(const u64 x) { return static_cast<i64>(x); }

constexpr auto operator ""_b(const u64 x) { return static_cast<byte>(x); }

constexpr auto operator ""_f32(const f128 x) { return static_cast<f32>(x); }

constexpr auto operator ""_f64(const f128 x) { return static_cast<f64>(x); }

constexpr auto operator ""_f128(const f128 x) { return static_cast<f128>(x); }

constexpr glm::vec3 CE_WORLD_RIGHT{1.0_f32, 0.0_f32, 0.0_f32};
constexpr glm::vec3 CE_WORLD_UP{0.0_f32, 1.0_f32, 0.0_f32};
constexpr glm::vec3 CE_WORLD_FORWARD{0.0_f32, 0.0_f32, -1.0_f32};

/**
 * @brief Default tolerance for the float comparisons below, per floating point type
 * @details Sized for values around 1 (unit vectors, quaternions, dot products). It is deliberately not
 *			std::numeric_limits<T>::epsilon(): that is only the gap after 1.0, far smaller than the error real math
 *			accumulates, so a result that should be 0 would almost never pass. Pass a bigger tolerance where the noise
 *			is bigger (e.g. input deltas).
 */
template <std::floating_point T>
constexpr T CE_EPSILON = std::same_as<T, f32> ? static_cast<T>(1.0e-6L) : static_cast<T>(1.0e-12L);

/**
 * @brief Absolute value usable in constant expressions
 * @details libc++ does not mark std::abs constexpr yet (C++23 asks for it), so the comparisons below could not be
 *			used in a static_assert with it. The compiler still turns this ternary into a single fabs instruction.
 */
template <std::floating_point T>
[[nodiscard]] constexpr T Abs(const T value) {
	return value < static_cast<T>(0) ? -value : value;
}

/**
 * @brief Checks whether a floating point value is zero within an absolute tolerance
 * @details The tolerance is a std::type_identity_t so that it never takes part in deducing T: IsNearlyZero(x, 1.0e-3)
 *			with an f32 x stays an f32 comparison instead of failing on a double literal.
 */
template <std::floating_point T>
[[nodiscard]] constexpr bool IsNearlyZero(const T value, const std::type_identity_t<T> tolerance = CE_EPSILON<T>) {
	return Abs(value) <= tolerance;
}

/**
 * @brief Checks whether two floating point values are equal within a tolerance
 * @details The tolerance is absolute while both values are below 1 and relative to the larger one above it: a fixed
 *			absolute tolerance would be too strict for large values (the gap between two consecutive f32 near 1000 is
 *			already ~6e-5), and a purely relative one would never accept anything compared against exactly 0.
 *			Both arguments must have the same type: mixing f32 and f64 is a compile error rather than a silent widening.
 */
template <std::floating_point T>
[[nodiscard]] constexpr bool IsNearlyEqual(const T a, const T b, const std::type_identity_t<T> tolerance = CE_EPSILON<T>) {
	const T scale = std::max({static_cast<T>(1), Abs(a), Abs(b)});
	return Abs(a - b) <= tolerance * scale;
}

#endif //CE_TYPES_VAR_VARS_HPP
