#
# Module: CelestialEngine/Tests/Engine/Modules/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-08-13
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

enable_testing()

set(BUILD_SOURCES
	src/Types/Build/BuildTests.cpp
)

set(KEYCODE_SOURCES
	src/Types/KeyCode/KeyboardKeyCodeTests.cpp
	src/Types/KeyCode/MouseButtonCodeTests.cpp
)

set(RENDER_SOURCES
	src/Types/Render/RenderTests.cpp
	src/Types/Render/ShaderTests.cpp
	src/Types/Render/Platforms/Common/OpenGl/OpenGlTests.cpp
)

set(WINDOW_SOURCES
	src/Types/Window/WindowPropsTests.cpp
)

add_executable(CE_TestsTypes
	${BUILD_SOURCES}
	${KEYCODE_SOURCES}
	${RENDER_SOURCES}
	${WINDOW_SOURCES}
)

set_target_properties(CE_TestsTypes PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsTypes PRIVATE
	CE_Config
	CE_TestConfig
	CE_Types
)

# Third-party
target_link_libraries(CE_TestsTypes PRIVATE
	GTest::gtest_main
)

add_test(NAME TypesTests
	COMMAND CE_TestsTypes
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>
)

set_tests_properties(TypesTests PROPERTIES
	LABELS "Engine;Types"
)

# Keeps Binaries/<toolchain>/<config>/ down to just executables, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see App/CMakeLists.txt / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsTypes Tests)
ce_stage_to_binaries(CE_TestsTypes Tests)
ce_enable_dll_bootstrap(CE_TestsTypes)
ce_set_dll_search_rpath(CE_TestsTypes "../DLL")
ce_embed_apple_modules_beside(CE_TestsTypes Tests)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_TestsTypes)
