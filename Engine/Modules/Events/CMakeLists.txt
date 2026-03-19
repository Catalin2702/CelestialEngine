#
# Module: CelestialEngine/Engine/Modules/Events
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-03-19
#

set(HEADERS
	src/include/Events/ApplicationEvent.hpp
	src/include/Events/I_Event.hpp
	src/include/Events/KeyEvent.hpp
	src/include/Events/MouseEvent.hpp
)

set(SOURCES
	src/source/Events/ApplicationEvent.cpp
	src/source/Events/I_Event.cpp
	src/source/Events/KeyEvent.cpp
	src/source/Events/MouseEvent.cpp
)

add_library(CE_Events SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Events PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Events PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

target_include_directories(CE_Events PUBLIC
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Events)

