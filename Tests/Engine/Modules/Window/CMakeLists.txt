#
# Module: CelestialEngine/Tests/Engine/Modules/Window
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-30
#

enable_testing()

set(SOURCES
	src/Platforms/Common/Glfw/GlfwWindowTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Platforms/Mac/Cocoa/CocoaWindowTests.cpp
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
	CE_Native
	CE_Render
	CE_Window
)

# Third-party
target_link_libraries(CE_TestsWindow PRIVATE
	GTest::gtest_main
)

#target_link_options(CE_TestsWindow PRIVATE
#	$<$<PLATFORM_ID:Darwin>:
#		-ObjC++
#		-fno-objc-arc
#	>
#)

add_test(NAME WindowTests
	COMMAND CE_TestsWindow
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(WindowTests PROPERTIES
	LABELS "Engine;Window"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsWindow)
