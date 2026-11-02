#
# Module: CelestialEngine/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

# Backend-specific files are grouped by what they belong to, and each group is compiled only when it is selected:
#   $<BOOL:${CE_RENDER_OPENGL}> / $<BOOL:${CE_RENDER_METAL}>  - graphics backends (CE_RENDER_BACKENDS)
#   $<BOOL:${CE_WINDOW_GLFW}>  / $<BOOL:${CE_WINDOW_COCOA}>   - window backends, derived from the graphics ones
#   $<PLATFORM_ID:Darwin>                                      - the OS itself, whatever the backends (the menu bar)
# See config/cmake/CERenderBackends.cmake.

set(CORE_HEADERS
	src/include/Core/Core.hpp
)

set(APPLICATION_HEADERS
	src/include/Core/Application/Application.hpp
)

set(INPUT_HEADERS
	src/include/Core/Input/Input.hpp
	src/include/Core/Input/InputState.hpp
	src/include/Core/Input/KeyShortcut.hpp
)

set(LAYERS_HEADERS
	src/include/Core/Layers/Layers.hpp
	src/include/Core/Layers/LayerStack.hpp


	# ImGui
	src/include/Core/Layers/ImGui/ImGui.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp
	>


	# Scene
	src/include/Core/Layers/Scene/SceneLayer.hpp
)

set(HUB_HEADERS
	src/include/Core/Hub/Hub.hpp

	# Events
	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/include/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/include/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp
	>
)

set(RENDER_HEADERS
	src/include/Core/Render/Render.hpp

	# Buffer
	src/include/Core/Render/Buffer/Buffer.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp
		src/include/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp
	>


	# Camera
	src/include/Core/Render/Camera/Camera.hpp
	src/include/Core/Render/Camera/CameraDescriptor.hpp
	src/include/Core/Render/Camera/CameraRig.hpp
	src/include/Core/Render/Camera/Controller/CameraControllerDescriptor.hpp
	src/include/Core/Render/Camera/Controller/Controller.hpp
	src/include/Core/Render/Camera/Controller/FreeCameraController.hpp
	src/include/Core/Render/Camera/Controller/MouseLook.hpp
	src/include/Core/Render/Camera/Controller/StaticCameraController.hpp
	src/include/Core/Render/Camera/Projection/CameraProjectionDescriptor.hpp
	src/include/Core/Render/Camera/Projection/OrthographicCameraProjection.hpp
	src/include/Core/Render/Camera/Projection/Projection.hpp


	# Command
	src/include/Core/Render/Command/Command.hpp
	src/include/Core/Render/Command/RenderPassDescriptor.hpp
	src/include/Core/Render/Command/Viewport.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp
	>


	# Device
	src/include/Core/Render/Device/Device.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp
	>

	# Pipeline
	src/include/Core/Render/Pipeline/Pipeline.hpp
	src/include/Core/Render/Pipeline/PipelineDescriptor.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp
	>


	# Renderer
	src/include/Core/Render/Renderer/DrawCommand.hpp
	src/include/Core/Render/Renderer/ForwardRenderer.hpp
	src/include/Core/Render/Renderer/Renderer.hpp
	src/include/Core/Render/Renderer/RenderStats.hpp


	# Shader
	src/include/Core/Render/Shader/Shader.hpp
	src/include/Core/Render/Shader/ShaderModuleDescriptor.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp
		src/include/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp
		src/include/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp
	>


	# Surface
	src/include/Core/Render/Surface/Surface.hpp


	# Swapchain
	src/include/Core/Render/Swapchain/Swapchain.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp
	>


	# Texture
	src/include/Core/Render/Texture/Texture.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Render/Texture/Platforms/Common/OpenGl/OpenGlTexture.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Render/Texture/Platforms/Mac/Metal/MetalTexture.hpp
	>
)

set(RUNLOOP_HEADERS
	src/include/Core/RunLoop/PacedRunLoop.hpp
	src/include/Core/RunLoop/RunLoop.hpp
)

set(WINDOW_HEADERS
	src/include/Core/Window/Window.hpp
	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/include/Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp
		src/include/Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp
		src/include/Core/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp
	>

	$<$<PLATFORM_ID:Darwin>:
		src/include/Core/Window/Platforms/Mac/MacMenuBar.hpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/include/Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp
		src/include/Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp
	>
)

set(APPLICATION_INTERFACES
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
	src/interface/Core/Hub/Events/I_RenderEventHubDispatcher.hpp
	src/interface/Core/Hub/Events/I_WindowEventHubDispatcher.hpp
)


set(RENDER_INTERFACES
	# Buffer
	src/interface/Core/Render/Buffer/I_Buffer.hpp
	src/interface/Core/Render/Buffer/I_VertexArray.hpp

	# Camera
	src/interface/Core/Render/Camera/Controller/I_CameraController.hpp
	src/interface/Core/Render/Camera/Projection/I_CameraProjection.hpp

	# Command
	src/interface/Core/Render/Command/I_CommandEncoder.hpp


	# Device
	src/interface/Core/Render/Device/I_GraphicDevice.hpp

	# Pipeline
	src/interface/Core/Render/Pipeline/I_PipelineState.hpp

	# Renderer
	src/interface/Core/Render/Renderer/I_Renderer.hpp

	# Shader
	src/interface/Core/Render/Shader/I_ShaderModule.hpp

	# Surface
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/interface/Core/Render/Surface/Common/OpenGl/I_OpenGlSurface.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/interface/Core/Render/Surface/Mac/Metal/I_MetalSurface.hpp
	>

	# Swapchain
	src/interface/Core/Render/Swapchain/I_Swapchain.hpp

	# Texture
	src/interface/Core/Render/Texture/I_Texture.hpp
)

set(RUNLOOP_INTERFACES
	src/interface/Core/RunLoop/I_RunLoop.hpp
)

set(WINDOW_INTERFACES
	src/interface/Core/Window/I_Platform.hpp
	src/interface/Core/Window/I_Window.hpp
)

set(APPLICATION_SOURCES
	src/source/Core/Application/Application.cpp
)

set(INPUT_SOURCES
	src/source/Core/Input/Input.cpp
	src/source/Core/Input/InputState.cpp
)

set(LAYERS_SOURCES
	src/source/Core/Layers/LayerStack.cpp


	# ImGui
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.cpp
	>


	# Scene
	src/source/Core/Layers/Scene/SceneLayer.cpp
)

set(HUB_SOURCES
	#Events
	src/source/Core/Hub/Events/I_EventHubDispatcher.cpp
	src/source/Core/Hub/Events/EventHubDispatcherFactory.cpp

	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/source/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.cpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/source/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.cpp
	>
)

set(RENDER_SOURCES
	# Buffer
	src/source/Core/Render/Buffer/Buffer.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.cpp
		src/source/Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.cpp
	>


	# Camera
	src/source/Core/Render/Camera/Camera.cpp
	src/source/Core/Render/Camera/CameraRig.cpp
	src/source/Core/Render/Camera/Controller/CameraControllerFactory.cpp
	src/source/Core/Render/Camera/Controller/FreeCameraController.cpp
	src/source/Core/Render/Camera/Controller/MouseLook.cpp
	src/source/Core/Render/Camera/Controller/StaticCameraController.cpp
	src/source/Core/Render/Camera/Projection/CameraProjectionFactory.cpp
	src/source/Core/Render/Camera/Projection/OrthographicCameraProjection.cpp


	# Command
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.cpp
	>


	# Device
	src/source/Core/Render/Device/GraphicDeviceFactory.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.cpp
	>


	# Pipeline
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.cpp
	>


	# Renderer
	src/source/Core/Render/Renderer/RendererFactory.cpp
	src/source/Core/Render/Renderer/ForwardRenderer.cpp


	# Shader
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.cpp
		src/source/Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.cpp
		src/source/Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.cpp
	>


	# Swapchain
	src/source/Core/Render/Swapchain/SwapchainFactory.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.cpp
	>


	# Texture
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Render/Texture/Platforms/Common/OpenGl/OpenGlTexture.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Render/Texture/Platforms/Mac/Metal/MetalTexture.cpp
	>
)

set(RUNLOOP_SOURCES
	src/source/Core/RunLoop/RunLoopFactory.cpp
	src/source/Core/RunLoop/PacedRunLoop.cpp
)

set(WINDOW_SOURCES
	src/source/Core/Window/PlatformFactory.cpp
	src/source/Core/Window/WindowFactory.cpp

	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/source/Core/Window/Platforms/Common/Glfw/GlfwPlatform.cpp
		src/source/Core/Window/Platforms/Common/Glfw/GlfwWindow.cpp
		src/source/Core/Window/Platforms/Common/Glfw/GlfwWindowDestructor.cpp
	>

	$<$<PLATFORM_ID:Darwin>:
		src/source/Core/Window/Platforms/Mac/MacMenuBar.cpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/source/Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.cpp
		src/source/Core/Window/Platforms/Mac/Cocoa/CocoaWindow.cpp
	>
)

set(HEADERS
	${CORE_HEADERS}
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

target_link_libraries(CE_Core PRIVATE
	CE_Internal
)

# Vendor libraries
target_link_libraries(CE_Core PUBLIC
	CE_Vendor_ImGui
	CE_Vendor_SpdLog
)

# GLFW and its ImGui backend exist only when a GLFW window is compiled in (CE_WINDOW_GLFW, derived from
# CE_RENDER_BACKENDS); glad comes along with CE_Types whenever OpenGL is.
if (CE_WINDOW_GLFW)
	target_link_libraries(CE_Core PUBLIC
		CE_Vendor_Glfw
		CE_Vendor_ImGui_Glfw
	)
endif ()

target_include_directories(CE_Core PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Core PRIVATE
	CE_CORE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Core)
