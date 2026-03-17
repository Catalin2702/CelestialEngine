#
# Module: CelestialEngine/Engine/Modules/Native/Apple
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-17
#

cmake_minimum_required(VERSION 3.21)

# Create the CE_Apple library (interface library since it aggregates platform-specific APIs)
add_library(CE_Apple INTERFACE)

# Add the Metal subdirectory
add_subdirectory(Metal)
add_subdirectory(Utility)

# Link Metal to CE_Apple
target_link_libraries(CE_Apple INTERFACE
	CE_Metal
	CE_AppleUtility
)
