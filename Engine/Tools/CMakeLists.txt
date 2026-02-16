set(HEADERS
	src/include/Tools/Log/Log.hpp
)

set(SOURCES
	src/source/Tools/Log/Log.cpp
)

find_package(spdlog CONFIG REQUIRED)

add_library(CE_Tools SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Tools PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Tools
	PRIVATE CE_Config
	PUBLIC spdlog::spdlog
)

target_include_directories(CE_Tools PUBLIC
	src/include
	../Core/src/include
)
