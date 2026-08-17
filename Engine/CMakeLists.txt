#
# Module: CelestialEngine/Engine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

cmake_minimum_required(VERSION 4.1)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_Engine LANGUAGES CXX VERSION 0.1.3)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

# Vendor libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Glfw)
add_subdirectory(Vendor/Glm)
add_subdirectory(Vendor/ImGui)
add_subdirectory(Vendor/SpdLog)

# Apple vendor libraries
if (APPLE)
	add_subdirectory(Vendor/MetalCpp)
endif ()

# Module libraries
add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Types)
add_subdirectory(Modules/Utility)

# Shaders
add_subdirectory(Shaders)

add_library(CE_Engine SHARED
	${HEADERS}
)

set_target_properties(CE_Engine PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables - skipped on macOS, where doing
# this would force the DLL/ folder to exist as soon as CE_App alone is built (see
# ce_set_output_subdirectory's doc comment)
if (NOT APPLE)
	ce_set_output_subdirectory(CE_Engine DLL)
endif ()

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Engine)

# Modules
target_link_libraries(CE_Engine PUBLIC
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_Native
	CE_Tools
	CE_Types
)

# Platform specific modules
target_link_libraries(CE_Engine PUBLIC
	$<$<PLATFORM_ID:Darwin>:
		CE_Vendor_MetalCpp
	>
)

# Shaders
target_link_libraries(CE_Engine PUBLIC
	CE_Shaders
)

target_include_directories(CE_Engine PUBLIC
	src/include
	src/internal
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Engine)
