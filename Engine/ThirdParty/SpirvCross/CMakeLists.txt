#
# Module: CelestialEngine/Engine/ThirdParty/SpirvCross
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# SPIRV-Cross: reads SPIR-V and writes it back out as source in another shading language (MSL, GLSL, HLSL), and
# answers questions about it (reflection). One package per back end; the core is pulled in by each of them.
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(spirv_cross_glsl CONFIG REQUIRED)
find_package(spirv_cross_msl CONFIG REQUIRED)

add_library(CE_Vendor_SpirvCross INTERFACE)

set_target_properties(CE_Vendor_SpirvCross PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Vendor_SpirvCross INTERFACE
	CE_Config
)

# Third-party
target_link_libraries(CE_Vendor_SpirvCross INTERFACE
	spirv-cross-core
	spirv-cross-glsl
	spirv-cross-msl
)

# Same rule as glslang: a build tool's dependency, kept out of the engine.
ce_restrict_link(CE_Vendor_SpirvCross ALLOWED CE_ShaderCompiler)
