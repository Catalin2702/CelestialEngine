#
# Module: CelestialEngine/Engine/Vendor/Glm
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-10
# Updated by: Catalin Chirosca
# Updated: 2026-03-10
#

cmake_minimum_required(VERSION 3.21)

find_package(glm CONFIG REQUIRED)

add_library(CE_Glm INTERFACE)

set_target_properties(CE_Glm PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Glm INTERFACE
	CE_Config
)

# Third-party
target_link_libraries(CE_Glm INTERFACE
	glm::glm
)
