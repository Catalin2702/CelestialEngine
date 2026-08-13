#
# Module: CelestialEngine/Tests/Engine/Modules/Native
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-08-13
#

enable_testing()

set(SOURCES
	src/native.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Apple/Bridge/ImGui/ImGuiBridgeTests.cpp
		src/Apple/MetalCpp/AppKit/NsViewEventDispatcherTests.cpp
		src/Apple/MetalCpp/AppKit/NsWindowEventDispatcherTests.cpp
		src/Apple/MetalCpp/MetalKit/MtkViewDelegateTests.cpp
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
	CE_TestConfig
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

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsNative)

