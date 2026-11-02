#
# Module: CelestialEngine/Engine/Shaders
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

set(SUBDIRECTORY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# One shader set per compiled-in graphics backend (CE_RENDER_BACKENDS). A build without Metal therefore never runs the
# Metal compiler, nor requires the Metal Toolchain to be installed.
#
# The subdirectories publish their outputs as CACHE INTERNAL variables (CE_OPENGL_SHADERS_SOURCES,
# CE_METAL_SHADERS_LIB_PATH), which survive a re-configure that drops the backend. They are cleared here in that case,
# so nothing downstream can pick up a stale value from a previous selection.
if (CE_RENDER_OPENGL)
	add_subdirectory("${SUBDIRECTORY_PATH}/Platforms/Common/OpenGl")
else ()
	unset(CE_OPENGL_SHADERS_SOURCES CACHE)
endif ()

if (CE_RENDER_METAL)
	add_subdirectory("${SUBDIRECTORY_PATH}/Platforms/Mac/Metal")
else ()
	unset(CE_METAL_SHADERS_LIB_PATH CACHE)
	unset(CE_METAL_MAIN_LIB CACHE)
endif ()

add_library(CE_Shaders INTERFACE)

# NOTE: add_dependencies() does not evaluate generator expressions, so the
# backend-specific shader targets have to be added with a plain if().
if (CE_RENDER_OPENGL)
	add_dependencies(CE_Shaders
		CE_OpenGLShaders
	)
endif ()

if (CE_RENDER_METAL)
	add_dependencies(CE_Shaders
		CE_MetalShaders
	)
endif ()
