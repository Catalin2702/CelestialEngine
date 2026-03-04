#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Metal
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-04
#

cmake_minimum_required(VERSION 3.21)

# Create the CE_Metal library (interface library since it only aggregates MetalBridge)
add_library(CE_Metal INTERFACE)

# Add the MetalBridge subdirectory
add_subdirectory(MetalBridge)

# Link MetalBridge to CE_Metal
target_link_libraries(CE_Metal INTERFACE
	$<BUILD_INTERFACE:CE_MetalBridge>
	$<INSTALL_INTERFACE:CE_MetalBridge>
)

# Propagate include directories from CE_MetalBridge
target_include_directories(CE_Metal INTERFACE
	$<TARGET_PROPERTY:CE_MetalBridge,INTERFACE_INCLUDE_DIRECTORIES>
)
