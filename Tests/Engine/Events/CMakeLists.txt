set(SOURCES
	src/Events/ApplicationEventTests.cpp
	src/Events/EventTests.cpp
	src/Events/KeyEventTests.cpp
	src/Events/MouseEventTests.cpp
)

add_executable(CE_TestsEvents
	${SOURCES}
)

set_target_properties(CE_TestsEvents PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_TestsEvents PRIVATE
	CE_Config
	CE_Events
	CE_Define

	GTest::gtest
	GTest::gtest_main
)

target_include_directories(CE_TestsEvents PUBLIC
	${CMAKE_SOURCE_DIR}/Engine/Events/src/include
)

add_test(NAME EventsTests
	COMMAND CE_TestsEvents
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(EventsTests PROPERTIES
	LABELS "Engine;Events"
)
