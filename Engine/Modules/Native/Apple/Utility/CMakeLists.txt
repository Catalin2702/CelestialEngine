#
# Module: CelestialEngine/Engine/Modules/Native/Apple/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-17
# Updated by: Catalin Chirosca
# Updated: 2026-03-17
#

cmake_minimum_required(VERSION 3.21)

set(HEADERS
	src/include/Utility/Time.hpp
)

set(SOURCES
)

# Create the CE_AppleUtility library
add_library(CE_AppleUtility SHARED
	${HEADERS}
	${SOURCES}
)

# Set the linker language to C++
set_target_properties(CE_AppleUtility PROPERTIES
	LINKER_LANGUAGE CXX
)

add_subdirectory(Time)

# Link the necessary modules to CE_AppleUtility
target_link_libraries(CE_AppleUtility PUBLIC
	CE_Config

	CE_AppleUtility_Time
)

target_include_directories(CE_AppleUtility PUBLIC
	src/include
	Time/src/include
)
