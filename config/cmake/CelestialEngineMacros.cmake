#
# Module: config/cmake
# File: CelestialEngineMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

if(NOT CMAKE_C_COMPILER)
	set(CMAKE_C_COMPILER "" CACHE STRING "No C compiler needed" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)

# Binaries/ subfolder keeping one toolchain's artifacts from overwriting another's. Set by
# every configure preset; falling back to the compiler ID means a bare "cmake -B ..." still
# lands somewhere sane instead of in "Binaries//<Config>".
if(NOT CE_TOOLCHAIN_NAME)
	set(CE_TOOLCHAIN_NAME "${CMAKE_CXX_COMPILER_ID}" CACHE STRING "Binaries/ subfolder identifying this toolchain")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>")
set(CE_LAST_BUILD_DIR "${CMAKE_SOURCE_DIR}/Binaries/Last")

# Declared as an option() so it shows up in the CMake settings UI / ccmake with a defined
# default, instead of being an undeclared variable that is only ever truthy by accident.
# Enable it per profile with -DCE_COPY_LAST=ON or a preset cacheVariable: it is read at
# configure time (it decides whether the POST_BUILD copies are generated at all), so
# flipping it needs a re-configure, not just a rebuild.
option(CE_COPY_LAST "Copy build artifacts to Binaries/Last" OFF)

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
		# Match both Homebrew LLVM (ID "Clang") and the Xcode toolchain (ID "AppleClang"): with only "Clang" the Apple
		# toolchain got NO optimization flags at all, leaving the custom Dist config (whose CMAKE_CXX_FLAGS_DIST default
		# is empty, unlike Release) at -O0.
		$<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<NOT:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
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

# Copies a target to the Binaries/Last folder, if CE_COPY_LAST is enabled
# Usage: ce_copy_to_last(target_name)
#
# A function rather than a macro: the CE_COPY_LAST guard lives here so the ~16 call sites
# do not have to repeat it, and only a function can bail out with return() (in a macro
# return() would exit the *calling* CMakeLists.txt, skipping whatever follows the call).
# The own scope is a bonus - TARGET_TYPE and IS_BUNDLE no longer leak into the caller.
function(ce_copy_to_last TARGET_NAME)
	if (NOT CE_COPY_LAST)
		return()
	endif ()

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
endfunction()
