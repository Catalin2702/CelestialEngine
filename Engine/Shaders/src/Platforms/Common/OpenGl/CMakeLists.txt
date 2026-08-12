#
# Module: CelestialEngine/Engine/Shaders/Platforms/Common/OpenGl
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-16
# Updated by: Catalin Chirosca
# Updated: 2026-08-12
#

cmake_minimum_required(VERSION 3.21)

set(SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Fragment.glsl"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Vertex.glsl"
)

# GLSL is compiled at runtime, so there is nothing to build here: the .glsl files
# are exported so that consumers can attach them to their bundle as resources and
# let the generator track them like any other source file.
set(CE_OPENGL_SHADERS_SOURCES ${SOURCES} CACHE INTERNAL "")

add_custom_target(CE_OpenGLShaders ALL
	SOURCES ${SOURCES}
)
