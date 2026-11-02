#
# Module: CelestialEngine/Engine/Modules/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

set(HEADERS
	src/include/Tools/Log/Log.hpp

	# Main
	src/include/Tools/Tools.hpp
)

set(SOURCES
	src/source/Tools/Log/Log.cpp
)

add_library(CE_Tools SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Tools PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Tools)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Tools)

# Modules
target_link_libraries(CE_Tools PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

# Vendor libraries
target_link_libraries(CE_Tools PUBLIC
	CE_Vendor_SpdLog
)

target_include_directories(CE_Tools PUBLIC
	src/include
)

# Engine-internal: unlocks the CE_CORE_* logging macros (see CE_Internal in CelestialEngineMacros.cmake)
target_link_libraries(CE_Tools PRIVATE
	CE_Internal
)

# The name of the Dist log: its folder and its file (<user log dir>/<name>/<name>.log, see Tools/Log/Log.cpp). Taken from
# the top-level project() - CMAKE_PROJECT_NAME is the project of the CMakeLists.txt that drives the whole build, so it
# is "CelestialEngine" here and becomes the game's or the editor's name once one of them includes the engine with
# add_subdirectory(). CE_LOG_NAME overrides it; empty, the default, means "use the top-level project name".
set(CE_LOG_NAME "" CACHE STRING "Name of the Dist log folder and file; empty uses the top-level project name (CMAKE_PROJECT_NAME)")

if (CE_LOG_NAME)
	set(CE_RESOLVED_LOG_NAME "${CE_LOG_NAME}")
else ()
	set(CE_RESOLVED_LOG_NAME "${CMAKE_PROJECT_NAME}")
endif ()

# It becomes a path component and a C string literal, so separators and quotes are refused here rather than producing a
# log in an unexpected folder, or a compile error far from the cause.
if (CE_RESOLVED_LOG_NAME STREQUAL "" OR CE_RESOLVED_LOG_NAME MATCHES "[/\\\\\":]")
	message(FATAL_ERROR "Log name '${CE_RESOLVED_LOG_NAME}' is not usable as a folder and file name (set CE_LOG_NAME to override it).")
endif ()

target_compile_definitions(CE_Tools PRIVATE
	CE_TOOLS_EXPORTS
	CE_LOG_NAME="${CE_RESOLVED_LOG_NAME}"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Tools)
