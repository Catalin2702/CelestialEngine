#
# Module: CelestialEngine/Engine/Modules/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

set(HEADERS
	# Build
	src/include/Types/Build/Build.hpp

	# KeyCode
	src/include/Types/KeyCode/KeyboardKeyCode.hpp
	src/include/Types/KeyCode/MouseButtonCode.hpp

	# Render
	src/include/Types/Render/Render.hpp
	src/include/Types/Render/Shader.hpp
	src/include/Types/Render/Platforms/Common/OpenGl/OpenGl.hpp

	# Window
	src/include/Types/Window/WindowProps.hpp
	src/include/Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp

	# FileSystem
	src/include/Types/FileSystem/File.hpp

	# Main
	src/include/Types/Types.hpp
)

set(SOURCES
	# Build
	src/source/Types/Build/Build.cpp

	# KeyCode
	src/source/Types/KeyCode/KeyboardKeyCode.cpp
	src/source/Types/KeyCode/MouseButtonCode.cpp

	# Render
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and (Linux/macOS) lets it find
# its own sibling module libraries wherever it ends up (DLL/ or CE_App.app/Contents/Frameworks)
ce_set_output_subdirectory(CE_Types DLL)
ce_configure_module_library_rpath(CE_Types)

# Modules
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
	CE_Vendor_Glad
	CE_Vendor_Glfw
	CE_Vendor_ImGui
)

target_include_directories(CE_Types PUBLIC
	src/include
)

target_compile_definitions(CE_Types PRIVATE
	CE_TYPES_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Types)
