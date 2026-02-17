set(HEADERS
)

set(INTERFACES
	src/interface/Window/InterfaceWindow.hpp
)

set(SOURCES
	src/source/Window/InterfaceWindow.cpp
)

if(APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Mac/Window.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Mac/Window.cpp
	)
elseif(WIN32)
	list(APPEND HEADERS
		src/include/Window/Platforms/Windows/Window.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Windows/Window.cpp
	)
elseif(UNIX AND NOT APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Linux/Window.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Linux/Window.cpp
	)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_library(CE_Window SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Window PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Window
	PRIVATE CE_Config
	PRIVATE CE_Define
	PRIVATE CE_Events
	PRIVATE CE_Tools
	PUBLIC glfw
	PUBLIC GLEW::GLEW
)

target_include_directories(CE_Window PUBLIC
	src/include
	../Core/src/include
	../Define/src/include
	../Events/src/include
	../Tools/src/include
)
