set(SOURCES
	src/source/Define/MetalDefine.cpp
)

find_library(APPKIT_LIBRARY AppKit REQUIRED)
find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
find_library(METAL_LIBRARY Metal REQUIRED)
find_library(METALKIT_LIBRARY MetalKit REQUIRED)
find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)

add_library(CE_MetalCpp SHARED
${SOURCES}
)

set_target_properties(CE_MetalCpp PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_MetalCpp
	PRIVATE CE_Config

	PUBLIC ${APPKIT_LIBRARY}
	PUBLIC ${FOUNDATION_LIBRARY}
	PUBLIC ${METAL_LIBRARY}
	PUBLIC ${METALKIT_LIBRARY}
	PUBLIC ${QUARTZCORE_LIBRARY}
)

target_compile_options(CE_MetalCpp
	PUBLIC -ObjC++
)

target_include_directories(CE_MetalCpp PUBLIC
	src/include
)
