#
# Module: CelestialEngine/Engine/Vendor/Apple/Metal/MetalCpp
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-18
# Updated by: Catalin Chirosca
# Updated: 2026-04-23
#

set(SOURCES
	src/source/Define/MetalDefine.cpp
)

add_library(CE_Vendor_MetalCpp SHARED
	${SOURCES}
)

set_target_properties(CE_Vendor_MetalCpp PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Vendor_MetalCpp PUBLIC
	CE_Config
)

# Third-party
target_link_libraries(CE_Vendor_MetalCpp PUBLIC
	"-framework AppKit"
	"-framework CoreVideo"
	"-framework Foundation"
	"-framework Metal"
	"-framework MetalKit"
	"-framework QuartzCore"
)

target_compile_options(CE_Vendor_MetalCpp
	PUBLIC -ObjC++ -fno-objc-arc
	$<$<CONFIG:Dist>:
		-Wno-nan-infinity-disabled
	>
)

target_include_directories(CE_Vendor_MetalCpp PUBLIC
	src/include
)

target_compile_definitions(CE_Vendor_MetalCpp PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Vendor_MetalCpp)
