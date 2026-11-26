#
# Module: CelestialEngine/Engine/ThirdParty/Glm
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-10
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

find_package(glm CONFIG REQUIRED)

add_library(CE_Vendor_Glm INTERFACE)

set_target_properties(CE_Vendor_Glm PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Vendor_Glm INTERFACE
	CE_Config
)

# Third-party
target_link_libraries(CE_Vendor_Glm INTERFACE
	glm::glm
)

# glm's configuration, the same in every translation unit that sees glm. Header-only, so nothing is compiled with it: two
# files that saw different GLM_FORCE_* would compile the same inline functions two ways, an ODR violation no linker
# reports. Every configuration macro goes here, as an INTERFACE definition, never as a #define in a header.
# CE_GLM_CONFIGURED travels with them: Types/Math/Glm.hpp refuses to compile without it.
target_compile_definitions(CE_Vendor_Glm INTERFACE
	CE_GLM_CONFIGURED=1
)

# Only CE_Types links glm; every other target gets it, with the configuration above, through CE_Types.
ce_restrict_link(CE_Vendor_Glm ALLOWED CE_Types)
ce_restrict_link(glm::glm ALLOWED CE_Vendor_Glm)
