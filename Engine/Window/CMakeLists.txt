set(HEADERS
	src/include/Window/Platforms/Universal/OpenGLViewport.hpp
)

set(INTERFACES
	src/interface/Window/InterfaceWindow.hpp
)

set(INTERNALS)

set(SOURCES
	src/source/Window/Platforms/Universal/OpenGLViewport.cpp
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
		src/source/Window/Platforms/Universal/OpenGLViewport.cpp
	)
elseif(WIN32)
	list(APPEND HEADERS
	)
	list(APPEND SOURCES
	)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

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
	PRIVATE CE_Types

	PUBLIC glfw
	PUBLIC glad::glad
	PUBLIC CE_MetalCpp
)

target_include_directories(CE_Window PUBLIC
	src/include
	src/interface
	src/internal
	src/template
	../Core/src/include
	../Define/src/include
	../Events/src/include
	../Tools/src/include
	../Types/src/include

	../Vendor/Apple/Metal/MetalCpp/src/include
)
