#
# Module: CelestialEngine/Engine/Modules/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

set(HEADERS
	# Build
	src/include/Types/Build/Build.hpp

	# FileSystem
	src/include/Types/FileSystem/File.hpp


	# KeyCode
	src/include/Types/KeyCode/KeyboardKeyCode.hpp
	src/include/Types/KeyCode/KeyCode.hpp
	src/include/Types/KeyCode/MouseButtonCode.hpp

	# Per-window-backend code conversions, compiled only with their window (CE_WINDOW_*, from CE_RENDER_BACKENDS)
	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/include/Types/KeyCode/Platforms/Common/Glfw/Glfw.hpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/include/Types/KeyCode/Platforms/Mac/Cocoa/Cocoa.hpp
	>


	# Render
	src/include/Types/Render/AttachmentAction.hpp
	src/include/Types/Render/BlendFactor.hpp
	src/include/Types/Render/Camera.hpp
	src/include/Types/Render/Compare.hpp
	src/include/Types/Render/CullMode.hpp
	src/include/Types/Render/GraphicsApi.hpp
	src/include/Types/Render/PixelFormat.hpp
	src/include/Types/Render/PrimitiveTopology.hpp
	src/include/Types/Render/Render.hpp
	src/include/Types/Render/Shader.hpp
	src/include/Types/Render/ShaderConstants.hpp
	src/include/Types/Render/ShaderDataType.hpp
	src/include/Types/Render/TextureUsage.hpp

	# Per-backend conversion tables, compiled only with their backend (CE_RENDER_BACKENDS)
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/Types/Render/Platforms/Common/OpenGl/OpenGl.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/Types/Render/Platforms/Mac/Metal/Metal.hpp
	>


	# Var
	src/include/Types/Var/Glm.hpp
	src/include/Types/Var/Vars.hpp


	# Window
	src/include/Types/Window/CursorMode.hpp
	src/include/Types/Window/Window.hpp
	src/include/Types/Window/WindowProps.hpp


	# Main
	src/include/Types/Types.hpp
)

set(SOURCES
	# Build
	src/source/Types/Build/Build.cpp


	# KeyCode
	src/source/Types/KeyCode/KeyboardKeyCode.cpp
	src/source/Types/KeyCode/MouseButtonCode.cpp

	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/source/Types/KeyCode/Platforms/Common/Glfw/Glfw.cpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/source/Types/KeyCode/Platforms/Mac/Cocoa/Cocoa.cpp
	>


	# Render
	src/source/Types/Render/BlendFactor.cpp
	src/source/Types/Render/GraphicsApi.cpp
	src/source/Types/Render/PixelFormat.cpp
	src/source/Types/Render/PrimitiveTopology.cpp
	src/source/Types/Render/Shader.cpp

	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/Types/Render/Platforms/Common/OpenGl/OpenGl.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/Types/Render/Platforms/Mac/Metal/Metal.cpp
	>


	# Window
	src/source/Types/Window/WindowProps.cpp
)

add_library(CE_Types SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Types PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Types)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Types)

# Modules
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
	CE_Vendor_Glm
	CE_Vendor_ImGui
)

target_link_libraries(CE_Types PRIVATE
	CE_Internal
)

# Backend-specific third parties: glad for the OpenGL conversions, GLFW for the GLFW key and button codes, metal-cpp
# for the Metal conversions. Each one only exists when its backend is compiled in.
if (CE_RENDER_OPENGL)
	target_link_libraries(CE_Types PUBLIC
		CE_Vendor_Glad
	)
endif ()

if (CE_WINDOW_GLFW)
	target_link_libraries(CE_Types PUBLIC
		CE_Vendor_Glfw
	)
endif ()

if (CE_RENDER_METAL)
	target_link_libraries(CE_Types PUBLIC
		CE_Vendor_MetalCpp
	)
endif ()

target_include_directories(CE_Types PUBLIC
	src/include
)

target_compile_definitions(CE_Types PRIVATE
	CE_TYPES_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Types)
