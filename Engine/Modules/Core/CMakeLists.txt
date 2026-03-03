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

set_target_properties(CE_Core PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Core PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Tools
	CE_Types
	CE_Window
)

# Platform-specific
target_link_libraries(CE_Core PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		MetalCpp
	>
)

# Third-party
target_link_libraries(CE_Core PUBLIC
	$<$<NOT:$<CONFIG:Dist>>:
		spdlog::spdlog
	>
)

target_include_directories(CE_Core PUBLIC
	src/include
)
