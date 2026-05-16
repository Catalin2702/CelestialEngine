#
# Module: CelestialEngine/Engine/Modules/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-21
# Updated by: Catalin Chirosca
# Updated: 2026-05-16
#

set(HEADERS
	src/include/Utility/Callback/Callback.hpp
	src/include/Utility/CMD/Window.hpp
	src/include/Utility/Manipulation/String.hpp
	src/include/Utility/Time/Chronometer.hpp
	src/include/Utility/ImGui/ImGui.hpp
	src/include/Utility/FileSystem/FileSystem.hpp
	src/include/Utility/FileSystem/File.hpp
)

set(SOURCES
	src/source/Utility/CMD/Window.cpp
	src/source/Utility/Manipulation/String.cpp
	src/source/Utility/Time/Chronometer.cpp
	src/source/Utility/ImGui/ImGui.cpp
	src/source/Utility/FileSystem/FileSystem.cpp
	src/source/Utility/FileSystem/File.cpp
)

add_library(CE_Utility SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Utility PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Utility PUBLIC
	CE_Config
	CE_Tools
	CE_Types
)

target_include_directories(CE_Utility PUBLIC
	src/include
)

target_compile_definitions(CE_Utility PUBLIC
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Utility)
