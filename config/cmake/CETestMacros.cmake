#
# Module: config/cmake
# File: CETestMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-05-15
# Updated by: Catalin Chirosca
# Updated: 2026-05-15
#

if (NOT TARGET CE_TestConfig)
	add_library(CE_TestConfig INTERFACE)

	target_compile_definitions(CE_TestConfig INTERFACE
		CE_TESTS
	)
endif ()
