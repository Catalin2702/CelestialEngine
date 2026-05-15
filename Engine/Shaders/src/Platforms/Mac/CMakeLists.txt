#
# Module: CelestialEngine/Engine/Shaders/Platforms/Mac
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-05-15
#

cmake_minimum_required(VERSION 3.21)

set(BUILD_DIR  ${CMAKE_CURRENT_BINARY_DIR})
set(SHADERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(VERTEX_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Fragment.metal"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Vertex.metal"
)

set(VERTEX_AIRS "")

set(MAIN_AIRS "")

set(MAIN_LIB "Main.metallib" CACHE INTERNAL "")

set(MAIN_LIB_PATH "${BUILD_DIR}/${MAIN_LIB}")

# Compile vertex shaders
foreach (SOURCE IN LISTS VERTEX_SOURCES)
	get_filename_component(FILENAME ${SOURCE} NAME_WE)
	set(AIR "${BUILD_DIR}/${FILENAME}.air")

	add_custom_command(
		OUTPUT ${AIR}
		COMMAND xcrun -sdk macosx metal -c ${SOURCE} -I ${SHADERS_INCLUDE_DIR} -o ${AIR}
		DEPENDS ${SOURCE}
		COMMENT "Compiling ${FILENAME}.metal"
	)
	list(APPEND VERTEX_AIRS ${AIR})
endforeach ()
list(APPEND MAIN_AIRS ${VERTEX_AIRS})

add_custom_command(
	OUTPUT ${MAIN_LIB_PATH}
	COMMAND xcrun -sdk macosx metallib ${MAIN_AIRS} -o ${MAIN_LIB_PATH}
	DEPENDS ${MAIN_AIRS}
	COMMENT "Linking ${MAIN_LIB}"
)

add_custom_target(CE_MetalShaders ALL
	DEPENDS ${MAIN_LIB_PATH}
)


set(CE_METAL_SHADERS_LIB_PATH ${MAIN_LIB_PATH} CACHE INTERNAL "")
set(CE_METAL_MAIN_LIB ${MAIN_LIB} CACHE INTERNAL "")
