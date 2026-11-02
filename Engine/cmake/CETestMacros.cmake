#
# Module: CelestialEngine/Engine/cmake
# File: CETestMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

if (NOT TARGET CE_TestConfig)
	add_library(CE_TestConfig INTERFACE)

	target_compile_definitions(CE_TestConfig INTERFACE
		CE_TESTS
	)

	# The tests exercise engine internals, CE_CORE_* logging included.
	target_link_libraries(CE_TestConfig INTERFACE
		CE_Internal
	)
endif ()
