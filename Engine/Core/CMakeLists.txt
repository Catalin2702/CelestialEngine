set(HEADERS
	src/include/Core/Application.hpp
	src/include/Layers/Layer.hpp
	src/include/Layers/LayerStack.hpp
)

set(SOURCES
	src/source/Core/Application.cpp
	src/source/Layers/Layer.cpp
	src/source/Layers/LayerStack.cpp
)

find_package(glfw3 CONFIG REQUIRED)

add_library(CE_Core SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Core
	PRIVATE CE_Config
	PRIVATE CE_Define
	PRIVATE CE_Events
	PRIVATE CE_Tools
	PRIVATE CE_Types
	PRIVATE CE_Window
)

target_compile_definitions(CE_Core PRIVATE CE_CORE_EXPORTS)

target_include_directories(CE_Core PUBLIC
	src/include
	../Define/src/include
	../Events/src/include
	../Tools/src/include
	../Types/src/include
	../Window/src/include
	../Window/src/interface
)
