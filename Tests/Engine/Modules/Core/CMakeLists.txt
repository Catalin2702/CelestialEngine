#
# Module: CelestialEngine/Tests/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-03-08
#

set(SOURCES
	src/Core/ApplicationTests.cpp
	src/Input/MetalInputTests.cpp
	src/Layers/ImGuiMetalLayerTests.cpp
	src/Layers/ImGuiOpenGlLayerTests.cpp
	src/Layers/LayerStackTests.cpp
	src/Layers/LayerTests.cpp
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
	CE_SpdLog
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
