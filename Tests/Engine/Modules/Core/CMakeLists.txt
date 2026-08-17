#
# Module: CelestialEngine/Tests/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

set(INPUT_SOURCES
	src/Core/Input/InputTests.cpp
)

set(LAYERS_SOURCES
	src/Core/Layers/LayerStackTests.cpp
	src/Core/Layers/LayerTests.cpp
)

set(MAINHUB_SOURCES
	# Events
	src/Core/MainHub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcherTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/MainHub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcherTests.cpp
	>
)

add_executable(CE_TestsCore
	${INPUT_SOURCES}
	${LAYERS_SOURCES}
	${MAINHUB_SOURCES}
)

set_target_properties(CE_TestsCore PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsCore PRIVATE
	CE_Config
	CE_TestConfig
	CE_Core
	CE_Vendor_SpdLog
)

# Third-party
target_link_libraries(CE_TestsCore PRIVATE
	GTest::gtest_main
)

add_test(NAME CoreTests
	COMMAND CE_TestsCore
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(CoreTests PROPERTIES
	LABELS "Engine;Core"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsCore)
