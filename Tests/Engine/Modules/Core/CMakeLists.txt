#
# Module: CelestialEngine/Tests/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-04-27
#

set(SOURCES
	src/Core/GlfwApplicationTests.cpp
	src/Input/Platforms/Glfw/GlfwInputTests.cpp
	src/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayerTests.cpp
	src/Layers/LayerStackTests.cpp
	src/Layers/LayerTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/CocoaApplicationTests.cpp
		src/Input/Platforms/Metal/MetalInputTests.cpp
		src/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayerTests.cpp
	>
)

add_executable(CE_TestsCore
	${SOURCES}
)

set_target_properties(CE_TestsCore PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsCore PRIVATE
	CE_Config
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
