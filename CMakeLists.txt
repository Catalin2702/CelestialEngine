#
# Module: CelestialEngine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-22
# Updated by: Catalin Chirosca
# Updated: 2026-03-22
#

cmake_minimum_required(VERSION 3.21)

# Configure vcpkg triplet based on build type BEFORE project()
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Dist")
	# Use custom triplet that only builds release libraries (no debug)
	if(NOT DEFINED VCPKG_TARGET_TRIPLET)
		if (APPLE)
			set(VCPKG_TARGET_TRIPLET "arm64-osx-release" CACHE STRING "")
		elseif (WIN32)
			set(VCPKG_TARGET_TRIPLET "x64-windows-release" CACHE STRING "")
		elseif (UNIX)
			set(VCPKG_TARGET_TRIPLET "x64-linux-release" CACHE STRING "")
		endif ()
	endif()
endif()

project(CelestialEngine LANGUAGES CXX)

enable_testing()

add_subdirectory(Engine)
add_subdirectory(App)
add_subdirectory(Tests)
