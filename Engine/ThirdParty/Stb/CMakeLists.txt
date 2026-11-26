#
# Module: CelestialEngine/Engine/ThirdParty/Stb
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-20
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

find_package(Stb REQUIRED)

add_library(CE_Vendor_Stb INTERFACE)

set_target_properties(CE_Vendor_Stb PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Vendor_Stb INTERFACE
	CE_Config
)

# Third-party. Header-only, and the port exports no imported target - only the folder the headers are in. SYSTEM, like
# ImGui and metal-cpp: stb_image.h carries its whole implementation, and its warnings are not ours to fix under -Werror.
target_include_directories(CE_Vendor_Stb SYSTEM INTERFACE
	${Stb_INCLUDE_DIR}
)
