//
// Module:
// File: Types.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_TYPES_VAR_VARS_HPP
#define CE_TYPES_VAR_VARS_HPP

#include <cstdint>

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

#endif //CE_TYPES_VAR_VARS_HPP
