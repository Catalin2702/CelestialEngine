set(HEADERS
	src/include/Window/Platforms/Universal/OpenGLViewport.hpp
)

set(INTERFACES
	src/interface/Window/InterfaceWindow.hpp
)

set(SOURCES
	src/source/Window/Platforms/Universal/OpenGLViewport.cpp
)

if(APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Mac/MetalViewport.hpp
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
	PRIVATE CE_Types

	PUBLIC glad::glad
	PUBLIC glfw
)

# Link spdlog only for Debug and Release builds, not for Dist
if(NOT CMAKE_BUILD_TYPE STREQUAL "Dist")
	target_link_libraries(CE_Window PUBLIC spdlog::spdlog)
endif()

if(APPLE)
	target_link_libraries(CE_Window
		PUBLIC CE_MetalBridge
		PUBLIC CE_MetalCpp
	)
endif()

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
)

if(APPLE)
	target_include_directories(CE_Window PUBLIC
		../Apple/MetalBridge/src/include
		../Vendor/Apple/Metal/MetalCpp/src/include
	)
endif()
