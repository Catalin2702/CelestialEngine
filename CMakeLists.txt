cmake_minimum_required(VERSION 3.21)

# Configure vcpkg triplet based on build type BEFORE project()
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Dist")
	# Use custom triplet that only builds release libraries (no debug)
	if(NOT DEFINED VCPKG_TARGET_TRIPLET)
		set(VCPKG_TARGET_TRIPLET "arm64-osx-release" CACHE STRING "")
	endif()
endif()

project(CelestialEngine LANGUAGES CXX)

enable_testing()

add_subdirectory(Engine)
add_subdirectory(App)
add_subdirectory(Tests)
