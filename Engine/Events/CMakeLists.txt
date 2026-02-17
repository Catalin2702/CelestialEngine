set(HEADERS
	src/include/Events/ApplicationEvent.hpp
	src/include/Events/Event.hpp
	src/include/Events/KeyEvent.hpp
	src/include/Events/MouseEvent.hpp
)

set(SOURCES
	src/source/Events/ApplicationEvent.cpp
	src/source/Events/Event.cpp
	src/source/Events/KeyEvent.cpp
	src/source/Events/MouseEvent.cpp
)

set(TEMPLATES
	src/template/Events/Event.tpp
)

add_library(CE_Events SHARED
	${HEADERS}
	${SOURCES}
	${TEMPLATES}
)

set_target_properties(CE_Events PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Events
	PRIVATE CE_Config
	PRIVATE CE_Define
)

target_include_directories(CE_Events PUBLIC
	src/include
	../Core/src/include
	../Define/src/include
)
