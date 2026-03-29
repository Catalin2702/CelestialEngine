#
# Module: CelestialEngine/Tests/Engine/Modules/Window
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-29
#

enable_testing()

set(SOURCES
	src/Platforms/Common/GlfwWindowTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Platforms/Mac/CocoaWindowTests.cpp
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
	CE_Render
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

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsWindow)

