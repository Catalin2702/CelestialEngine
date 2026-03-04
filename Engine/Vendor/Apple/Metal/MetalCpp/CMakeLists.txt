#
# Module: CelestialEngine/Engine/Vendor/Apple/Metal/MetalCpp
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-18
# Updated by: Catalin Chirosca
# Updated: 2026-03-03
#

set(SOURCES
	src/source/Define/MetalDefine.cpp
)

add_library(MetalCpp SHARED
	${SOURCES}
)

set_target_properties(MetalCpp PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(MetalCpp PUBLIC
	CE_Config
)

# Third-party
target_link_libraries(MetalCpp PUBLIC
	"-framework AppKit"
	"-framework CoreVideo"
	"-framework Foundation"
	"-framework Metal"
	"-framework MetalKit"
	"-framework QuartzCore"
)

target_compile_options(MetalCpp
	PUBLIC -ObjC++ -fno-objc-arc
)

target_include_directories(MetalCpp PUBLIC
	src/include
)
