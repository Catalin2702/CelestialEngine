#
# Module: CelestialEngine/Engine/Modules/Window
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-17
# Updated by: Catalin Chirosca
# Updated: 2026-03-03
#

set(HEADERS
	src/include/Window/Platforms/Universal/OpenGlWindow.hpp
)

list(APPEND HEADERS
	$<$<PLATFORM_ID:Darwin>:
	src/include/Window/Platforms/Mac/MetalWindow.hpp
	>
)

set(INTERFACES
	src/interface/Window/I_Window.hpp
)

set(SOURCES
	src/source/Window/Platforms/Universal/OpenGlWindow.cpp
)

# Platform-specific
list(APPEND SOURCES
	$<$<PLATFORM_ID:Darwin>:
	src/source/Window/Platforms/Mac/MetalWindow.cpp
	>
)

add_library(CE_Window SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Window PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Window PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Glad
	CE_Glfw
	CE_Native
	CE_SpdLog
	CE_Tools
	CE_Types
)

# Platform-specific
target_link_libraries(CE_Window PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		MetalCpp
	>
)

target_include_directories(CE_Window PUBLIC
	src/include
	src/interface
	src/internal
	src/template
)
