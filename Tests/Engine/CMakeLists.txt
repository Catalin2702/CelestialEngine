add_subdirectory(Core)
add_subdirectory(Events)
add_subdirectory(Tools)

# Target personalizzato per eseguire tutti i test di Engine
add_custom_target(run_engine_tests
	COMMAND ${CMAKE_CTEST_COMMAND} -L Engine --output-on-failure
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running all Engine tests..."
	USES_TERMINAL
)
