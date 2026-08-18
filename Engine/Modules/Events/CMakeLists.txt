#
# Module: CelestialEngine/Engine/Modules/Events
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-16
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

set(HEADERS
	src/include/Events/ApplicationEvent.hpp
	src/include/Events/GenericEvent.hpp
	src/include/Events/KeyEvent.hpp
	src/include/Events/MouseEvent.hpp
	src/include/Events/RenderEvent.hpp
	src/include/Events/WindowEvent.hpp
)

set(INTERFACES
	src/interface/Events/I_Event.hpp
)

set(SOURCES
	src/source/Events/GenericEvent.cpp
	src/source/Events/KeyEvent.cpp
	src/source/Events/MouseEvent.cpp
	src/source/Events/RenderEvent.cpp
	src/source/Events/WindowEvent.cpp
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables: DLL/ on Windows and Linux, and on
# macOS out of Binaries/ entirely, into the build tree (see ce_set_module_library_output)
ce_set_module_library_output(CE_Events)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CE_App.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Events)

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
	CE_EVENTS_EXPORTS
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Events)
