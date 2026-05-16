#
# Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-16
# Updated by: Catalin Chirosca
# Updated: 2026-05-16
#

cmake_minimum_required(VERSION 3.21)

set(SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Fragment.glsl"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Vertex.glsl"
)

set(CE_OPENGL_SHADERS_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source" CACHE INTERNAL "")

add_custom_target(CE_OpenGLShaders ALL
	DEPENDS ${SOURCES}
)
