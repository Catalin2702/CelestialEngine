#
# Module: CelestialEngine/Engine/Modules/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-13
#

set(HEADERS
	src/include/Types/Window/WindowProps.hpp
	src/include/Types/Window/WindowDestructor.hpp
	src/include/Types/Container/Vec.hpp
	src/include/Types/KeyCode/KeyboardKeyCode.hpp
	src/include/Types/KeyCode/MouseButtonCode.hpp
)

set(SOURCES
	src/source/Types/Window/WindowProps.cpp
	src/source/Types/Window/WindowDestructor.cpp
	src/source/Types/KeyCode/KeyboardKeyCode.cpp
	src/source/Types/KeyCode/MouseButtonCode.cpp
	src/include/Types/Build/Build.hpp
	src/source/Types/Build/Build.cpp
)

add_library(CE_Types SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Types PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
	CE_Glfw
	CE_ImGui
)

target_include_directories(CE_Types PUBLIC
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Types)

