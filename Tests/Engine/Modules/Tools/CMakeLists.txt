#
# Module: CelestialEngine/Tests/Engine/Modules/Tools
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
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
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(ToolsTests PROPERTIES
	LABELS "Engine;Tools"
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and finds the module DLLs in
# ../DLL/ (Windows delay-load bootstrap / Linux RPATH - see App/CMakeLists.txt for details)
ce_set_output_subdirectory(CE_TestsTools Tests)
ce_enable_dll_bootstrap(CE_TestsTools)
ce_set_dll_search_rpath(CE_TestsTools "../DLL")

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsTools)

