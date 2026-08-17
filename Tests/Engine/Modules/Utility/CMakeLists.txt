#
# Module: CelestialEngine/Tests/Engine/Modules/Utility
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-29
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

enable_testing()

set(SOURCES
	src/CMD/WindowCMDTests.cpp
	src/Config/ConfigTests.cpp
	src/Delegate/DelegateTests.cpp
	src/Delegate/DispatcherTests.cpp
	src/FileSystem/FileSystemTests.cpp
	src/ImGui/ImGuiTests.cpp
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
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(UtilityTests PROPERTIES
	LABELS "Engine;Utility"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsUtility)
