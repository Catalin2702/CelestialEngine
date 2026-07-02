#
# Module: CelestialEngine/Engine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-07-02
#

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_Engine LANGUAGES CXX VERSION 0.1.3)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

set(SOURCES
	src/implementation/Platforms/Mac/MetalCppImplementation.cpp
)

# Vendor libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Glfw)
add_subdirectory(Vendor/Glm)
add_subdirectory(Vendor/ImGui)
add_subdirectory(Vendor/MetalCpp)
add_subdirectory(Vendor/SpdLog)

# Module libraries
add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Types)
add_subdirectory(Modules/Utility)

# Shaders
add_subdirectory(Shaders)

add_library(CE_Engine INTERFACE
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Engine PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Engine INTERFACE
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_Native
	CE_Tools
	CE_Types
)

# Platform specific modules
target_link_libraries(CE_Engine INTERFACE
	$<$<PLATFORM_ID:Darwin>:
		CE_Vendor_MetalCpp
	>
)

# Shaders
target_link_libraries(CE_Engine INTERFACE
	CE_Shaders
)

target_include_directories(CE_Engine INTERFACE
	src/include
	src/internal
)

target_compile_definitions(CE_Engine INTERFACE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Engine)
