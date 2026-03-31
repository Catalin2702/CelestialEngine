#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/Bridge
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-26
# Updated by: Catalin Chirosca
# Updated: 2026-03-31
#

set(HEADERS
	src/include/Bridge/AppKit/Window/WindowBridge.h
	src/include/Bridge/ImGui/ImGuiBridge.h
)

set(SOURCES
	src/source/Bridge/AppKit/Window/WindowBridge.mm
	src/source/Bridge/ImGui/ImGuiBridge.mm
)

add_library(CE_Bridge SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Bridge PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Bridge PUBLIC
	CE_Config
	CE_ImGui
)

# Third-party
target_link_libraries(CE_Bridge PUBLIC
	CE_MetalCpp

	"-framework GameController"
)

target_compile_options(CE_Bridge
	PUBLIC -ObjC++ -fno-objc-arc

	$<$<CONFIG:Dist>:
		-Wno-nan-infinity-disabled
	>
)

target_include_directories(CE_Bridge PUBLIC
	src/include
)

target_compile_definitions(CE_Bridge PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Bridge)
