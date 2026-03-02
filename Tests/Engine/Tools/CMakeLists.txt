enable_testing()

set(SOURCES
	src/Tools/Time/ChronometerTests.cpp
	src/Tools/ImGui/ImGuiTests.cpp
	src/Tools/Log/LogTests.cpp
	src/Tools/Manipulation/StringManipulationTests.cpp
	src/Tools/CMD/WindowCMDTests.cpp
)

add_executable(CE_TestsTools
	${SOURCES}
)

set_target_properties(CE_TestsTools PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_TestsTools PRIVATE
	CE_Config
	CE_Tools

	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)

add_test(NAME ToolsTests
	COMMAND CE_TestsTools
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(ToolsTests PROPERTIES
	LABELS "Engine;Tools"
)

