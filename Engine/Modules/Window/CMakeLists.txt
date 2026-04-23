#
# Module: CelestialEngine/Engine/Modules/Window
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-17
# Updated by: Catalin Chirosca
# Updated: 2026-04-23
#

set(HEADERS
	src/include/Window/Platforms/Common/Glfw/GlfwWindow.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp
	>
)

set(INTERFACES
	src/interface/Window/I_Window.hpp
)

set(SOURCES
	src/source/Window/Platforms/Common/Glfw/GlfwWindow.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Window/Platforms/Mac/Cocoa/CocoaWindow.cpp
	>
)

add_library(CE_Window SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Window PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Window PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Vendor_Glad
	CE_Vendor_Glfw
	CE_Native
	CE_Vendor_SpdLog
	CE_Tools
	CE_Types
	CE_Utility
)

# Platform-specific
target_link_libraries(CE_Window PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		CE_Vendor_MetalCpp
	>
)

target_include_directories(CE_Window PUBLIC
	src/include
	src/interface
	src/internal
	src/template
)

target_compile_definitions(CE_Window PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Window)
