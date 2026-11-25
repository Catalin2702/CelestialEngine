#
# Module: CelestialEngine/Engine/Tests/Modules/Define
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-11-25
#

set(SOURCES
	src/CompilationTests.cpp
)

add_executable(CE_TestsDefine ${SOURCES})

# Modules
target_link_libraries(CE_TestsDefine PRIVATE
	CE_Config
	CE_TestConfig
	CE_Define
)

# Third-party
target_link_libraries(CE_TestsDefine PRIVATE
	GTest::gtest_main
)

add_test(NAME DefineTests
	COMMAND CE_TestsDefine
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsDefine>
)

set_tests_properties(DefineTests PROPERTIES
	LABELS "Engine;Define"
)

# Builds into Out/<config>/Tests/, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/cmake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsDefine Tests)
ce_enable_dll_bootstrap(CE_TestsDefine)
ce_set_dll_search_rpath(CE_TestsDefine "../DLL")
ce_embed_apple_modules_beside(CE_TestsDefine Tests)
