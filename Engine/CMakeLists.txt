#
# Module: CelestialEngine/Engine
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# Normally already included by the top-level CMakeLists.txt (it has to be, for the applications to see its variables);
# included here too for a project that only adds this directory. The include guard makes the second inclusion free.
include("${CMAKE_CURRENT_LIST_DIR}/CMake/CelestialEngineMacros.cmake")

project(CE_Engine LANGUAGES CXX VERSION 0.4.0)

set(HEADERS
	Source/Launch/include/CelestialEngine.hpp
)

# Not a header and not part of CE_Engine: Source/Launch/source/EntryPoint.cpp (main) is compiled into each application by
# ce_configure_application, see CEApplication.cmake.

# Third-party libraries (CMake wrappers; ImGui and metal-cpp also hold their git submodule). The backend-specific ones are added only when their backend is compiled in (CE_RENDER_BACKENDS), so
# a build without OpenGL does not even look for glad, and one without a GLFW window does not look for GLFW.
if (CE_RENDER_OPENGL)
	add_subdirectory(ThirdParty/Glad)
endif ()

if (CE_WINDOW_GLFW)
	add_subdirectory(ThirdParty/Glfw)
endif ()

add_subdirectory(ThirdParty/Glm)
add_subdirectory(ThirdParty/Glslang)
add_subdirectory(ThirdParty/ImGui)
add_subdirectory(ThirdParty/SpdLog)
add_subdirectory(ThirdParty/SpirvCross)
add_subdirectory(ThirdParty/Stb)

# Apple third-party libraries
if (APPLE)
	add_subdirectory(ThirdParty/MetalCpp)
endif ()

# Runtime: the module libraries every application links
add_subdirectory(Source/Runtime/Core)
add_subdirectory(Source/Runtime/Define)
add_subdirectory(Source/Runtime/Events)
add_subdirectory(Source/Runtime/IO)
add_subdirectory(Source/Runtime/Native)
add_subdirectory(Source/Runtime/RHI)
add_subdirectory(Source/Runtime/Tools)
add_subdirectory(Source/Runtime/Types)
add_subdirectory(Source/Runtime/Utility)

# Programs: build-time tools, linked into no application
add_subdirectory(Source/Programs/ShaderCompiler)

# Shaders
add_subdirectory(Shaders)

# INTERFACE, not SHARED: CE_Engine has no code of its own - it is the one name an application links to get every
# module, their include directories and CE_Config. As a SHARED library it was a DLL exporting nothing, and MSVC's
# link.exe skips the import library of such a DLL, so every executable linking CE_Engine failed with the Visual Studio
# generator; an exported GetEngineVersion() used to be there only to dodge that. With no binary there is no DLL, no
# import library and no dylib to embed. GetEngineVersion() now lives in CE_Core (Core/Version.hpp).
# The headers are listed so IDEs show them under the target; an INTERFACE library compiles nothing.
add_library(CE_Engine INTERFACE
	${HEADERS}
)

# Modules
target_link_libraries(CE_Engine INTERFACE
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_IO
	CE_Native
	CE_RHI
	CE_Tools
	CE_Types
	CE_Utility
)

# Platform specific modules
target_link_libraries(CE_Engine INTERFACE
	$<$<PLATFORM_ID:Darwin>:
		CE_Vendor_MetalCpp
	>
)

# Shaders
target_link_libraries(CE_Engine INTERFACE
	CE_Shaders
)

target_include_directories(CE_Engine INTERFACE
	Source/Launch/include
)

# The engine's tests live with it (Engine/Tests) and come last, once every module target they link exists. They build
# into <build>/Out/<config>/Tests/. Turn them off to configure without them:
#   cmake --preset config-preset-clang -DCE_BUILD_TESTS=OFF
# A cache variable read at configure time, so flipping it needs a re-configure.
option(CE_BUILD_TESTS "Build the engine's test executables" ON)

if (CE_BUILD_TESTS)
	enable_testing()
	add_subdirectory(Tests)
endif ()
