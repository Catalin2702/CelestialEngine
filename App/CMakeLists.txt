cmake_minimum_required(VERSION 4.1)

project(App LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(App src/app.cpp)

target_link_libraries(App PRIVATE Core)

target_compile_options(App PRIVATE
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
