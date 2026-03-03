cmake_minimum_required(VERSION 3.21)

find_package(spdlog CONFIG REQUIRED)

add_library(CE_SpdLog INTERFACE)

set_target_properties(CE_SpdLog PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_SpdLog INTERFACE
	CE_Config
)

target_link_libraries(CE_SpdLog INTERFACE
	$<$<NOT:$<CONFIG:Dist>>:
		spdlog::spdlog
	>
)
