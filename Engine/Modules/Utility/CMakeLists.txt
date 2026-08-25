#
# Module: CelestialEngine/Engine/Modules/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-21
# Updated by: Catalin Chirosca
# Updated: 2026-08-25
#

set(HEADERS
	# CMD
	src/include/Utility/CMD/Window.hpp

	# Config
	src/include/Utility/Config/Config.hpp

	# Delegate
	src/include/Utility/Delegate/Delegate.hpp
	src/include/Utility/Delegate/Dispatcher.hpp

	# FileSystem
	src/include/Utility/FileSystem/FileSystem.hpp
	src/include/Utility/FileSystem/File.hpp

	# ImGui
	src/include/Utility/ImGui/ImGui.hpp

	# Manipulation
	src/include/Utility/Manipulation/String.hpp

	# Range
	src/include/Utility/Range/Enumerate.hpp

	# Time
	src/include/Utility/Time/Chronometer.hpp

	# Main
	src/include/Utility/Utility.hpp
)

set(SOURCES
	# CMD
	src/source/Utility/CMD/Window.cpp

	# Config
	src/source/Utility/Config/Config.cpp

	# FileSystem
	src/source/Utility/FileSystem/FileSystem.cpp
	src/source/Utility/FileSystem/File.cpp

	# ImGui
	src/source/Utility/ImGui/ImGui.cpp

	# Manipulation
	src/source/Utility/Manipulation/String.cpp

	# Time
	src/source/Utility/Time/Chronometer.cpp
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Utility)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Utility)

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

target_compile_definitions(CE_Utility PRIVATE
	CE_UTILITY_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Utility)
