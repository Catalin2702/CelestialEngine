cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_Tests LANGUAGES CXX VERSION 0.0.3)

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_subdirectory(Engine)

# Target personalizzato per eseguire tutti i test
add_custom_target(run_all_tests
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running all tests..."
	USES_TERMINAL
)
