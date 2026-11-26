#
# Module: CelestialEngine/Engine/Tests/Runtime/IO
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

enable_testing()

set(SOURCES
	src/IOHeadersTests.cpp

	# FileSystem
	src/IO/FileSystem/FileSystemTests.cpp
	src/IO/FileSystem/VirtualPathTests.cpp
)

add_executable(CE_TestsIO
	${SOURCES}
)

set_target_properties(CE_TestsIO PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_TestsIO PRIVATE
	CE_Config
	CE_TestConfig
	CE_IO
)

# Third-party
target_link_libraries(CE_TestsIO PRIVATE
	GTest::gtest_main
)

add_test(NAME IOTests
	COMMAND CE_TestsIO
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsIO>
)

set_tests_properties(IOTests PROPERTIES
	LABELS "Engine;IO"
)

# Builds into Out/<config>/Tests/, and finds the module libraries:
# Windows via delay-load bootstrap, Linux via RPATH into ../DLL/, macOS via its own sibling copies
# (see Engine/CMake/CEApplication.cmake / CelestialEngineMacros.cmake for details)
ce_set_output_subdirectory(CE_TestsIO Tests)
ce_enable_dll_bootstrap(CE_TestsIO)
ce_set_dll_search_rpath(CE_TestsIO "../DLL")
ce_embed_apple_modules_beside(CE_TestsIO Tests)
