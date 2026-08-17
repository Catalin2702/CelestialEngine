#
# Module: CelestialEngine/Tests/Engine/Modules/Define
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

set(SOURCES
	src/CompilationTests.cpp
)

add_executable(CE_TestsDefine ${SOURCES})

# Modules
target_link_libraries(CE_TestsDefine PRIVATE
	CE_Config
	CE_TestConfig
	CE_Define
	CE_Events
)

# Third-party
target_link_libraries(CE_TestsDefine PRIVATE
	GTest::gtest_main
)

add_test(NAME DefineTests
	COMMAND CE_TestsDefine
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(DefineTests PROPERTIES
	LABELS "Engine;Define"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsDefine)

