#
# Module: CelestialEngine/Engine/Programs/ShaderCompiler
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# region Headers

set(HEADERS
	src/include/ShaderCompiler/CrossCompiler.hpp
	src/include/ShaderCompiler/Reflection.hpp
	src/include/ShaderCompiler/ShaderStage.hpp
	src/include/ShaderCompiler/SpirvCompiler.hpp
)

# endregion

# region Sources

set(SOURCES
	src/source/ShaderCompiler/CrossCompiler.cpp
	src/source/ShaderCompiler/main.cpp
	src/source/ShaderCompiler/Reflection.cpp
	src/source/ShaderCompiler/ShaderStage.cpp
	src/source/ShaderCompiler/SpirvCompiler.cpp
)

# endregion

# A program of the engine, not an application: it has its own main(), no window, no engine module. It runs during the
# build (Engine/Shaders), so it must not need anything the build has not produced yet - linking CE_Tools or CE_Types
# would make it load their dylibs, which on macOS are only copied next to the executables that embed them.
add_executable(CE_ShaderCompiler
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_ShaderCompiler PROPERTIES
	LINKER_LANGUAGE CXX
)

target_include_directories(CE_ShaderCompiler PRIVATE
	src/include
	../../Modules/Types/src/include
)

# CE_Config for the engine's warnings and C++ standard; the two compilers are static, so the executable is
# self-contained.
target_link_libraries(CE_ShaderCompiler PRIVATE
	CE_Config
	CE_Vendor_Glslang
	CE_Vendor_SpirvCross
)

# <build>/Out/<config>/Programs/: beside the applications, apart from them.
ce_set_output_subdirectory(CE_ShaderCompiler Programs)
