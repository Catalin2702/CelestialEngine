#
# Module: CelestialEngine/Engine/Tests/Runtime/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-29
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

enable_testing()

set(SOURCES
	src/UtilityHeadersTests.cpp

	src/Delegate/DelegateTests.cpp
	src/Delegate/DispatcherTests.cpp
	src/Manipulation/StringManipulationTests.cpp
	src/Time/ChronometerTests.cpp
)

add_executable(CE_TestsUtility
	${SOURCES}
)

set_target_properties(CE_TestsUtility PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsUtility PRIVATE
	CE_Config
	CE_TestConfig
	CE_Utility
)

# Third-party
target_link_libraries(CE_TestsUtility PRIVATE
	GTest::gtest_main
)

add_test(NAME UtilityTests
	COMMAND CE_TestsUtility
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsUtility>
)

set_tests_properties(UtilityTests PROPERTIES
	LABELS "Engine;Utility"
)

# Builds into Out/<config>/Tests/, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/CMake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsUtility Tests)
ce_enable_dll_bootstrap(CE_TestsUtility)
ce_set_dll_search_rpath(CE_TestsUtility "../DLL")
ce_embed_apple_modules_beside(CE_TestsUtility Tests)
