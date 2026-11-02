#
# Module: CelestialEngine/Engine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

# Normally already included by the top-level CMakeLists.txt (it has to be, for the applications to see its variables);
# included here too for a project that only adds this directory. The include guard makes the second inclusion free.
include("${CMAKE_CURRENT_LIST_DIR}/cmake/CelestialEngineMacros.cmake")

project(CE_Engine LANGUAGES CXX VERSION 0.1.3)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

set(SOURCES
	src/source/CelestialEngine.cpp
)

# Vendor libraries. The backend-specific ones are added only when their backend is compiled in (CE_RENDER_BACKENDS), so
# a build without OpenGL does not even look for glad, and one without a GLFW window does not look for GLFW.
if (CE_RENDER_OPENGL)
	add_subdirectory(Vendor/Glad)
endif ()

if (CE_WINDOW_GLFW)
	add_subdirectory(Vendor/Glfw)
endif ()

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
	${SOURCES}
)

set_target_properties(CE_Engine PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Engine)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Engine)

# Engine-internal: unlocks the CE_CORE_* logging macros (see CE_Internal in CelestialEngineMacros.cmake)
target_link_libraries(CE_Engine PRIVATE
	CE_Internal
)

target_compile_definitions(CE_Engine PRIVATE
	CE_ENGINE_EXPORTS
	CE_ENGINE_VERSION="${PROJECT_VERSION}"
)

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

# The engine's tests live with it (Engine/Tests) and come last, once every module target they link exists. Building them
# is what makes CMake create Binaries/<toolchain>/<config>/Tests/ - at GENERATE time, so the folder shows up (empty) even
# in a build that only ever compiles an application. Turn them off to configure without them:
#   cmake --preset config-preset-clang -DCE_BUILD_TESTS=OFF
# A cache variable read at configure time, so flipping it needs a re-configure.
option(CE_BUILD_TESTS "Build the engine's test executables" ON)

if (CE_BUILD_TESTS)
	enable_testing()
	add_subdirectory(Tests)
endif ()
