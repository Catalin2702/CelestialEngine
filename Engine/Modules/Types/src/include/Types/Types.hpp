//
// Module: CelestialEngine/Engine/Modules/Types
// File: Types.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_TYPES_TYPES_HPP
#define CE_TYPES_TYPES_HPP

#include "Types/Build/Build.hpp"

#include "FileSystem/File.hpp"

#include "KeyCode/KeyboardKeyCode.hpp"
#include "KeyCode/MouseButtonCode.hpp"

#include "Render/Render.hpp"
#include "Render/Shader.hpp"
#include "Render/Platforms/Common/OpenGl/OpenGl.hpp"

#include "Window/WindowProps.hpp"
#include "Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"

#include <cstdint>


constexpr auto operator""_u32(const unsigned long long x) { return static_cast<uint32_t>(x); }
constexpr auto operator""_i32(const unsigned long long x) { return static_cast<int32_t>(x); }
constexpr auto operator""_u64(const unsigned long long x) { return static_cast<uint64_t>(x); }
constexpr auto operator""_i64(const unsigned long long x) { return static_cast<int64_t>(x); }

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint64_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

#endif //CE_TYPES_TYPES_HPP
