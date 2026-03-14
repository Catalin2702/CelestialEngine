#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/MetalBridge
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-26
# Updated by: Catalin Chirosca
# Updated: 2026-03-14
#

set(HEADERS
	src/include/MetalBridge/Cocoa/CocoaBridge.h
	src/include/MetalBridge/ImGui/ImGuiBridge.h
)

set(SOURCES
	src/source/MetalBridge/Cocoa/CocoaBridge.mm
	src/source/MetalBridge/ImGui/ImGuiBridge.mm
)

add_library(CE_MetalBridge SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_MetalBridge PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_MetalBridge PUBLIC
	CE_Config
	CE_ImGui
)

# Third-party
target_link_libraries(CE_MetalBridge PUBLIC
	CE_MetalCpp
)

target_compile_options(CE_MetalBridge
	PUBLIC -ObjC++
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
		-fno-objc-arc
	>
)

target_include_directories(CE_MetalBridge PUBLIC
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_MetalBridge)

