#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-21
# Updated by: Catalin Chirosca
# Updated: 2026-03-22
#

set(HEADERS
	# View
	src/include/AppKit/View/RenderView.h
	src/include/AppKit/View/RenderViewCallback.h

	# Window
	src/include/AppKit/Window/WindowDelegate.h
	src/include/AppKit/Window/WindowDelegateCallback.h
	src/include/AppKit/Window/WindowDelegate.hpp
)

set(SOURCES
	# View
	src/source/AppKit/View/RenderView.mm

	# Window
	src/source/AppKit/Window/WindowDelegate.mm
)

add_library(CE_AppKit SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_AppKit PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_AppKit PUBLIC
	CE_Config
	CE_Bridge
)

target_link_libraries(CE_AppKit PUBLIC
	CE_MetalCpp
)

target_compile_options(CE_AppKit PUBLIC
	-ObjC++ -fno-objc-arc

	$<$<CONFIG:Dist>:
		-Wno-nan-infinity-disabled
	>
)

target_include_directories(CE_AppKit PUBLIC
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_AppKit)
