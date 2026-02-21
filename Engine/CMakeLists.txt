if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/Engine/config/cmake/CelestialEngineMacros.cmake")

project(CelestialEngine LANGUAGES CXX VERSION 0.0.1)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

add_subdirectory(Core)
add_subdirectory(Define)
add_subdirectory(Events)
add_subdirectory(Tools)
add_subdirectory(Types)
add_subdirectory(Window)

if(APPLE)
	add_subdirectory(Vendor/Apple/Metal/MetalCpp)
endif()

add_library(CelestialEngine SHARED
	${HEADERS}
)

set_target_properties(CelestialEngine PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CelestialEngine
	PRIVATE CE_Config
	PUBLIC CE_Core
	PUBLIC CE_Define
	PUBLIC CE_Events
	PUBLIC CE_Tools
	PUBLIC CE_Types
	PUBLIC CE_Window
)

if(APPLE)
	target_link_libraries(CelestialEngine
		PUBLIC CE_MetalCpp
	)
endif()

target_include_directories(CelestialEngine
	PUBLIC src/include
	PUBLIC src/internal
)
