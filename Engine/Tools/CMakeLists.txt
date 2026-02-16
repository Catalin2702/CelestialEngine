cmake_minimum_required(VERSION 4.1)

project(CE_Tools LANGUAGES CXX VERSION 0.0.1)

set(HEADERS)

set(SOURCES)

add_library(CE_Tools SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Tools PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Tools PRIVATE
	CE_Config
)
