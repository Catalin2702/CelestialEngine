#
# Module: CelestialEngine/Engine/Modules/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-21
# Updated by: Catalin Chirosca
# Updated: 2026-07-04
#

set(HEADERS
	# Callback
	src/include/Utility/Callback/Callback.hpp
	src/include/Utility/Callback/EventDispatcher.hpp

	# CMD
	src/include/Utility/CMD/Window.hpp

	# Config
	src/include/Utility/Config/Config.hpp

	# Manipulation
	src/include/Utility/Manipulation/String.hpp

	# Time
	src/include/Utility/Time/Chronometer.hpp

	# ImGui
	src/include/Utility/ImGui/ImGui.hpp

	# FileSystem
	src/include/Utility/FileSystem/FileSystem.hpp
	src/include/Utility/FileSystem/File.hpp

	# Main
	src/include/Utility/Utility.hpp
)

set(SOURCES
	# CMD
	src/source/Utility/CMD/Window.cpp

	# Config
	src/source/Utility/Config/Config.cpp

	# Manipulation
	src/source/Utility/Manipulation/String.cpp

	# Time
	src/source/Utility/Time/Chronometer.cpp

	# ImGui
	src/source/Utility/ImGui/ImGui.cpp

	# FileSystem
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
	CE_Define
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
