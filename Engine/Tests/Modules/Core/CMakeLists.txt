#
# Module: CelestialEngine/Engine/Tests/Modules/Core
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

set(CORE_SOURCES
	src/Core/CoreHeadersTests.cpp
)
set(INPUT_SOURCES
	src/Core/Input/InputTests.cpp
	src/Core/Input/KeyShortcutTests.cpp
)

set(LAYERS_SOURCES
	src/Core/Layers/LayerStackTests.cpp
	src/Core/Layers/LayerTests.cpp
)

set(MAINHUB_SOURCES
	# Events
	src/Core/Hub/Events/EventHubDispatcherTests.cpp
	# One per compiled-in window backend (CE_RENDER_BACKENDS)
	$<$<BOOL:${CE_WINDOW_GLFW}>:
		src/Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcherTests.cpp
	>

	$<$<BOOL:${CE_WINDOW_COCOA}>:
		src/Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcherTests.cpp
	>
)

set(RENDER_SOURCES
	# Buffer
	src/Core/Render/Buffer/BufferLayoutTests.cpp

	# Camera
	src/Core/Render/Camera/FreeCameraControllerTests.cpp
	src/Core/Render/Camera/MouseLookTests.cpp

	# Command
	src/Core/Render/Command/RenderPassDescriptorTests.cpp
	src/Core/Render/Command/ViewportTests.cpp

	# Pipeline
	src/Core/Render/Pipeline/PipelineDescriptorTests.cpp

	# Renderer
	src/Core/Render/Renderer/ForwardRendererTests.cpp
	src/Core/Render/Renderer/RenderStatsTests.cpp
)

add_executable(CE_TestsCore
	${CORE_SOURCES}
	${INPUT_SOURCES}
	${LAYERS_SOURCES}
	${MAINHUB_SOURCES}
	${RENDER_SOURCES}
)

set_target_properties(CE_TestsCore PROPERTIES
	LINKER_LANGUAGE CXX
)

# For the helpers shared by several test files (TestEventHub.hpp)
target_include_directories(CE_TestsCore PRIVATE
	src
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
	WORKING_DIRECTORY ${CE_BINARIES_DIR}
)

set_tests_properties(CoreTests PROPERTIES
	LABELS "Engine;Core"
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/cmake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsCore Tests)
ce_stage_to_binaries(CE_TestsCore Tests)
ce_enable_dll_bootstrap(CE_TestsCore)
ce_set_dll_search_rpath(CE_TestsCore "../DLL")
ce_embed_apple_modules_beside(CE_TestsCore Tests)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsCore)
