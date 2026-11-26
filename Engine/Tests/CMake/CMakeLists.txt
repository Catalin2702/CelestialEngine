#
# Module: CelestialEngine/Engine/Tests/CMake
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

# Tests of the engine's own CMake functions, run as CMake scripts (cmake -P): they need no compiler and no build, only
# the function under test and a scratch folder.
add_test(NAME ApplicationContentTests
	COMMAND ${CMAKE_COMMAND}
		-DCE_ENGINE_CMAKE_DIR=${CE_ENGINE_CMAKE_DIR}
		-DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/ApplicationContent
		-P ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationContentTests.cmake
)

set_tests_properties(ApplicationContentTests PROPERTIES
	LABELS "Engine;CMake"
)
