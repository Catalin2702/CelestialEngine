set(HEADERS
	src/include/Utils/Window.hpp
)

set(INTERFACES
	src/interface/Window/InterfaceWindow.hpp
)

set(INTERNALS)

set(SOURCES
	src/source/Window/InterfaceWindow.cpp
)

if(APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Mac/MetalViewport.hpp
	)
	list(APPEND INTERNALS
		src/internal/Window/Platforms/Mac/MetalViewportHelper.h
		src/internal/Window/Platforms/Mac/MetalViewportHelper.mm
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Mac/MetalViewport.cpp
	)
elseif(WIN32)
	list(APPEND HEADERS
		src/include/Window/Platforms/Windows/WindowsViewport.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Windows/WindowsViewport.cpp
	)
elseif(UNIX AND NOT APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Linux/LinuxViewport.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Linux/LinuxViewport.cpp
	)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_library(CE_Window SHARED
	${HEADERS}
	${INTERFACES}
	${INTERNALS}
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
	PUBLIC CE_MetalCpp
)

target_include_directories(CE_Window PUBLIC
	src/include
	src/internal
	../Core/src/include
	../Define/src/include
	../Events/src/include
	../Tools/src/include

	../Vendor/Apple/Metal/MetalCpp/src/include
)
