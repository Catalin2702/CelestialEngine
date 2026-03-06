#
# Module: CelestialEngine/Engine/Modules/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-03-06
#

set(HEADERS
	src/include/Tools/CMD/Window.hpp
	src/include/Tools/Log/Log.hpp
	src/include/Tools/Manipulation/String.hpp
	src/include/Tools/Time/Chronometer.hpp
	src/include/Tools/ImGui/ImGui.hpp
)

set(SOURCES
	src/source/Tools/CMD/Window.cpp
	src/source/Tools/Log/Log.cpp
	src/source/Tools/Manipulation/String.cpp
	src/source/Tools/Time/Chronometer.cpp
	src/source/Tools/ImGui/ImGui.cpp
)

add_library(CE_Tools SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Tools PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Tools PUBLIC
	CE_Config
	CE_Define
	CE_SpdLog
	CE_Types
)

target_include_directories(CE_Tools PUBLIC
	src/include
)
