#
# Module: CelestialEngine/Engine/Shaders
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-05-16
#

cmake_minimum_required(VERSION 3.21)

set(SUBDIRECTORY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory("${SUBDIRECTORY_PATH}/Platforms/Common/OpenGl")

if (APPLE)
	add_subdirectory("${SUBDIRECTORY_PATH}/Platforms/Mac/Metal")
endif ()

add_library(CE_Shaders INTERFACE)

add_dependencies(CE_Shaders
	CE_OpenGLShaders
	$<$<PLATFORM_ID:Darwin>:
		CE_MetalShaders
	>
)
