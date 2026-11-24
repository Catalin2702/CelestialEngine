#
# Module: CelestialEngine/Engine/Tests/Modules/RHI
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-24
#

enable_testing()

set(SOURCES
	src/RHIHeadersTests.cpp

	# Buffer
	src/RHI/Buffer/BufferLayoutTests.cpp

	# Command
	src/RHI/Command/RenderPassDescriptorTests.cpp
	src/RHI/Command/ViewportTests.cpp

	# Conversion, tested only where its backend is compiled (CE_RENDER_BACKENDS)
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversionTests.cpp
	>

	# Pipeline
	src/RHI/Pipeline/PipelineDescriptorTests.cpp
)

add_executable(CE_TestsRHI
	${SOURCES}
)

set_target_properties(CE_TestsRHI PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules: CE_RHI alone - not CE_Core - so these tests also prove the RHI builds and links without the rest of the
# engine above it. CE_Tools for the logger ViewportTests starts.
target_link_libraries(CE_TestsRHI PRIVATE
	CE_Config
	CE_TestConfig
	CE_RHI
	CE_Tools
	CE_Vendor_SpdLog
)

# glad is private to CE_RHI; the conversion tests compare against the GL constants themselves.
if (CE_RENDER_OPENGL)
	target_link_libraries(CE_TestsRHI PRIVATE
		CE_Vendor_Glad
	)
endif ()

# Third-party
target_link_libraries(CE_TestsRHI PRIVATE
	GTest::gtest_main
)

add_test(NAME RHITests
	COMMAND CE_TestsRHI
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsRHI>
)

set_tests_properties(RHITests PROPERTIES
	LABELS "Engine;RHI"
)

# Builds into Out/<config>/Tests/, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/cmake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsRHI Tests)
ce_enable_dll_bootstrap(CE_TestsRHI)
ce_set_dll_search_rpath(CE_TestsRHI "../DLL")
ce_embed_apple_modules_beside(CE_TestsRHI Tests)
