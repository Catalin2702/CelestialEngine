#
# Module: CelestialEngine/Engine/Vendor/ImGui
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-03-03
#

cmake_minimum_required(VERSION 3.21)

find_package(imgui CONFIG REQUIRED)

add_library(CE_ImGui INTERFACE)

set_target_properties(CE_ImGui PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_ImGui INTERFACE
	CE_Config
)

# Third-party
target_link_libraries(CE_ImGui INTERFACE
	imgui::imgui
)
