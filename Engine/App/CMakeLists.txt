cmake_minimum_required(VERSION 4.1)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "../../vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(AppEngine LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(AppEngine main.cpp)
