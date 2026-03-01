set(HEADERS
	src/include/MetalBridge/Cocoa/MetalCocoaBridge.h
	src/include/MetalBridge/ImGui/MetalImGuiBridge.h
)

set(SOURCES
	src/source/MetalBridge/Cocoa/MetalCocoaBridge.mm
	src/source/MetalBridge/ImGui/MetalImGuiBridge.mm
)

add_library(CE_MetalBridge SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_MetalBridge PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_MetalBridge PUBLIC
	CE_Config
	CE_MetalCpp

	imgui::imgui

	"-framework Foundation"
	"-framework Metal"
	"-framework QuartzCore"
	"-framework AppKit"
)

target_compile_options(CE_MetalBridge
	PUBLIC -ObjC++ -fno-objc-arc
)

target_include_directories(CE_MetalBridge PUBLIC
	src/include
)
