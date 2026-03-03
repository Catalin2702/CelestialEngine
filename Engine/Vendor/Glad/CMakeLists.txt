cmake_minimum_required(VERSION 3.21)

find_package(glad CONFIG REQUIRED)

add_library(CE_Glad INTERFACE)

set_target_properties(CE_Glad PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Glad INTERFACE
	CE_Config
)

target_link_libraries(CE_Glad INTERFACE
	glad::glad
)
