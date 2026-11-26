#
# Module: CelestialEngine/Engine/Source/Runtime/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
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

# Keeps Out/<config>/ down to just executables: the library goes to its DLL/ subfolder on Windows and Linux, and to
# <build>/Modules/<config>/ on macOS (see ce_set_module_library_output)
ce_set_module_library_output(CE_Tools)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Tools)

# Modules
target_link_libraries(CE_Tools PUBLIC
	CE_Config
	CE_Define
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

# The Dist log's name is not set here: CE_Tools is built once for every executable of the build, so the name comes at run
# time from each application's main() (Log::Init(CE_APPLICATION_NAME), see Tools/Log/Log.hpp).
target_compile_definitions(CE_Tools PRIVATE
	CE_TOOLS_EXPORTS
)
