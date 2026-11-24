#
# Module: CelestialEngine/Engine/Modules/IO
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-24
#

# region Headers

set(BASE_HEADERS
	src/include/IO/IO.hpp
)

set(FILESYSTEM_HEADERS
	src/include/IO/FileSystem/FileSystem.hpp
	src/include/IO/FileSystem/FileSystemError.hpp
	src/include/IO/FileSystem/VirtualPath.hpp
)

set(HEADERS
	${BASE_HEADERS}
	${FILESYSTEM_HEADERS}
)

# endregion

# region Sources

set(FILESYSTEM_SOURCES
	src/source/IO/FileSystem/FileSystem.cpp
	src/source/IO/FileSystem/VirtualPath.cpp
)

set(SOURCES
	${FILESYSTEM_SOURCES}
)

# endregion

add_library(CE_IO SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_IO PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Out/<config>/ down to just executables: the library goes to its DLL/ subfolder on Windows and Linux, and to
# <build>/Modules/<config>/ on macOS (see ce_set_module_library_output)
ce_set_module_library_output(CE_IO)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_IO)

# Modules: what the public headers need - the platform macros, CE_IO_API and the integer aliases (u8) of CE_Types.
# Types is PUBLIC because the headers include it: every target that includes IO needs its include path too.
target_link_libraries(CE_IO PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

# Engine-internal: the CE_CORE_* logging macros (CE_Internal) and the logger behind them, used by Initialize alone. Not
# PUBLIC: nothing in the headers logs, so a tool linking CE_IO does not inherit the logger's include paths from it.
target_link_libraries(CE_IO PRIVATE
	CE_Internal
	CE_Tools
)

target_include_directories(CE_IO PUBLIC
	src/include
)

target_compile_definitions(CE_IO PRIVATE
	CE_IO_EXPORTS
)
