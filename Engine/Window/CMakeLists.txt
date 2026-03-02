set(HEADERS
	src/include/Window/Platforms/Universal/OpenGlWindow.hpp
)

set(INTERFACES
	src/interface/Window/I_Window.hpp
)

set(SOURCES
	src/source/Window/Platforms/Universal/OpenGlWindow.cpp
)

if(APPLE)
	list(APPEND HEADERS
		src/include/Window/Platforms/Mac/MetalWindow.hpp
	)
	list(APPEND SOURCES
		src/source/Window/Platforms/Mac/MetalWindow.cpp
		src/source/Window/Platforms/Universal/OpenGlWindow.cpp
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

target_link_libraries(CE_Window PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Tools
	CE_Types

	glad::glad
	glfw
	$<$<NOT:$<CONFIG:Dist>>:spdlog::spdlog>
)

if(APPLE)
	target_link_libraries(CE_Window PUBLIC
		CE_Native

		MetalCpp
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
		../Native/Apple/Metal/MetalBridge/src/include
		../Vendor/Apple/Metal/MetalCpp/src/include
	)
endif()
