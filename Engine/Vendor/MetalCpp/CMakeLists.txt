#
# Module: CelestialEngine/Engine/Vendor/MetalCpp
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-07-02
# Updated by: Catalin Chirosca
# Updated: 2026-08-13
#

cmake_minimum_required(VERSION 4.1)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Submodules/MetalCpp MetalCpp)

add_library(CE_Vendor_MetalCpp INTERFACE)

set_target_properties(CE_Vendor_MetalCpp PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Vendor_MetalCpp INTERFACE
	CE_Config
	METAL_CPP
)

target_include_directories(CE_Vendor_MetalCpp INTERFACE
	${CMAKE_SOURCE_DIR}/Engine/Submodules/MetalCpp
)
