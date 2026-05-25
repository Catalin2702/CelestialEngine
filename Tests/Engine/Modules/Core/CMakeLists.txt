#
# Module: CelestialEngine/Tests/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-05-25
#

set(APPLICATION_SOURCES
	src/Core/Application/Platforms/Common/Glfw/GlfwApplicationTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/Application/Platforms/Mac/Cocoa/CocoaApplicationTests.cpp
	>
)

set(INPUT_SOURCES
	src/Core/Input/Platforms/Common/Glfw/GlfwInputTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/Input/Platforms/Mac/Metal/MetalInputTests.cpp
	>
)

set(LAYERS_SOURCES
	src/Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayerTests.cpp
	src/Core/Layers/LayerStackTests.cpp
	src/Core/Layers/LayerTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayerTests.cpp
	>
)

set(WINDOW_SOURCES
	src/Core/Window/Platforms/Common/Glfw/GlfwWindowTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/Window/Platforms/Mac/Cocoa/CocoaWindowTests.cpp
	>
)

add_executable(CE_TestsCore
	${APPLICATION_SOURCES}
	${INPUT_SOURCES}
	${LAYERS_SOURCES}
	${WINDOW_SOURCES}
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
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(CoreTests PROPERTIES
	LABELS "Engine;Core"
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsCore)
