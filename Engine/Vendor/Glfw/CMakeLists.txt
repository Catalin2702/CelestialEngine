cmake_minimum_required(VERSION 3.21)

find_package(glfw3 CONFIG REQUIRED)

add_library(CE_Glfw INTERFACE)

set_target_properties(CE_Glfw PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Glfw INTERFACE
	CE_Config
)

target_link_libraries(CE_Glfw INTERFACE
	CE_Config
	glfw
)
