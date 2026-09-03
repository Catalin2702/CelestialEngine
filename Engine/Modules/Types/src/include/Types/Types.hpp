//
// Module: CelestialEngine/Engine/Modules/Types
// File: Types.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

/**
 * @file Types.hpp
 * @brief Umbrella header for the Types module
 * @details **Naming an enum value.** Every enum in the engine is printed through `std::format("{}", value)` or a
 *			logging macro, never through a hand-rolled ToString: each one declares a `format_as` overload holding the
 *			single switch that names it, plus a `std::formatter` specialisation that delegates to it.
 *
 *			Two hooks are needed because the engine formats through two libraries. `std::format` and `std::print` look
 *			for `std::formatter`; spdlog is built on fmt, which ignores `std::formatter` entirely and looks for a
 *			`format_as` free function by ADL instead. A `std::formatter` alone compiles fine until the first
 *			`CE_CORE_INFO("{}", value)`, which then fails with a wall of fmt template errors - hence both, delegating
 *			to one switch so they can never disagree.
 *
 *			Flag enums (BufferBit, EventCategory) return an owning std::string naming the set, e.g. "Color|Depth".
 */

#pragma once

#ifndef CE_TYPES_TYPES_HPP
#define CE_TYPES_TYPES_HPP

#include "Types/Build/Build.hpp"

#include "Compare/Compare.hpp"

#include "FileSystem/File.hpp"

#include "KeyCode/KeyboardKeyCode.hpp"
#include "KeyCode/MouseButtonCode.hpp"

#include "Render/AttachmentAction.hpp"
#include "Render/BlendFactor.hpp"
#include "Render/CullMode.hpp"
#include "Render/PixelFormat.hpp"
#include "Render/PrimitiveTopology.hpp"
#include "Render/Render.hpp"
#include "Render/Shader.hpp"
#include "Render/Platforms/Common/OpenGl/OpenGl.hpp"
#if CE_PLATFORM_MACOS
#include "Render/Platforms/Mac/Metal/Metal.hpp"
#endif

#include "Var/Vars.hpp"

#include "Window/WindowProps.hpp"
#include "Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"

#endif //CE_TYPES_TYPES_HPP
