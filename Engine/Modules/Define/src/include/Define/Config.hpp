//
// Module: CelestialEngine/Engine/Modules/Define
// File: Config.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-25
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_DEFINE_CONFIG_HPP
#define CE_DEFINE_CONFIG_HPP

// CMake (CE_Config, see config/cmake/CelestialEngineMacros.cmake) defines every macro below to 0 or 1 in every build, so they
// can be used both by the preprocessor and by `if constexpr`. The fallbacks exist because those two uses fail differently: an
// undefined macro is silently 0 inside #if, but a hard "use of undeclared identifier" inside `if constexpr` - which is exactly
// what a translation unit that forgot to link CE_Config would hit. Defaulting them here keeps that failure mode uniform, and
// the checks below turn a missing or contradictory definition into one readable error instead of a wrong build.

#ifndef CE_DEBUG
	#define CE_DEBUG 0
#endif

#ifndef CE_RELEASE
	#define CE_RELEASE 0
#endif

#ifndef CE_DIST
	#define CE_DIST 0
#endif

#ifndef CE_PLATFORM_WINDOWS
	#define CE_PLATFORM_WINDOWS 0
#endif

#ifndef CE_PLATFORM_MACOS
	#define CE_PLATFORM_MACOS 0
#endif

#ifndef CE_PLATFORM_LINUX
	#define CE_PLATFORM_LINUX 0
#endif

#if (CE_DEBUG + CE_RELEASE + CE_DIST) != 1
	#error "Exactly one of CE_DEBUG, CE_RELEASE, CE_DIST must be 1 - link CE_Config (or CE_Define) into this target."
#endif

#if (CE_PLATFORM_WINDOWS + CE_PLATFORM_MACOS + CE_PLATFORM_LINUX) != 1
	#error "Exactly one of CE_PLATFORM_WINDOWS, CE_PLATFORM_MACOS, CE_PLATFORM_LINUX must be 1 - CelestialEngine supports only Windows, macOS and Linux."
#endif

// Graphics backends compiled in, chosen with CE_RENDER_BACKENDS (config/cmake/CERenderBackends.cmake). These say what was
// BUILT; CE_PLATFORM_* says what the OS is. Code that only exists for a backend - a Metal swapchain, a GL loader call -
// is guarded by these, never by the platform macro it happens to coincide with today.
#ifndef CE_RENDER_OPENGL
	#define CE_RENDER_OPENGL 0
#endif

#ifndef CE_RENDER_METAL
	#define CE_RENDER_METAL 0
#endif

#ifndef CE_RENDER_VULKAN
	#define CE_RENDER_VULKAN 0
#endif

#ifndef CE_RENDER_DIRECTX11
	#define CE_RENDER_DIRECTX11 0
#endif

#ifndef CE_RENDER_DIRECTX12
	#define CE_RENDER_DIRECTX12 0
#endif

// Window backends, derived from the graphics ones by CMake rather than chosen: GLFW for OpenGL/Vulkan, Cocoa for Metal.
#ifndef CE_WINDOW_GLFW
	#define CE_WINDOW_GLFW 0
#endif

#ifndef CE_WINDOW_COCOA
	#define CE_WINDOW_COCOA 0
#endif

#if (CE_RENDER_OPENGL + CE_RENDER_METAL + CE_RENDER_VULKAN + CE_RENDER_DIRECTX11 + CE_RENDER_DIRECTX12) == 0
	#error "No graphics backend is compiled in - link CE_Config, which carries the CE_RENDER_* selection made by CE_RENDER_BACKENDS."
#endif

#if (CE_WINDOW_GLFW + CE_WINDOW_COCOA) == 0
	#error "No window backend is compiled in - CE_WINDOW_* is derived from CE_RENDER_BACKENDS; link CE_Config."
#endif

#if CE_RENDER_METAL and not CE_PLATFORM_MACOS
	#error "CE_RENDER_METAL is set on a platform other than macOS."
#endif

#if (CE_RENDER_DIRECTX11 or CE_RENDER_DIRECTX12) and not CE_PLATFORM_WINDOWS
	#error "CE_RENDER_DIRECTX11/12 is set on a platform other than Windows."
#endif

#if CE_WINDOW_COCOA and not CE_PLATFORM_MACOS
	#error "CE_WINDOW_COCOA is set on a platform other than macOS."
#endif

// The GraphicsApi enumerator a launch without -g starts on: the first entry of CE_RENDER_BACKENDS. No fallback - there is
// no backend that is always present to fall back to.
#ifndef CE_RENDER_DEFAULT_API
	#error "CE_RENDER_DEFAULT_API is not defined - link CE_Config."
#endif

#endif //CE_DEFINE_CONFIG_HPP
