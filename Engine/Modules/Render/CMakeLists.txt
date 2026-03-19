#
# Module: CelestialEngine/Engine/Modules/Render
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-19
# Updated by: Catalin Chirosca
# Updated: 2026-03-19
#

set(HEADERS
	# Context
	src/include/Render/Context/Platforms/Common/OpenGlContext.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Render/Context/Platforms/Mac/MetalContext.hpp
	>
)

set(INTERFACES
	# Context
	src/interface/Render/Context/I_Context.hpp
)

set(SOURCES
	# Context
	src/source/Render/Context/Platforms/Common/OpenGlContext.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Render/Context/Platforms/Mac/MetalContext.cpp
	>
)

add_library(CE_Render SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Render PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Render PUBLIC
	CE_Config
	CE_Define
	CE_Native
	CE_Tools
	CE_Types
)

target_include_directories(CE_Render PUBLIC
	src/include
	src/interface
)

ce_copy_to_last(CE_Render)
