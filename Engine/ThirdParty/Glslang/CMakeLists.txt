#
# Module: CelestialEngine/Engine/ThirdParty/Glslang
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# glslang: the Khronos reference compiler from GLSL to SPIR-V. Static libraries (vcpkg's default on macOS), so they end
# up inside the one executable that links them and nowhere else.
find_package(glslang CONFIG REQUIRED)

add_library(CE_Vendor_Glslang INTERFACE)

set_target_properties(CE_Vendor_Glslang PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Vendor_Glslang INTERFACE
	CE_Config
)

# glslang::glslang is the front end (parser, type checker), glslang::SPIRV the SPIR-V generator, and the default
# resource limits the table of GPU limits (max textures, max varyings...) a shader is checked against.
target_link_libraries(CE_Vendor_Glslang INTERFACE
	glslang::glslang
	glslang::SPIRV
	glslang::glslang-default-resource-limits
)

# A shader compiler belongs to the build, never to the engine that runs the shaders: only the shader compiler program
# may link it. An engine module linking it by mistake would carry several MB of compiler into every application.
ce_restrict_link(CE_Vendor_Glslang ALLOWED CE_ShaderCompiler)
