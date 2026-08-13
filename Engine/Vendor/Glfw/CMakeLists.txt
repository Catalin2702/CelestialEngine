#
# Module: CelestialEngine/Engine/Vendor/Glfw
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-08-13
#

cmake_minimum_required(VERSION 4.1)

find_package(glfw3 CONFIG REQUIRED)

add_library(CE_Vendor_Glfw INTERFACE)

set_target_properties(CE_Vendor_Glfw PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Vendor_Glfw INTERFACE
	CE_Config
)

# Third-party
target_link_libraries(CE_Vendor_Glfw INTERFACE
	glfw
)
