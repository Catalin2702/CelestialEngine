#
# Module: CelestialEngine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-22
# Updated by: Catalin Chirosca
# Updated: 2026-11-03
#

cmake_minimum_required(VERSION 4.1)

# Point non-Apple compilers (e.g. Homebrew LLVM) at the macOS SDK BEFORE project(): once the Darwin platform module has
# run, CMAKE_OSX_SYSROOT is already defined-but-empty and setting it here would be both too late and skipped. CMake
# resolves the "macosx" SDK name through xcrun, so no absolute path is baked in.
if(APPLE AND NOT CMAKE_OSX_SYSROOT)
	set(CMAKE_OSX_SYSROOT "macosx" CACHE STRING "")
endif()

# Configure vcpkg triplet based on build type BEFORE project()
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Dist")
	# Use custom triplet that only builds release libraries (no debug)
	if(NOT DEFINED VCPKG_TARGET_TRIPLET)
		if (APPLE)
			set(VCPKG_TARGET_TRIPLET "arm64-osx-release" CACHE STRING "")
		elseif (WIN32)
			if (CXX_COMPILER_ID STREQUAL "MSVC")
				set(VCPKG_TARGET_TRIPLET "x64-windows-msvc-release" CACHE STRING "")
			elseif (CXX_COMPILER_ID STREQUAL "Clang")
				set(VCPKG_TARGET_TRIPLET "x64-windows-clang-release" CACHE STRING "")
			elseif (CXX_COMPILER_ID STREQUAL "GNU")
				set(VCPKG_TARGET_TRIPLET "x64-windows-gcc-release" CACHE STRING "")
			endif ()
		elseif (UNIX)
			set(VCPKG_TARGET_TRIPLET "x64-linux-release" CACHE STRING "")
		endif ()
	endif()
endif()

# Which graphics (and so window) backends are compiled in: CE_RENDER_BACKENDS, resolved into the CE_RENDER_* and
# CE_WINDOW_* variables every subdirectory below reads. BEFORE project(): the vcpkg toolchain installs the dependencies
# inside project(), and the selection decides which optional ones (glad, GLFW) it has to install at all.
include("${CMAKE_CURRENT_LIST_DIR}/Engine/cmake/CERenderBackends.cmake")

project(CelestialEngine LANGUAGES CXX VERSION 0.1.3)

enable_testing()

# The engine's build machinery - CE_Config, the Binaries/ layout, the module helpers, ce_configure_application - included here,
# at the top, so that both the engine and the applications next to it see the same variables.
include("${CMAKE_CURRENT_LIST_DIR}/Engine/cmake/CelestialEngineMacros.cmake")

# The engine: its modules, vendors, shaders and - with CE_BUILD_TESTS, ON by default - its tests.
add_subdirectory(Engine)

# The applications built on it. Each is self-contained: a directory with add_executable() and one ce_configure_application() call.
add_subdirectory(Apps/Sandbox)
