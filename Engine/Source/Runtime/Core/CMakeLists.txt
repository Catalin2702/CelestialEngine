#
# Module: CelestialEngine/Engine/Source/Runtime/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

# Backend-specific files are grouped by what they belong to, and each group is compiled only when it is selected:
#   $<BOOL:${CE_RENDER_OPENGL}> / $<BOOL:${CE_RENDER_METAL}>  - graphics backends (CE_RENDER_BACKENDS)
#   $<BOOL:${CE_WINDOW_GLFW}>  / $<BOOL:${CE_WINDOW_COCOA}>   - window backends, derived from the graphics ones
#   $<PLATFORM_ID:Darwin>                                      - the OS itself, whatever the backends (the menu bar)
# See Engine/CMake/CERenderBackends.cmake.

# region Headers
set(CORE_HEADERS
	src/include/Core/Core.hpp
	src/include/Core/Version.hpp
)

set(APPLICATION_HEADERS
	src/include/Core/Application/Application.hpp
	src/include/Core/Application/ApplicationSpecification.hpp
)

set(CMD_HEADERS
	src/include/Core/CMD/CommandLine.hpp
	src/include/Core/CMD/Window.hpp
)

set(CONFIG_HEADERS
	src/include/Core/Config/Config.hpp
)

set(HUB_HEADERS
	src/include/Core/Hub/Hub.hpp

	# Events
	$<$<BOOL:${CE_WINDOW_GLFW}>:
	src/include/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp
	src/include/Core/Hub/Events/Platforms/Common/Glfw/GlfwKeyCode.hpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
	src/include/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp
	src/include/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaKeyCode.hpp
	>
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
	src/include/Core/Layers/ImGui/ImGuiHelpers.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalBridge.h
		src/include/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp
	>


	# Scene
	src/include/Core/Layers/Scene/SceneLayer.hpp
)

set(RENDER_HEADERS
	src/include/Core/Render/Render.hpp


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


	# Renderer
	src/include/Core/Render/Renderer/DrawCommand.hpp
	src/include/Core/Render/Renderer/ForwardRenderer.hpp
	src/include/Core/Render/Renderer/Renderer.hpp
	src/include/Core/Render/Renderer/RenderStats.hpp


	# Texture
	src/include/Core/Render/Texture/Image.hpp
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

# endregion

# region Interfaces

set(APPLICATION_INTERFACES
	src/interface/Core/Application/I_ApplicationClient.hpp
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

set(LAYERS_INTERFACES
	src/interface/Core/Layers/I_Layer.hpp
	src/interface/Core/Layers/ImGui/I_ImGuiLayer.hpp
)

set(RENDER_INTERFACES
	# Camera
	src/interface/Core/Render/Camera/Controller/I_CameraController.hpp
	src/interface/Core/Render/Camera/Projection/I_CameraProjection.hpp


	# Renderer
	src/interface/Core/Render/Renderer/I_Renderer.hpp
)

set(RUNLOOP_INTERFACES
	src/interface/Core/RunLoop/I_RunLoop.hpp
)

set(WINDOW_INTERFACES
	src/interface/Core/Window/I_Platform.hpp
	src/interface/Core/Window/I_Window.hpp
)

# endregion

# region Sources

set(CORE_SOURCES
	src/source/Core/Version.cpp
)

set(APPLICATION_SOURCES
	src/source/Core/Application/Application.cpp
)

set(CMD_SOURCES
	src/source/Core/CMD/CommandLine.cpp
	src/source/Core/CMD/Window.cpp
)

set(CONFIG_SOURCES
	src/source/Core/Config/Config.cpp
)

set(HUB_SOURCES
	#Events
	src/source/Core/Hub/Events/I_EventHubDispatcher.cpp
	src/source/Core/Hub/Events/EventHubDispatcherFactory.cpp

	$<$<BOOL:${CE_WINDOW_GLFW}>:
	src/source/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.cpp
	src/source/Core/Hub/Events/Platforms/Common/Glfw/GlfwKeyCode.cpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
	src/source/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.cpp
	src/source/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaKeyCode.cpp
	>
)

set(INPUT_SOURCES
	src/source/Core/Input/Input.cpp
	src/source/Core/Input/InputState.cpp
)

set(LAYERS_SOURCES
	src/source/Core/Layers/LayerStack.cpp


	# ImGui
	src/source/Core/Layers/ImGui/ImGuiHelpers.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalBridge.mm
		src/source/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.cpp
	>


	# Scene
	src/source/Core/Layers/Scene/SceneLayer.cpp
)

set(RENDER_SOURCES
	# Camera
	src/source/Core/Render/Camera/Camera.cpp
	src/source/Core/Render/Camera/CameraRig.cpp
	src/source/Core/Render/Camera/Controller/CameraControllerFactory.cpp
	src/source/Core/Render/Camera/Controller/FreeCameraController.cpp
	src/source/Core/Render/Camera/Controller/MouseLook.cpp
	src/source/Core/Render/Camera/Controller/StaticCameraController.cpp
	src/source/Core/Render/Camera/Projection/CameraProjectionFactory.cpp
	src/source/Core/Render/Camera/Projection/OrthographicCameraProjection.cpp


	# Renderer
	src/source/Core/Render/Renderer/RendererFactory.cpp
	src/source/Core/Render/Renderer/ForwardRenderer.cpp


	# Texture
	src/source/Core/Render/Texture/Image.cpp
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

# endregion

set(HEADERS
	${CORE_HEADERS}
	${APPLICATION_HEADERS}
	${CMD_HEADERS}
	${CONFIG_HEADERS}
	${HUB_HEADERS}
	${INPUT_HEADERS}
	${LAYERS_HEADERS}
	${RENDER_HEADERS}
	${RUNLOOP_HEADERS}
	${WINDOW_HEADERS}
)

set(INTERFACES
	${APPLICATION_INTERFACES}
	${HUB_INTERFACES}
	${LAYERS_INTERFACES}
	${RENDER_INTERFACES}
	${RUNLOOP_INTERFACES}
	${WINDOW_INTERFACES}
)

set(SOURCES
	${CORE_SOURCES}
	${APPLICATION_SOURCES}
	${CMD_SOURCES}
	${CONFIG_SOURCES}
	${HUB_SOURCES}
	${INPUT_SOURCES}
	${LAYERS_SOURCES}
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

# Keeps Out/<config>/ down to just executables: the library goes to its DLL/ subfolder on Windows and Linux, and to
# <build>/Modules/<config>/ on macOS (see ce_set_module_library_output)
ce_set_module_library_output(CE_Core)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Core)

# Modules
target_link_libraries(CE_Core PUBLIC
	CE_Config
	CE_Define
	CE_Events
	CE_Native
	CE_RHI
	CE_Tools
	CE_Types
	CE_Utility
)

target_link_libraries(CE_Core PRIVATE
	CE_Internal
	# No public header of Core includes IO (main() and the ImGui layers use it from .cpp files); clients that want the
	# file system get it from CE_Engine.
	CE_IO
	# Only Image.cpp includes stb_image.h, and no public header does: PRIVATE keeps it off the include path of every
	# target that links CE_Core.
	CE_Vendor_Stb
)

# Vendor libraries
# ImGui is public: Core/Layers/ImGui/ImGuiHelpers.hpp includes imgui.h. spdlog is not linked here: Core never names it,
# and the logging macros bring it through CE_Tools.
target_link_libraries(CE_Core PUBLIC
	CE_Vendor_ImGui
)

# Each backend's third parties exist only when the backend is compiled in (CE_RENDER_BACKENDS): GLFW and its ImGui
# backend with a GLFW window, glad with OpenGL, metal-cpp with Metal. Core links them itself - Types, which used to
# carry them for its conversion tables, knows no backend any more.
# GLFW, its ImGui backend and glad are PRIVATE: only Core's .cpp files include them (GlfwWindow, the GLFW event hub,
# ImGuiOpenGlLayer), never a public header. They are static libraries, so linking them PUBLIC copied GLFW into every
# executable that linked CE_Core as well - two copies of GLFW's global state in one process.
if (CE_WINDOW_GLFW)
	target_link_libraries(CE_Core PRIVATE
		CE_Vendor_Glfw
		CE_Vendor_ImGui_Glfw
	)
endif ()

if (CE_RENDER_OPENGL)
	target_link_libraries(CE_Core PRIVATE
		CE_Vendor_Glad
	)
endif ()

if (CE_RENDER_METAL)
	target_link_libraries(CE_Core PUBLIC
		CE_Vendor_MetalCpp
	)
endif ()

target_include_directories(CE_Core PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Core PRIVATE
	CE_CORE_EXPORTS
	# GetEngineVersion (Core/Version.hpp). Core has no project() of its own, so this is Engine/CMakeLists.txt's version.
	CE_ENGINE_VERSION="${PROJECT_VERSION}"
)
