#
# Module: config/cmake
# File: CelestialEngineMacros.cmake
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
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

# Base names of the engine's own SHARED library modules (no extension): the set every
# executable delay-loads on Windows (see ce_enable_dll_bootstrap) and whose runtime artifact
# (.dll / .so / .dylib) is redirected into the DLL/ subfolder below, so
# Binaries/<toolchain>/<config>/ only ever holds executables.
set(CE_MODULE_DLL_NAMES
	CE_Core
	CE_Engine
	CE_Events
	CE_Tools
	CE_Types
	CE_Utility
)

# macOS also builds CE_Apple (the Cocoa/Metal native bridge, Modules/Native/Platforms/Apple) as
# its own SHARED library. It is embedded into CE_App.app/Contents/Frameworks alongside the others
# (see ce_bundle_apple_frameworks) but kept out of CE_MODULE_DLL_NAMES itself, since that list also
# drives ce_enable_dll_bootstrap's /DELAYLOAD flags on Windows, where CE_Apple does not exist.
if (APPLE)
	set(CE_APPLE_FRAMEWORK_MODULE_NAMES ${CE_MODULE_DLL_NAMES} CE_Apple)
else ()
	set(CE_APPLE_FRAMEWORK_MODULE_NAMES ${CE_MODULE_DLL_NAMES})
endif ()

# Redirects TARGET_NAME's runtime artifact (the .exe on Windows/Linux, or the .dll/.so) into
# Binaries/<toolchain>/<config>/<SUBDIR> instead of the flat default, so e.g. every module library
# can be pointed at "DLL" and every test executable at "Tests", leaving the top-level folder with
# just CE_App's own executable and Resources/. Do NOT use this for the module libraries on macOS:
# doing so would force Binaries/<toolchain>/<config>/DLL/ to exist as soon as CE_App alone is
# built (it needs to link them), even when no Tests/*.exe - the only macOS consumer that would
# actually need a shared folder - are being built at all. Skip it there and let those libraries
# build to CMake's own default location instead; ce_bundle_apple_frameworks /
# ce_embed_apple_modules_beside copy them out of wherever that is on demand.
# Usage: ce_set_output_subdirectory(target_name subdir)
function(ce_set_output_subdirectory TARGET_NAME SUBDIR)
	set_target_properties(${TARGET_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>/${SUBDIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>/${SUBDIR}"
	)
endfunction()

# Gives a CE module SHARED library (Core, Events, Tools, Types, Utility, Engine, and on macOS also
# Apple) its own "look in my own directory" RPATH, so once every module library ends up sitting
# flat in the same folder - DLL/ in the build tree and on Linux, or CE_App.app/Contents/Frameworks
# on macOS (see ce_bundle_apple_frameworks) - it resolves its dependencies on sibling module
# libraries as-is, without needing install_name_tool/patchelf rewriting after the fact. On macOS
# this also sets INSTALL_NAME_DIR so the library's own identity is @rpath-relative (portable)
# instead of an absolute build-tree path. No-op on Windows, which resolves inter-module DLL
# dependencies via ce_enable_dll_bootstrap instead.
# Usage: ce_configure_module_library_rpath(target_name)
function(ce_configure_module_library_rpath TARGET_NAME)
	if (WIN32)
		return()
	endif ()

	if (APPLE)
		set_target_properties(${TARGET_NAME} PROPERTIES
			INSTALL_NAME_DIR "@rpath"
			BUILD_RPATH "@loader_path"
			INSTALL_RPATH "@loader_path"
		)
	else ()
		set_target_properties(${TARGET_NAME} PROPERTIES
			BUILD_RPATH "$ORIGIN"
			INSTALL_RPATH "$ORIGIN"
		)
	endif ()
endfunction()

# Embeds an $ORIGIN-relative RPATH into TARGET_NAME - a plain (non-bundle) Linux executable, such
# as one of the Tests/*.exe or CE_App itself - so the dynamic loader finds the module libraries in
# RELATIVE_PATH without needing LD_LIBRARY_PATH set. Linux-only: Windows uses
# ce_enable_dll_bootstrap, and macOS uses ce_bundle_apple_frameworks (CE_App, which embeds its own
# copies into the .app bundle so it stays portable if moved) or ce_embed_apple_modules_beside
# (plain executables, e.g. Tests/*.exe) instead - neither needs a shared DLL/ folder to exist at
# all, which this RPATH-to-an-external-folder approach would otherwise force into being built
# even when only CE_App (no tests) is being built.
# Usage: ce_set_dll_search_rpath(target_name relative_path_to_dll_dir)  e.g. "DLL" or "../DLL"
function(ce_set_dll_search_rpath TARGET_NAME RELATIVE_PATH)
	if (NOT (CMAKE_SYSTEM_NAME STREQUAL "Linux"))
		return()
	endif ()

	set_target_properties(${TARGET_NAME} PROPERTIES
		BUILD_RPATH "$ORIGIN/${RELATIVE_PATH}"
		INSTALL_RPATH "$ORIGIN/${RELATIVE_PATH}"
	)
endfunction()

# On macOS, copies (and ad-hoc re-signs, same reasoning as ce_bundle_apple_frameworks) every
# CE_APPLE_FRAMEWORK_MODULE_NAMES library directly beside TARGET_NAME - a plain (non-bundle)
# executable, such as one of the Tests/*.exe. Each copied library resolves its own sibling
# dependencies via its own @loader_path RPATH (see ce_configure_module_library_rpath), so once
# they're all sitting flat next to the executable it needs no further help finding them - no
# shared DLL/ folder required. That matters because ce_set_output_subdirectory is intentionally
# skipped for the module libraries on macOS (see each module's CMakeLists.txt): without a shared
# folder to redirect to, TARGET_NAME needs its own copy regardless. No-op elsewhere - Windows and
# Linux use ce_enable_dll_bootstrap / ce_set_dll_search_rpath instead.
# Usage: ce_embed_apple_modules_beside(target_name)
function(ce_embed_apple_modules_beside TARGET_NAME)
	if (NOT APPLE)
		return()
	endif ()

	foreach (MODULE_LIB ${CE_APPLE_FRAMEWORK_MODULE_NAMES})
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:${MODULE_LIB}>"
				"$<TARGET_FILE_DIR:${TARGET_NAME}>/"
			COMMAND codesign --force --sign -
				"$<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_NAME:${MODULE_LIB}>"
			COMMENT "Embedding and signing ${MODULE_LIB} beside ${TARGET_NAME}"
		)
	endforeach ()
endfunction()

# On Windows, marks every CE_MODULE_DLL_NAMES import as delay-loaded on TARGET_NAME and compiles
# in the bootstrap translation unit that registers the DLL/ subfolder as an additional search
# directory (via AddDllDirectory) before those imports are first touched. Needed because the
# module DLLs live in DLL/ instead of next to the executable, and unlike Linux/macOS's RPATH, the
# Windows loader has no notion of "look in this subfolder relative to me" - it only searches the
# executable's own directory, system directories and PATH. No-op elsewhere.
# Usage: ce_enable_dll_bootstrap(target_name)
function(ce_enable_dll_bootstrap TARGET_NAME)
	if (NOT WIN32)
		return()
	endif ()

	target_sources(${TARGET_NAME} PRIVATE
		"${CMAKE_SOURCE_DIR}/Engine/Modules/Native/Platforms/Windows/src/source/Windows/DllSearchBootstrap.cpp"
	)

	target_link_options(${TARGET_NAME} PRIVATE
		/DEFAULTLIB:delayimp
	)

	foreach (MODULE_DLL ${CE_MODULE_DLL_NAMES})
		target_link_options(${TARGET_NAME} PRIVATE
			/DELAYLOAD:${MODULE_DLL}.dll
		)
	endforeach ()
endfunction()

# On macOS, embeds every CE_APPLE_FRAMEWORK_MODULE_NAMES library into TARGET_NAME's own
# Contents/Frameworks/ - the standard, portable way to ship a self-contained .app bundle - and
# points the bundle executable's RPATH at @executable_path/../Frameworks so it resolves them
# there. Each embedded library resolves ITS OWN sibling dependencies the same way, via its own
# @loader_path RPATH set by ce_configure_module_library_rpath, so no install_name_tool rewriting
# is needed after the copy. Each copied library is also re-signed ad-hoc on the spot: signing
# nested content up front and letting the OUTER app signing step (see App/CMakeLists.txt) run
# last, without --deep, is Apple's documented order for bundles with embedded code - --deep
# resigns nested items in an unspecified order and does not reliably produce a valid seal, which
# is what made Gatekeeper flag the bundle as "damaged" when a plain --deep sign ran after this
# copy. Requires TARGET_NAME to be a MACOSX_BUNDLE executable. No-op elsewhere - Windows/Linux use
# ce_enable_dll_bootstrap / ce_set_dll_search_rpath instead, pointing at a shared DLL/ folder
# rather than duplicating the module libraries into the bundle.
# Usage: ce_bundle_apple_frameworks(target_name)
function(ce_bundle_apple_frameworks TARGET_NAME)
	if (NOT APPLE)
		return()
	endif ()

	set_target_properties(${TARGET_NAME} PROPERTIES
		BUILD_RPATH "@executable_path/../Frameworks"
		INSTALL_RPATH "@executable_path/../Frameworks"
	)

	foreach (MODULE_LIB ${CE_APPLE_FRAMEWORK_MODULE_NAMES})
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_CONTENT_DIR:${TARGET_NAME}>/Frameworks"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:${MODULE_LIB}>"
				"$<TARGET_BUNDLE_CONTENT_DIR:${TARGET_NAME}>/Frameworks/"
			COMMAND codesign --force --sign -
				"$<TARGET_BUNDLE_CONTENT_DIR:${TARGET_NAME}>/Frameworks/$<TARGET_FILE_NAME:${MODULE_LIB}>"
			COMMENT "Embedding and signing ${MODULE_LIB} in ${TARGET_NAME}.app/Contents/Frameworks"
		)
	endforeach ()
endfunction()
