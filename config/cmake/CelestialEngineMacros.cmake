#
# Module: config
# File: CelestialEngineMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-30
#

if(NOT CMAKE_C_COMPILER)
	set(CMAKE_C_COMPILER "" CACHE STRING "No C compiler needed" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>")
set(CE_LAST_BUILD_DIR "${CMAKE_SOURCE_DIR}/Binaries/Last")

set(CMAKE_MAP_IMPORTED_CONFIG_DIST Release "")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

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
		$<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
			$<$<CONFIG:Debug>:
				-O0
				-g
				-Wall
				-Wextra
				-Werror
			>
			$<$<CONFIG:Release>:
				-O2
			>
			$<$<CONFIG:Dist>:
				-O3
				-ffast-math
			>
			-march=native
		>

		$<$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:
			$<$<CONFIG:Debug>:
				/MDd
				/Od
				/Zi
				/W4
				/WX
			>
			$<$<OR:$<CONFIG:Release>,$<CONFIG:Dist>>:
				/MD
				/O2
			>

			/permissive-
			/utf-8
			/wd4251
		>
	)

	target_link_options(CE_Config INTERFACE
		$<$<PLATFORM_ID:Windows>:
			$<$<CONFIG:Debug>:
				/DEFAULTLIB:msvcrtd
				/DEFAULTLIB:vcruntimed
				/DEFAULTLIB:ucrtd
			>
			$<$<OR:$<CONFIG:Release>,$<CONFIG:Dist>>:
				/DEFAULTLIB:msvcrt
				/DEFAULTLIB:vcruntime
				/DEFAULTLIB:ucrt
			>
			/VERBOSE
		>
	)
endif ()

# Macro to copy targets to the Binaries/Last folder
# Usage: ce_copy_to_last(target_name)
macro(ce_copy_to_last TARGET_NAME)
	# Create the Binaries/Last directory if it doesn't exist
	file(MAKE_DIRECTORY "${CE_LAST_BUILD_DIR}")

	# Get the target type
	get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

	# For executables and libraries
	if(TARGET_TYPE STREQUAL "EXECUTABLE" OR
	   TARGET_TYPE STREQUAL "SHARED_LIBRARY" OR
	   TARGET_TYPE STREQUAL "MODULE_LIBRARY")

		# For macOS bundles (like CE_App.app)
		get_target_property(IS_BUNDLE ${TARGET_NAME} MACOSX_BUNDLE)
		if(IS_BUNDLE)
			# Copy the entire bundle
			add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E remove_directory "${CE_LAST_BUILD_DIR}/${TARGET_NAME}.app"
				COMMAND ${CMAKE_COMMAND} -E copy_directory
					"$<TARGET_BUNDLE_DIR:${TARGET_NAME}>"
					"${CE_LAST_BUILD_DIR}/${TARGET_NAME}.app"
				COMMENT "Copying ${TARGET_NAME}.app to Binaries/Last"
			)
		else()
			# Copy the executable or library file
			add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"$<TARGET_FILE:${TARGET_NAME}>"
					"${CE_LAST_BUILD_DIR}/$<TARGET_FILE_NAME:${TARGET_NAME}>"
				COMMENT "Copying ${TARGET_NAME} to Binaries/Last"
			)
		endif()
	endif()
endmacro()

