#
# Module: CelestialEngine/Engine/Shaders/Platforms/Mac/Metal
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-13
#

cmake_minimum_required(VERSION 4.1)

set(BUILD_DIR  ${CMAKE_CURRENT_BINARY_DIR})
set(SHADERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS
	"${SHADERS_INCLUDE_DIR}/Common.h"
)

set(SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Fragment.metal"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Vertex.metal"
)

set(IRS "")

set(MAIN_IRS "")

set(MAIN_LIB "Main.metallib" CACHE INTERNAL "")

set(MAIN_LIB_PATH "${BUILD_DIR}/${MAIN_LIB}" CACHE INTERNAL "")

# Compile vertex shaders
foreach (SOURCE IN LISTS SOURCES)
	get_filename_component(FILENAME ${SOURCE} NAME_WE)
	set(IR "${BUILD_DIR}/${FILENAME}.ir")

	# -MMD/-MF makes the Metal compiler emit a Make-style dependency file listing
	# every header the shader pulled in; DEPFILE hands it to the generator so that
	# editing an included header rebuilds the shader, exactly like .cpp/.hpp pairs.
	add_custom_command(
		OUTPUT ${IR}
		COMMAND xcrun -sdk macosx metal -c ${SOURCE} -I ${SHADERS_INCLUDE_DIR} -MMD -MF ${IR}.d -o ${IR}
		DEPENDS ${SOURCE}
		DEPFILE ${IR}.d
		COMMENT "Compiling ${FILENAME}.metal"
	)
	list(APPEND IRS ${IR})
endforeach ()
list(APPEND MAIN_IRS ${IRS})

add_custom_command(
	OUTPUT ${MAIN_LIB_PATH}
	COMMAND xcrun -sdk macosx metallib ${MAIN_IRS} -o ${MAIN_LIB_PATH}
	DEPENDS ${MAIN_IRS}
	COMMENT "Linking ${MAIN_LIB}"
)

# The shaders are attached to the target through SOURCES, the custom-target
# counterpart of listing HEADERS/SOURCES in add_library(): they are never fed to
# the C++ compiler, they just make the files belong to a target so that IDEs stop
# treating them as loose files. The build itself is driven by DEPENDS.
add_custom_target(CE_MetalShaders ALL
	DEPENDS ${MAIN_LIB_PATH}
	SOURCES
		${HEADERS}
		${SOURCES}
)


set(CE_METAL_SHADERS_LIB_PATH ${MAIN_LIB_PATH} CACHE INTERNAL "")
set(CE_METAL_MAIN_LIB ${MAIN_LIB} CACHE INTERNAL "")
