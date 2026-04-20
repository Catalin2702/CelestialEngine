#
# Module: CelestialEngine/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-04-20
#

set(HEADERS
	src/include/Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp

	src/include/Core/Input/Platforms/Common/Glfw/GlfwInput.hpp

	src/include/Core/Layers/LayerStack.hpp
	src/include/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp
		src/include/Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp

		src/include/Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp

		src/include/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp
	>
)

set(INTERFACES
	src/interface/Core/Application/I_Application.hpp

	src/interface/Core/Input/I_Input.hpp

	src/interface/Core/Layers/I_Layer.hpp
	src/interface/Core/Layers/ImGui/I_ImGuiLayer.hpp
	src/interface/Core/Layers/Render/I_RenderLayer.hpp
)

set(SOURCES
	src/source/Core/Application/ApplicationImpl.cpp
	src/source/Core/Application/Platforms/Common/Glfw/GlfwApplication.cpp

	src/source/Core/Input/InputImpl.cpp
	src/source/Core/Input/Platforms/Common/Glfw/GlfwInput.cpp

	src/source/Core/Layers/LayerStack.cpp
	src/source/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Application/Platforms/Mac/Cocoa/CocoaApplication.cpp
		src/source/Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.cpp

		src/source/Core/Input/Platforms/Mac/Cocoa/CocoaInput.cpp

		src/source/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.cpp
	>
)

add_library(CE_Core SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Core PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Core PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Glfw
	CE_ImGui
	CE_Render
	CE_SpdLog
	CE_Tools
	CE_Types
	CE_Window
)

# Platform-specific
target_link_libraries(CE_Core PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		CE_AppleUtility
		CE_CoreAnimation
		CE_MetalCpp
	>
)

target_compile_options(CE_Core PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		-ObjC++ -fno-objc-arc
	>
)

target_include_directories(CE_Core PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Core PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Core)
