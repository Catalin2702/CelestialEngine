#
# Module: CelestialEngine/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-09-02
#

set(APPLICATION_HEADERS
	src/include/Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp
		src/include/Core/Application/Platforms/Mac/Cocoa/MacMenuBar.hpp
	>
)

set(INPUT_HEADERS
	src/include/Core/Input/Input.hpp
	src/include/Core/Input/InputState.hpp
)

set(LAYERS_HEADERS
	src/include/Core/Layers/LayerStack.hpp
	src/include/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp
	>
)

set(HUB_HEADERS
	#Events
	src/include/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp
	>
)

set(RENDER_HEADERS
	# Buffer
	src/include/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp
	src/include/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.hpp

	# Command
	src/include/Core/Render/Command/RenderPassDescriptor.hpp
	src/include/Core/Render/Command/Viewport.hpp
	src/include/Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp

	# Context
	src/include/Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp
	>

	# Device
	src/include/Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp

	# Pipeline
	src/include/Core/Render/Pipeline/PipelineDescriptor.hpp
	src/include/Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp

	# Shader
	src/include/Core/Render/Shader/ShaderModuleDescriptor.hpp
	src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp
	src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp
	src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp
	src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp
		src/include/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp
	>

	# Swapchain
	src/include/Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp
)

set(RUNLOOP_HEADERS
	src/include/Core/RunLoop/PacedRunLoop.hpp
)

set(WINDOW_HEADERS
	src/include/Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp
	src/include/Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp
		src/include/Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp
	>
)

set(APPLICATION_INTERFACES
	src/interface/Core/Application/I_Application.hpp
)

set(LAYERS_INTERFACES
	src/interface/Core/Layers/I_Layer.hpp
	src/interface/Core/Layers/ImGui/I_ImGuiLayer.hpp
)

set(HUB_INTERFACES
	# Events
	src/interface/Core/Hub/Events/I_EventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_ApplicationEventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_KeyboardEventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_MouseEventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_RenderContextEventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_WindowEventHubDispatcher.hpp
)


set(RENDER_INTERFACES
	# Buffer
	src/interface/Core/Render/Buffer/I_Buffer.hpp
	src/interface/Core/Render/Buffer/I_VertexArray.hpp

	# Command
	src/interface/Core/Render/Command/I_CommandEncoder.hpp

	# Context
	src/interface/Core/Render/Context/I_Context.hpp

	# Device
	src/interface/Core/Render/Device/I_GraphicDevice.hpp

	# Pipeline
	src/interface/Core/Render/Pipeline/I_PipelineState.hpp

	# Shader
	src/interface/Core/Render/Shader/I_Shader.hpp
	src/interface/Core/Render/Shader/I_ShaderModule.hpp
	src/interface/Core/Render/Shader/I_ShaderProgram.hpp

	# Surface
	src/interface/Core/Render/Surface/I_OpenGlSurface.hpp

	# Swapchain
	src/interface/Core/Render/Swapchain/I_Swapchain.hpp
)

set(RUNLOOP_INTERFACES
	src/interface/Core/RunLoop/I_RunLoop.hpp
)

set(WINDOW_INTERFACES
	src/interface/Core/Window/I_Platform.hpp
	src/interface/Core/Window/I_Window.hpp
)

set(APPLICATION_SOURCES
	src/source/Core/Application/ApplicationImpl.cpp

	src/source/Core/Application/Platforms/Common/Glfw/GlfwApplication.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Application/Platforms/Mac/Cocoa/CocoaApplication.cpp
	>
)

set(INPUT_SOURCES
	src/source/Core/Input/Input.cpp
	src/source/Core/Input/InputState.cpp
)

set(LAYERS_SOURCES
	src/source/Core/Layers/LayerStack.cpp
	src/source/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.cpp
	>
)

set(HUB_SOURCES
	#Events
	src/source/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.cpp
	>
)

set(RENDER_SOURCES
	# Buffer
	src/source/Core/Render/Buffer/Buffer.cpp
	src/source/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.cpp
	src/source/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.cpp

	# Command
	src/source/Core/Render/Command/Viewport.cpp
	src/source/Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.cpp

	# Context
	src/source/Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Render/Context/Platforms/Mac/Metal/MetalContext.cpp
	>

	# Device
	src/source/Core/Render/Device/GraphicDeviceFactory.cpp
	src/source/Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.cpp

	# Pipeline
	src/source/Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.cpp

	# Shader
	src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.cpp
	src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.cpp
	src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.cpp
	src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.cpp
		src/source/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.cpp
	>

	# Swapchain
	src/source/Core/Render/Swapchain/SwapchainFactory.cpp
	src/source/Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.cpp
)

set(RUNLOOP_SOURCES
	src/source/Core/RunLoop/PacedRunLoop.cpp
)

set(WINDOW_SOURCES
	src/source/Core/Window/PlatformFactory.cpp
	src/source/Core/Window/WindowFactory.cpp

	src/source/Core/Window/Platforms/Common/Glfw/GlfwPlatform.cpp
	src/source/Core/Window/Platforms/Common/Glfw/GlfwWindow.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Window/Platforms/Mac/Cocoa/CocoaWindow.cpp
	>
)

set(HEADERS
	${APPLICATION_HEADERS}
	${INPUT_HEADERS}
	${LAYERS_HEADERS}
	${HUB_HEADERS}
	${RENDER_HEADERS}
	${RUNLOOP_HEADERS}
	${WINDOW_HEADERS}
)

set(INTERFACES
	${APPLICATION_INTERFACES}
	${LAYERS_INTERFACES}
	${HUB_INTERFACES}
	${RENDER_INTERFACES}
	${RUNLOOP_INTERFACES}
	${WINDOW_INTERFACES}
)

set(SOURCES
	${APPLICATION_SOURCES}
	${INPUT_SOURCES}
	${LAYERS_SOURCES}
	${HUB_SOURCES}
	${RENDER_SOURCES}
	${RUNLOOP_SOURCES}
	${WINDOW_SOURCES}
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Core)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Core)

# Modules
target_link_libraries(CE_Core PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Native
	CE_Tools
	CE_Types
	CE_Utility
)

# Vendor libraries
target_link_libraries(CE_Core PUBLIC
	CE_Vendor_Glfw
	CE_Vendor_ImGui
	CE_Vendor_SpdLog
)

target_include_directories(CE_Core PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Core PRIVATE
	CE_CORE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Core)
