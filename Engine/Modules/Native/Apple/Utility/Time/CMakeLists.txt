#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Utility/Time
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-17
# Updated by: Catalin Chirosca
# Updated: 2026-03-17
#

set(HEADERS
	src/include/Time/UtilityTime.hpp
)

set(SOURCES
	src/source/Time/UtilityTime.cpp
)

add_library(CE_AppleUtility_Time SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_AppleUtility_Time PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_AppleUtility_Time PUBLIC
	CE_Config
)

target_include_directories(CE_AppleUtility_Time PUBLIC
	src/include
)

ce_copy_to_last(CE_AppleUtility_Time)
