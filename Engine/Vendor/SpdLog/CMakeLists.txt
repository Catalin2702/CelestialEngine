#
# Module: CelestialEngine/Engine/Vendor/SpdLog
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

find_package(spdlog CONFIG REQUIRED)

add_library(CE_Vendor_SpdLog INTERFACE)

set_target_properties(CE_Vendor_SpdLog PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Vendor_SpdLog INTERFACE
	CE_Config
)

# Third-party. Every configuration, Dist included: Dist keeps CE_CRITICAL / CE_CORE_CRITICAL and gives them a stderr and
# a file sink (Tools/Log/Log.cpp), so it needs spdlog like the others.
target_link_libraries(CE_Vendor_SpdLog INTERFACE
	spdlog::spdlog
)
