#
# Module: CelestialEngine/Engine/Modules/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-21
# Updated by: Catalin Chirosca
# Updated: 2026-03-22
#

set(HEADERS
	src/include/Utility/Callback/Callback.hpp
)

set(SOURCES
)

add_library(CE_Utility SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Utility PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Utility PUBLIC
	CE_Config
)

target_include_directories(CE_Utility PUBLIC
	src/include
)

target_compile_definitions(CE_Utility PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Utility)
