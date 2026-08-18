#
# Module: CelestialEngine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-22
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
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

project(CelestialEngine LANGUAGES CXX VERSION 0.1.3)

enable_testing()

# Building the tests is what makes CMake create Binaries/<toolchain>/<config>/Tests/ - and it
# creates it at GENERATE time, so the folder shows up (empty) even in a build that only ever
# compiles CE_App. Turn this off to configure the app alone, with no Tests/ folder at all:
#   cmake --preset config-preset-clang -DCE_BUILD_TESTS=OFF
# It is a cache variable read at configure time, so flipping it needs a re-configure.
option(CE_BUILD_TESTS "Build the test executables" ON)

add_subdirectory(Engine)
add_subdirectory(App)

if (CE_BUILD_TESTS)
	add_subdirectory(Tests)
endif ()
