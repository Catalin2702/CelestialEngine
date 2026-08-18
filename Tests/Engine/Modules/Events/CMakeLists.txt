#
# Module: CelestialEngine/Tests/Engine/Modules/Events
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

set(SOURCES
	src/Events/ApplicationEventTests.cpp
	src/Events/EventTests.cpp
	src/Events/GenericEventTests.cpp
	src/Events/KeyEventTests.cpp
	src/Events/MouseEventTests.cpp
	src/Events/RenderEventTests.cpp
	src/Events/WindowEventTests.cpp
)

add_executable(CE_TestsEvents
	${SOURCES}
)

set_target_properties(CE_TestsEvents PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsEvents PRIVATE
	CE_Config
	CE_TestConfig
	CE_Events
	CE_Define
)

# Third-party
target_link_libraries(CE_TestsEvents PRIVATE
	GTest::gtest_main
)

add_test(NAME EventsTests
	COMMAND CE_TestsEvents
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(EventsTests PROPERTIES
	LABELS "Engine;Events"
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see App/CMakeLists.txt / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsEvents Tests)
ce_stage_to_binaries(CE_TestsEvents Tests)
ce_enable_dll_bootstrap(CE_TestsEvents)
ce_set_dll_search_rpath(CE_TestsEvents "../DLL")
ce_embed_apple_modules_beside(CE_TestsEvents Tests)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsEvents)

