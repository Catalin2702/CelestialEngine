#
# Module: config
# File: CelestialEngineMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-02
#

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>")

set(CMAKE_MAP_IMPORTED_CONFIG_DIST Release)

if (NOT TARGET CE_Config)
	add_library(CE_Config INTERFACE)

	target_compile_definitions(CE_Config INTERFACE
		$<$<CONFIG:Debug>:
		CE_DEBUG
		DEBUG
		>
		$<$<CONFIG:Release>:
		CE_RELEASE
		NDEBUG
		>
		$<$<CONFIG:Dist>:
		CE_DIST
		NDEBUG
		>

		$<$<PLATFORM_ID:Windows>:
		CE_PLATFORM_WINDOWS
		>
		$<$<PLATFORM_ID:Darwin>:
		CE_PLATFORM_MACOS
		>
		$<$<PLATFORM_ID:Linux>:
		CE_PLATFORM_LINUX
		>
	)

	target_compile_features(CE_Config INTERFACE
		cxx_std_23
	)

	target_compile_options(CE_Config INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
		$<$<CONFIG:Debug>:-O0 -g -Wall -Wextra -Werror>
		$<$<CONFIG:Release>:-O2>
		$<$<CONFIG:Dist>:-O3 -ffast-math>
		-march=native
		>

		$<$<CXX_COMPILER_ID:MSVC>:
		$<$<CONFIG:Debug>:/MDd /Od /Zi>
		$<$<OR:$<CONFIG:Release>,$<CONFIG:Dist>>:/MD /O2>
		/W4
		/WX
		/permissive-
		>
	)
endif ()
