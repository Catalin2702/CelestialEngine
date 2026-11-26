#
# Module: CelestialEngine/Engine/Tests/Programs/ShaderCompiler
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# CE_ShaderCompiler run as the build runs it, on small inputs of its own: a shader that compiles to every output, one
# with an error, one whose extension names no stage. They check the program's contract with the build - an exit code
# and a readable message - which the engine's shaders alone never exercise, since they compile.
set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Output")

add_test(NAME ShaderCompiler.ValidShader
	COMMAND CE_ShaderCompiler
		--input "${DATA_DIR}/Valid.vert"
		--spirv "${OUTPUT_DIR}/Valid.spv"
		--msl "${OUTPUT_DIR}/Valid.metal"
		--glsl "${OUTPUT_DIR}/Valid.glsl"
		--reflection "${OUTPUT_DIR}/Valid.reflect"
)

# The diagnostic names the file and the line, the way compiler errors do, so a build log points straight at it. A
# test with PASS_REGULAR_EXPRESSION passes on its output alone, so WILL_FAIL is not combined with it.
add_test(NAME ShaderCompiler.BrokenShader
	COMMAND CE_ShaderCompiler --input "${DATA_DIR}/Broken.frag" --spirv "${OUTPUT_DIR}/Broken.spv"
)

add_test(NAME ShaderCompiler.UnknownStage
	COMMAND CE_ShaderCompiler --input "${DATA_DIR}/Unknown.comp" --spirv "${OUTPUT_DIR}/Unknown.spv"
)

set_tests_properties(ShaderCompiler.BrokenShader PROPERTIES
	PASS_REGULAR_EXPRESSION "Broken\\.frag:[0-9]+"
)

set_tests_properties(ShaderCompiler.UnknownStage PROPERTIES
	PASS_REGULAR_EXPRESSION "does not name a stage"
)

set_tests_properties(ShaderCompiler.ValidShader ShaderCompiler.BrokenShader ShaderCompiler.UnknownStage PROPERTIES
	LABELS "Engine;ShaderCompiler"
)
