#
# Module: CelestialEngine/Engine/Shaders
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

# Every shader is written once, in Vulkan-style GLSL (src/<Name>.vert / .frag), and CE_ShaderCompiler turns it into
# what each compiled-in backend (CE_RENDER_BACKENDS) loads:
#   Metal  -> Generated/Metal/<Name>.metal, compiled by Apple's compiler into .air and linked into Main.metallib;
#   OpenGL -> Generated/OpenGL/<Name>.glsl, GLSL 4.10 the driver compiles at run time;
#   always -> Generated/SPIRV/<Name>.spv (the intermediate form) and Generated/Reflection/<Name>.reflect, what the
#             shader declares, read by the engine's tests.
# The name of the file is the engine's name for the shader (ShaderModuleDescriptor::name).
set(SOURCES
	src/source/CompositeFragment.frag
	src/source/CompositeVertex.vert
	src/source/Fragment.frag
	src/source/Vertex.vert
)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated")

set(ALL_OUTPUTS "")
set(GLSL_OUTPUTS "")
set(MSL_OUTPUTS "")

foreach (SOURCE IN LISTS SOURCES)
	get_filename_component(NAME "${SOURCE}" NAME_WE)
	set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")

	set(SPIRV "${GENERATED_DIR}/SPIRV/${NAME}.spv")
	set(REFLECTION "${GENERATED_DIR}/Reflection/${NAME}.reflect")

	set(OUTPUTS "${SPIRV}" "${REFLECTION}")
	set(ARGUMENTS --input "${SOURCE_PATH}" --name "${NAME}" --spirv "${SPIRV}" --reflection "${REFLECTION}")

	# Only the languages of the selected backends: a build without Metal never writes MSL.
	if (CE_RENDER_OPENGL)
		set(GLSL "${GENERATED_DIR}/OpenGL/${NAME}.glsl")
		list(APPEND OUTPUTS "${GLSL}")
		list(APPEND ARGUMENTS --glsl "${GLSL}")
		list(APPEND GLSL_OUTPUTS "${GLSL}")
	endif ()

	if (CE_RENDER_METAL)
		set(MSL "${GENERATED_DIR}/Metal/${NAME}.metal")
		list(APPEND OUTPUTS "${MSL}")
		list(APPEND ARGUMENTS --msl "${MSL}")
		list(APPEND MSL_OUTPUTS "${MSL}")
	endif ()

	list(APPEND ALL_OUTPUTS ${OUTPUTS})

	# The command names the target, not a path: CMake substitutes the executable of the configuration being built and
	# makes this step wait for it. Depending on CE_ShaderCompiler too re-runs every shader when the compiler changes.
	add_custom_command(
		OUTPUT ${OUTPUTS}
		COMMAND CE_ShaderCompiler ${ARGUMENTS}
		DEPENDS "${SOURCE_PATH}" CE_ShaderCompiler
		COMMENT "Cross-compiling ${NAME}"
		VERBATIM
	)
endforeach ()


# One target owns every cross-compiler command, and every target below depends on it. One command writes both the .metal
# and the .glsl of a shader, so the Metal and OpenGL targets need the same commands; Xcode's build system refuses a
# command shared by two targets unless both depend on a third that runs it (Ninja would not mind, Xcode fails to
# configure). Its outputs also include the reflection, which the engine's tests read.
add_custom_target(CE_CrossCompiledShaders ALL
	DEPENDS ${ALL_OUTPUTS}
)

# Metal: the generated .metal files take the same road the hand-written ones did - one .air per file, all linked into
# one library.
if (CE_RENDER_METAL)
	execute_process(
		COMMAND xcrun -sdk macosx metal -v
		RESULT_VARIABLE CE_METAL_TOOLCHAIN_RESULT
		OUTPUT_QUIET
		ERROR_VARIABLE CE_METAL_TOOLCHAIN_ERROR
	)

	if (NOT CE_METAL_TOOLCHAIN_RESULT EQUAL 0)
		message(FATAL_ERROR
			"Metal Toolchain not found - the Metal shaders cannot be compiled.\n"
			"Install it with: xcodebuild -downloadComponent MetalToolchain\n"
			"xcrun said: ${CE_METAL_TOOLCHAIN_ERROR}"
		)
	endif ()

	set(METAL_LIB "Main.metallib")
	set(METAL_LIB_PATH "${GENERATED_DIR}/${METAL_LIB}")

	set(AIRS "")
	foreach (MSL IN LISTS MSL_OUTPUTS)
		get_filename_component(NAME "${MSL}" NAME_WE)
		set(AIR "${GENERATED_DIR}/Metal/${NAME}.air")

		add_custom_command(
			OUTPUT "${AIR}"
			COMMAND xcrun -sdk macosx metal -c "${MSL}" -o "${AIR}"
			DEPENDS "${MSL}"
			COMMENT "Compiling ${NAME}.metal"
			VERBATIM
		)
		list(APPEND AIRS "${AIR}")
	endforeach ()

	add_custom_command(
		OUTPUT "${METAL_LIB_PATH}"
		COMMAND xcrun -sdk macosx metallib ${AIRS} -o "${METAL_LIB_PATH}"
		DEPENDS ${AIRS}
		COMMENT "Linking ${METAL_LIB}"
		VERBATIM
	)

	add_custom_target(CE_MetalShaders ALL
		DEPENDS "${METAL_LIB_PATH}"
	)

	add_dependencies(CE_MetalShaders
		CE_CrossCompiledShaders
	)

	set(CE_METAL_SHADERS_LIB_PATH "${METAL_LIB_PATH}" CACHE INTERNAL "")
	set(CE_METAL_MAIN_LIB "${METAL_LIB}" CACHE INTERNAL "")
else ()
	unset(CE_METAL_SHADERS_LIB_PATH CACHE)
	unset(CE_METAL_MAIN_LIB CACHE)
endif ()

# OpenGL: nothing to build past the cross-compiler, the driver compiles GLSL at run time.
if (CE_RENDER_OPENGL)
	add_custom_target(CE_OpenGLShaders ALL
		DEPENDS ${GLSL_OUTPUTS}
	)

	add_dependencies(CE_OpenGLShaders
		CE_CrossCompiledShaders
	)

	set(CE_OPENGL_SHADERS_SOURCES ${GLSL_OUTPUTS} CACHE INTERNAL "")
else ()
	unset(CE_OPENGL_SHADERS_SOURCES CACHE)
endif ()

set(CE_SHADERS_REFLECTION_DIR "${GENERATED_DIR}/Reflection" CACHE INTERNAL "")
set(CE_SHADERS_GENERATED_DIR "${GENERATED_DIR}" CACHE INTERNAL "")

# The sources are attached to CE_Shaders so IDEs list them under a target; nothing compiles them as C++.
add_library(CE_Shaders INTERFACE
	${SOURCES}
)

add_dependencies(CE_Shaders
	CE_CrossCompiledShaders
)

# add_dependencies() does not evaluate generator expressions, so the backend targets are added with a plain if().
if (CE_RENDER_METAL)
	add_dependencies(CE_Shaders
		CE_MetalShaders
	)
endif ()

if (CE_RENDER_OPENGL)
	add_dependencies(CE_Shaders
		CE_OpenGLShaders
	)
endif ()