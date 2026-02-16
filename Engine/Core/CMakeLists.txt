set(HEADERS
	src/include/Core/Core.hpp
	src/include/Core/Application.hpp
)

set(SOURCES
	src/source/Core/Application.cpp
)

add_library(CE_Core SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Core PRIVATE
	CE_Config
)

target_include_directories(CE_Core PUBLIC
	src/include
)

target_compile_definitions(CE_Core PRIVATE CE_CORE_EXPORTS)
