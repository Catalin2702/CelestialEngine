#
# Module: Tests/Engine/Define
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Updated by: Catalin Chirosca
# Created: 2026-03-03
# Updated: 2026-03-03
#

set(SOURCES
	src/CompilationTests.cpp
)

add_executable(CE_TestsDefine ${SOURCES})

# Modules
target_link_libraries(CE_TestsDefine PRIVATE
	CE_Config
	CE_Define
	CE_Events
)

# Third-party
target_link_libraries(CE_TestsDefine PRIVATE
	GTest::gtest_main

#	$<$<NOT:$<CONFIG:Dist>>:
#		spdlog::spdlog
#	>
)

add_test(NAME DefineTests
	COMMAND CE_TestsDefine
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(DefineTests PROPERTIES
	LABELS "Engine;Define"
)
