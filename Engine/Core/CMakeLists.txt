cmake_minimum_required(VERSION 4.1)

project(Core LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CORE_HEADERS
	src/include/Core/Test.hpp
	src/internal/Core/Core.hpp
	src/include/Core/CoreApi.hpp
)

set(CORE_SOURCES
	src/Test.cpp
)

add_library(Core SHARED ${CORE_HEADERS} ${CORE_SOURCES})

target_include_directories(Core PRIVATE
	src/internal
)

target_include_directories(Core PUBLIC
	src/include
)


target_compile_definitions(Core PRIVATE CORE_EXPORTS)

target_compile_options(Core PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:
	/W4
	/WX
	/permissive-
	>

	$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
	-Wall
	-Wextra
	-Werror
	>
)
