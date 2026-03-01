set(SOURCES
	src/source/Define/MetalDefine.cpp
)

add_library(CE_MetalCpp SHARED
	${SOURCES}
)

set_target_properties(CE_MetalCpp PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_MetalCpp PUBLIC
	CE_Config

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
