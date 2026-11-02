#
# Module: CelestialEngine/Engine/Tests
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

# Added by Engine/CMakeLists.txt when CE_BUILD_TESTS is on, so the engine macros (CelestialEngineMacros.cmake) and
# every module target already exist here. The tests belong to the engine: they move with it, and a project that
# includes the engine gets them - or turns them off - through that one option.
project(CE_Tests LANGUAGES CXX VERSION 0.1.3)

include("${CE_ENGINE_CMAKE_DIR}/CETestMacros.cmake")

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Types)
add_subdirectory(Modules/Utility)

# Runs every engine test (each one carries the "Engine" label next to its module's)
add_custom_target(RunEngineTests
	COMMAND ${CMAKE_CTEST_COMMAND} -L Engine --output-on-failure
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running all Engine tests..."
	USES_TERMINAL
)
