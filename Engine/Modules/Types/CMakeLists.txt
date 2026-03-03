set(HEADERS
	src/include/Types/Window/WindowProps.hpp
	src/include/Types/Window/WindowDestructor.hpp
)

set(SOURCES
	src/source/Types/Window/WindowProps.cpp
	src/source/Types/Window/WindowDestructor.cpp
)

add_library(CE_Types SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Types PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
)

# Third-party
target_link_libraries(CE_Types PUBLIC
	glfw
)

target_include_directories(CE_Types PUBLIC
	src/include
)
