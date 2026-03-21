#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-21
#

cmake_minimum_required(VERSION 3.21)

# Create the CE_Metal library (interface library since it only aggregates Bridge)
add_library(CE_Metal INTERFACE)

# Add the Bridge subdirectory
add_subdirectory(AppKit)
add_subdirectory(Bridge)

# Link Bridge to CE_Metal
target_link_libraries(CE_Metal INTERFACE
	$<BUILD_INTERFACE:
		CE_AppKit
		CE_Bridge
	>
	$<INSTALL_INTERFACE:
		CE_AppKit
		CE_Bridge
	>
)

## Propagate include directories
target_include_directories(CE_Metal INTERFACE
	$<TARGET_PROPERTY:CE_AppKit,INTERFACE_INCLUDE_DIRECTORIES>
	$<TARGET_PROPERTY:CE_Bridge,INTERFACE_INCLUDE_DIRECTORIES>
)
