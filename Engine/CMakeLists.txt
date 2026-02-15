cmake_minimum_required(VERSION 4.1)

include("${CMAKE_SOURCE_DIR}/Engine/config/cmake/CelestialEngineMacros.cmake")

project(CelestialEngine LANGUAGES CXX VERSION 0.0.1)

set(CE_HEADERS
	src/include/CelestialEngine.hpp
	src/EntryPoint.hpp
)

add_subdirectory(Core)

add_library(CelestialEngine SHARED
	${CE_HEADERS}
)

set_target_properties(CelestialEngine PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CelestialEngine
	PRIVATE CE_Config
	PUBLIC CE_Core
)

target_include_directories(CelestialEngine PUBLIC
	src/include
)
