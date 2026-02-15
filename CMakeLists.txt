cmake_minimum_required(VERSION 4.1)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(CelestialEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(Engine/Core)
add_subdirectory(App)
