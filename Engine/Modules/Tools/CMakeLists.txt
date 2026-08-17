#
# Module: CelestialEngine/Engine/Modules/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

set(HEADERS
	src/include/Tools/Log/Log.hpp
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables - skipped on macOS, where doing
# this would force the DLL/ folder to exist as soon as CE_App alone is built (see
# ce_set_output_subdirectory's doc comment)
if (NOT APPLE)
	ce_set_output_subdirectory(CE_Tools DLL)
endif ()

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

target_compile_definitions(CE_Tools PRIVATE
	CE_TOOLS_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Tools)
