cmake_minimum_required(VERSION 3.21)

find_package(imgui CONFIG REQUIRED)

add_library(CE_ImGui INTERFACE)

set_target_properties(CE_ImGui PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_ImGui INTERFACE
	CE_Config
)

target_link_libraries(CE_ImGui INTERFACE
	imgui::imgui
)
