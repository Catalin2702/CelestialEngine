if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CelestialEngine LANGUAGES CXX VERSION 0.0.3)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Types)
add_subdirectory(Modules/Window)

# Vendor libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Glfw)
add_subdirectory(Vendor/ImGui)
add_subdirectory(Vendor/SpdLog)

if(APPLE)
	add_subdirectory(Vendor/Apple/Metal/MetalCpp)
endif()

add_library(CelestialEngine SHARED
	${HEADERS}
)

set_target_properties(CelestialEngine PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CelestialEngine PUBLIC
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_Native
	CE_Tools
	CE_Types
	CE_Window
)

if(APPLE)
	target_link_libraries(CelestialEngine PUBLIC
		MetalCpp
	)
endif()

target_include_directories(CelestialEngine PUBLIC
	src/include
	src/internal
)
