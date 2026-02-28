set(HEADERS
	src/include/Core/Application.hpp
	src/include/Layers/I_Layer.hpp
	src/include/Layers/LayerStack.hpp
	src/include/Layers/ImGuiOpenGlLayer.hpp
	src/include/Layers/ImGuiMetalLayer.hpp
	src/include/Layers/I_ImGuiLayer.hpp
)

set(SOURCES
	src/source/Core/Application.cpp
	src/source/Layers/I_Layer.cpp
	src/source/Layers/LayerStack.cpp
	src/source/Layers/ImGuiOpenGlLayer.cpp
	src/source/Layers/ImGuiMetalLayer.cpp
)

add_library(CE_Core SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Core PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Tools
	CE_Types
	CE_Window
)

# Link spdlog only for Debug and Release builds, not for Dist
if(NOT CMAKE_BUILD_TYPE STREQUAL "Dist")
	target_link_libraries(CE_Core PUBLIC spdlog::spdlog)
endif()

if(APPLE)
	target_link_libraries(CE_Core PUBLIC
		CE_MetalBridge
		CE_MetalCpp
	)
endif()

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

if(APPLE)
	target_include_directories(CE_Core PUBLIC
		../Apple/MetalBridge/src/include
		../Vendor/Apple/Metal/MetalCpp/src/include
	)
endif()
