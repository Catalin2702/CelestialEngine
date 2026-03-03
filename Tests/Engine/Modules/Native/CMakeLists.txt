enable_testing()

set(SOURCES
	$<$<PLATFORM_ID:Darwin>:
		src/Apple/Metal/MetalBridge/Cocoa/CocoaBridgeTests.cpp
		src/Apple/Metal/MetalBridge/ImGui/ImGuiBridgeTests.cpp
	>
)

add_executable(CE_TestsNative
	${SOURCES}
)

set_target_properties(CE_TestsNative PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsNative PRIVATE
	CE_Config
	CE_Native
)

# Third-party
target_link_libraries(CE_TestsNative PRIVATE
	GTest::gtest_main
)

add_test(NAME NativeTests
	COMMAND CE_TestsNative
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(NativeTests PROPERTIES
	LABELS "Engine;Native"
)
