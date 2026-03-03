set(HEADERS
	src/include/Events/ApplicationEvent.hpp
	src/include/Events/I_Event.hpp
	src/include/Events/KeyEvent.hpp
	src/include/Events/MouseEvent.hpp
)

set(SOURCES
	src/source/Events/ApplicationEvent.cpp
	src/source/Events/I_Event.cpp
	src/source/Events/KeyEvent.cpp
	src/source/Events/MouseEvent.cpp
)

add_library(CE_Events SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Events PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Events PUBLIC
	CE_Config
	CE_Define
)

target_include_directories(CE_Events PUBLIC
	src/include
	src/template
)
