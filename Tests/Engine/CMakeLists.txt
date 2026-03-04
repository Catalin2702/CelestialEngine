#
# Module: CelestialEngine/Tests/Engine/
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-03-03
#

add_subdirectory(Modules/Core)
add_subdirectory(Modules/Define)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Native)
add_subdirectory(Modules/Tools)
add_subdirectory(Modules/Window)

# Target personalizzato per eseguire tutti i test di Engine
add_custom_target(RunEngineTests
	COMMAND ${CMAKE_CTEST_COMMAND} -L Engine --output-on-failure
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running all Engine tests..."
	USES_TERMINAL
)
