enable_testing()

set(SOURCES
	src/Platforms/Universal/OpenGlWindowTests.cpp
)

if(APPLE)
	list(APPEND SOURCES
		src/Platforms/Mac/MetalWindowTests.cpp
	)
else()
	message(FATAL_ERROR "No window tests implemented for this platform yet.")
endif()

add_executable(CE_TestsWindow
	${SOURCES}
)

set_target_properties(CE_TestsWindow PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_TestsWindow PRIVATE
	CE_Config
	CE_Window

	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)

add_test(NAME WindowTests
	COMMAND CE_TestsWindow
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(WindowTests PROPERTIES
	LABELS "Engine;Window"
)
