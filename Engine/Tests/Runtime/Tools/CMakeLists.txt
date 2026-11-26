#
# Module: CelestialEngine/Engine/Tests/Runtime/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

enable_testing()

set(SOURCES
	src/Tools/Log/LogTests.cpp
)

add_executable(CE_TestsTools
	${SOURCES}
)

set_target_properties(CE_TestsTools PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsTools PRIVATE
	CE_Config
	CE_TestConfig
	CE_Tools
)

# Third-party
target_link_libraries(CE_TestsTools PRIVATE
	GTest::gtest_main
)

add_test(NAME ToolsTests
	COMMAND CE_TestsTools
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsTools>
)

set_tests_properties(ToolsTests PROPERTIES
	LABELS "Engine;Tools"
)

# Builds into Out/<config>/Tests/, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/CMake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsTools Tests)
ce_enable_dll_bootstrap(CE_TestsTools)
ce_set_dll_search_rpath(CE_TestsTools "../DLL")
ce_embed_apple_modules_beside(CE_TestsTools Tests)
