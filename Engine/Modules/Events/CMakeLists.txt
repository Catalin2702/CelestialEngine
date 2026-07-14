#
# Module: CelestialEngine/Engine/Modules/Events
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-07-14
#

set(HEADERS
	src/include/Events/ApplicationEvent.hpp
	src/include/Events/GenericEvent.hpp
	src/include/Events/KeyEvent.hpp
	src/include/Events/MouseEvent.hpp
)

set(INTERFACES
	src/interface/Events/I_Event.hpp
)

set(SOURCES
	src/source/Events/ApplicationEvent.cpp
	src/source/Events/GenericEvent.cpp
	src/source/Events/I_Event.cpp
	src/source/Events/KeyEvent.cpp
	src/source/Events/MouseEvent.cpp
)

add_library(CE_Events SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Events PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Modules
target_link_libraries(CE_Events PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

target_include_directories(CE_Events PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_Events PRIVATE
	CE_DEFINE_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Events)
