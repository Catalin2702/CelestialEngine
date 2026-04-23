#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-04-19
# Updated by: Catalin Chirosca
# Updated: 2026-04-23
#

set(HEADERS
	src/include/CoreAnimation/DisplayLink/DisplayLink.h
	src/include/CoreAnimation/DisplayLink/DisplayLink.hpp
)

set(SOURCES
	src/source/CoreAnimation/DisplayLink/DisplayLink.mm
)

add_library(CE_CoreAnimation SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_CoreAnimation PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_CoreAnimation PUBLIC
	CE_Config
)

target_link_libraries(CE_CoreAnimation PUBLIC
	CE_Vendor_MetalCpp
)


target_compile_options(CE_CoreAnimation PUBLIC
	-ObjC++ -fno-objc-arc

	$<$<CONFIG:Dist>:
		-Wno-nan-infinity-disabled
	>
)

target_include_directories(CE_CoreAnimation PUBLIC
	src/include
)

target_compile_definitions(CE_CoreAnimation PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_CoreAnimation)
