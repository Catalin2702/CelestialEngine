#
# Module: CelestialEngine/Engine/Modules/Render
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-19
# Updated by: Catalin Chirosca
# Updated: 2026-03-31
#

set(HEADERS
	# Context
	src/include/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Render/Context/Platforms/Mac/Metal/MetalContext.hpp
	>
)

set(INTERFACES
	# Context
	src/interface/Render/Context/I_Context.hpp
)

set(SOURCES
	# Context
	src/source/Render/Context/Platforms/Common/OpenGl/OpenGlContext.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Render/Context/Platforms/Mac/Metal/MetalContext.cpp
	>
)

add_library(CE_Render SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Render PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

target_link_libraries(CE_Render PUBLIC
	CE_Config
	CE_Define
	CE_Native
	CE_Tools
	CE_Types
	CE_Utility
	CE_Window
)

target_link_libraries(CE_Render PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		CE_MetalCpp
	>
)

target_link_libraries(CE_Render PUBLIC
	CE_Glad
)

target_include_directories(CE_Render PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Render PRIVATE
	CE_DEFINE_EXPORTS
)

ce_copy_to_last(CE_Render)
