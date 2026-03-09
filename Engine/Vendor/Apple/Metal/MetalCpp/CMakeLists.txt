#
# Module: CelestialEngine/Engine/Vendor/Apple/Metal/MetalCpp
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-18
# Updated by: Catalin Chirosca
# Updated: 2026-03-09
#

set(SOURCES
	src/source/Define/MetalDefine.cpp
)

add_library(CE_MetalCpp SHARED
	${SOURCES}
)

set_target_properties(CE_MetalCpp PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_MetalCpp PUBLIC
	CE_Config
)

# Third-party
target_link_libraries(CE_MetalCpp PUBLIC
	"-framework AppKit"
	"-framework CoreVideo"
	"-framework Foundation"
	"-framework Metal"
	"-framework MetalKit"
	"-framework QuartzCore"
)

target_compile_options(CE_MetalCpp
	PUBLIC -ObjC++ -fno-objc-arc
)

target_include_directories(CE_MetalCpp PUBLIC
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_MetalCpp)

