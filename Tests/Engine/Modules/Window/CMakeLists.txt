enable_testing()

set(SOURCES
	src/Platforms/Universal/OpenGlWindowTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Platforms/Mac/MetalWindowTests.cpp
	>
)

add_executable(CE_TestsWindow
	${SOURCES}
)

set_target_properties(CE_TestsWindow PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsWindow PRIVATE
	CE_Config
	CE_Window
)

# Third-party
target_link_libraries(CE_TestsWindow PRIVATE
	GTest::gtest_main
)

add_test(NAME WindowTests
	COMMAND CE_TestsWindow
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(WindowTests PROPERTIES
	LABELS "Engine;Window"
)
