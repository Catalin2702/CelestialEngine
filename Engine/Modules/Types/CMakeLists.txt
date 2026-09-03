#
# Module: CelestialEngine/Engine/Modules/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-09-03
#

set(HEADERS
	# Build
	src/include/Types/Build/Build.hpp

	# Compare
	src/include/Types/Compare/Compare.hpp

	# FileSystem
	src/include/Types/FileSystem/File.hpp

	# KeyCode
	src/include/Types/KeyCode/KeyboardKeyCode.hpp
	src/include/Types/KeyCode/MouseButtonCode.hpp

	# Render
	src/include/Types/Render/AttachmentAction.hpp
	src/include/Types/Render/BlendFactor.hpp
	src/include/Types/Render/CullMode.hpp
	src/include/Types/Render/PixelFormat.hpp
	src/include/Types/Render/PrimitiveTopology.hpp
	src/include/Types/Render/Render.hpp
	src/include/Types/Render/Shader.hpp
	src/include/Types/Render/Platforms/Common/OpenGl/OpenGl.hpp

	# Var
	src/include/Types/Var/Vars.hpp

	# Window
	src/include/Types/Window/WindowProps.hpp
	src/include/Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp


	# Main
	src/include/Types/Types.hpp
)

set(SOURCES
	# Build
	src/source/Types/Build/Build.cpp

	# Compare
	src/source/Types/Compare/Compare.cpp

	# KeyCode
	src/source/Types/KeyCode/KeyboardKeyCode.cpp
	src/source/Types/KeyCode/MouseButtonCode.cpp

	# Render
	src/source/Types/Render/AttachmentAction.cpp
	src/source/Types/Render/BlendFactor.cpp
	src/source/Types/Render/CullMode.cpp
	src/source/Types/Render/PixelFormat.cpp
	src/source/Types/Render/PrimitiveTopology.cpp
	src/source/Types/Render/Render.cpp
	src/source/Types/Render/Shader.cpp
	src/source/Types/Render/Platforms/Common/OpenGl/OpenGl.cpp

	# Window
	src/source/Types/Window/WindowProps.cpp
	src/source/Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.cpp
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
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Types)

# Modules
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
	CE_Vendor_Glad
	CE_Vendor_Glfw
	CE_Vendor_ImGui
	CE_Vendor_MetalCpp
)

target_include_directories(CE_Types PUBLIC
	src/include
)

target_compile_definitions(CE_Types PRIVATE
	CE_TYPES_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Types)
