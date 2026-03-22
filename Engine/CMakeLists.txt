#
# Module: CelestialEngine/Engine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-22
#

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_Engine LANGUAGES CXX VERSION 0.0.6)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

# Vendor libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Glfw)
add_subdirectory(Vendor/Glm)
add_subdirectory(Vendor/ImGui)
add_subdirectory(Vendor/SpdLog)

# Platform-specific vendor libraries
if(APPLE)
	add_subdirectory(Vendor/Apple/Metal/MetalCpp)
endif()

# Modules libraries
add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Render)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Types)
add_subdirectory(Modules/Utility)
add_subdirectory(Modules/Window)

add_library(CE_Engine SHARED
	${HEADERS}
)

set_target_properties(CE_Engine PROPERTIES LINKER_LANGUAGE CXX)

# Modules
target_link_libraries(CE_Engine PUBLIC
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_Native
	CE_Render
	CE_Tools
	CE_Types
	CE_Window
)

target_include_directories(CE_Engine PUBLIC
	src/include
	src/internal
)

target_compile_definitions(CE_Engine PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Engine)
