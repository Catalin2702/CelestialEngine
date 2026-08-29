#
# Module: CelestialEngine/Tests/Engine/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-08-29
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
	src/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcherTests.cpp

	$<$<PLATFORM_ID:Darwin>:
		src/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcherTests.cpp
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

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see App/CMakeLists.txt / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsCore Tests)
ce_stage_to_binaries(CE_TestsCore Tests)
ce_enable_dll_bootstrap(CE_TestsCore)
ce_set_dll_search_rpath(CE_TestsCore "../DLL")
ce_embed_apple_modules_beside(CE_TestsCore Tests)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsCore)
