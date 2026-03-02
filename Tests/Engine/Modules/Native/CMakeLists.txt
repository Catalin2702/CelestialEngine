enable_testing()

set(SOURCES)

if(APPLE)
	list(APPEND SOURCES
		src/Apple/Metal/MetalBridge/Cocoa/CocoaBridgeTests.cpp
		src/Apple/Metal/MetalBridge/ImGui/ImGuiBridgeTests.cpp
	)
else()
	message(FATAL_ERROR "No native tests implemented for this platform yet.")
endif()

add_executable(CE_TestsNative
	${SOURCES}
)

set_target_properties(CE_TestsNative PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_TestsNative PRIVATE
	CE_Config
	CE_Native

	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)


add_test(NAME NativeTests
	COMMAND CE_TestsNative
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(NativeTests PROPERTIES
	LABELS "Engine;Native"
)
