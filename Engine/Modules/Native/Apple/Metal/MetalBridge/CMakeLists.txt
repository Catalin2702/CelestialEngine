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
)

# Third-party
target_link_libraries(CE_MetalBridge PUBLIC
	CE_ImGui
	MetalCpp
)

target_compile_options(CE_MetalBridge
	PUBLIC -ObjC++ -fno-objc-arc
)

target_include_directories(CE_MetalBridge PUBLIC
	src/include
)
