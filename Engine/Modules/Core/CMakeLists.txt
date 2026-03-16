#
# Module: CelestialEngine/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-16
#

set(HEADERS
	src/include/Core/Application.hpp
	src/include/Layers/LayerStack.hpp
	src/include/Layers/ImGui/Platforms/Common/ImGuiOpenGlGlfwLayer.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Layers/ImGui/Platforms/Mac/ImGuiMetalGlfwLayer.hpp
		src/include/Input/Platforms/Mac/MetalInput.hpp
	>
)

set(INTERFACES
	src/interface/Layers/I_Layer.hpp
	src/interface/Layers/Render/I_RenderLayer.hpp
	src/interface/Layers/ImGui/I_ImGuiLayer.hpp
	src/interface/Input/I_Input.hpp
)

set(SOURCES
	src/source/Core/Application.cpp
	src/source/Layers/LayerStack.cpp
	src/source/Layers/ImGui/Platforms/Common/ImGuiOpenGlGlfwLayer.cpp


	$<$<PLATFORM_ID:Darwin>:
		src/source/Layers/ImGui/Platforms/Mac/ImGuiMetalGlfwLayer.cpp
		src/source/Input/Platforms/Mac/MetalInput.cpp
	>
)

add_library(CE_Core SHARED
	${HEADERS}
	${INTERFACES}
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
	CE_Glfw
	CE_ImGui
	CE_SpdLog
	CE_Tools
	CE_Types
	CE_Window
)

# Platform-specific
target_link_libraries(CE_Core PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		CE_MetalCpp
	>
)

target_include_directories(CE_Core PUBLIC
	src/include
	src/interface
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Core)

