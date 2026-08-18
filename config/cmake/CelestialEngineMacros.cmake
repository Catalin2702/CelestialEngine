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

# Where the artifacts are DELIVERED. Nothing is ever built straight into it: CMake creates a
# target's output directory (and, for a MACOSX_BUNDLE, the whole Contents/ skeleton with its
# Info.plist) at GENERATE time, so pointing any target here would make Binaries/<toolchain>/<Config>
# spring into existence - empty Tests/ folder, hollow CelestialEngine.app and all - for all three
# configurations of every profile an IDE loads, without a single file being compiled. Everything is
# built inside the build tree instead and copied here by ce_stage_to_binaries() as a POST_BUILD
# step, so a folder under Binaries/ only ever appears once something was actually built into it.
set(CE_BINARIES_DIR "${CMAKE_SOURCE_DIR}/Binaries/${CE_TOOLCHAIN_NAME}/$<CONFIG>")
set(CE_LAST_BUILD_DIR "${CMAKE_SOURCE_DIR}/Binaries/Last")

# The build-tree counterpart of CE_BINARIES_DIR: the real output directory of every executable and
# shared library, mirroring the same <subfolder> layout.
set(CE_BUILD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Out/$<CONFIG>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CE_BUILD_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CE_BUILD_OUTPUT_DIR}")

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
			# Copy the entire bundle, under its REAL name ($<TARGET_BUNDLE_DIR_NAME>, i.e.
			# CelestialEngine.app - the target's OUTPUT_NAME) rather than "${TARGET_NAME}.app":
			# calling it CE_App.app here produced a bundle whose executable was still named
			# CelestialEngine, and whatever runs it (Scripts/Run/run_all.sh) then looked for the
			# wrong path.
			add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E remove_directory
					"${CE_LAST_BUILD_DIR}/$<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}>"
				COMMAND ${CMAKE_COMMAND} -E copy_directory
					"$<TARGET_BUNDLE_DIR:${TARGET_NAME}>"
					"${CE_LAST_BUILD_DIR}/$<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}>"
				COMMENT "Copying $<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}> to Binaries/Last"
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

# Redirects TARGET_NAME's runtime artifact into <build>/Out/<config>/<SUBDIR> instead of the flat
# default, so e.g. every test executable can be pointed at "Tests" and, once staged, ends up in
# Binaries/<toolchain>/<config>/Tests/ rather than beside CelestialEngine.app. Pair it with
# ce_stage_to_binaries(TARGET_NAME SUBDIR) using the SAME subfolder.
# Usage: ce_set_output_subdirectory(target_name subdir)
function(ce_set_output_subdirectory TARGET_NAME SUBDIR)
	set_target_properties(${TARGET_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CE_BUILD_OUTPUT_DIR}/${SUBDIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CE_BUILD_OUTPUT_DIR}/${SUBDIR}"
	)
endfunction()

# Copies TARGET_NAME's build-tree artifact into Binaries/<toolchain>/<config>[/SUBDIR] after it is
# built - the ONLY way anything gets into Binaries/, see CE_BINARIES_DIR above for why nothing is
# built there directly. Handles both plain files and macOS .app bundles (copied whole, under their
# real bundle name).
#
# Registration order matters: this must be called AFTER any other POST_BUILD step that modifies the
# artifact (code signing, resource copies), since POST_BUILD commands run in the order they were
# added - staging first would ship a half-finished bundle.
# Usage: ce_stage_to_binaries(target_name [subdir])
function(ce_stage_to_binaries TARGET_NAME)
	set(DEST "${CE_BINARIES_DIR}")
	if (ARGC GREATER 1 AND NOT "${ARGV1}" STREQUAL "")
		set(DEST "${CE_BINARIES_DIR}/${ARGV1}")
	endif ()

	get_target_property(IS_BUNDLE ${TARGET_NAME} MACOSX_BUNDLE)
	get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

	# The staged file is declared as a BYPRODUCT so the generator knows this build step produces it:
	# deleting it (or the whole Binaries/ tree) by hand makes the step dirty again and the next
	# build re-stages it, instead of silently leaving a hole because nothing needed relinking. The
	# name has to be spelled out here rather than taken from $<TARGET_FILE_NAME:...>, since
	# BYPRODUCTS generator expressions are evaluated without a target context. That is also why
	# shared libraries get no byproduct: their real file name carries the VERSION suffix
	# (libCE_Core.0.1.3.dylib), which is not reconstructible from the target properties alone.
	get_target_property(STAGED_NAME ${TARGET_NAME} OUTPUT_NAME)
	if (NOT STAGED_NAME)
		set(STAGED_NAME "${TARGET_NAME}")
	endif ()

	set(BYPRODUCT "")
	if (TARGET_TYPE STREQUAL "EXECUTABLE")
		if (IS_BUNDLE)
			set(BYPRODUCT "${DEST}/${STAGED_NAME}.app/Contents/MacOS/${STAGED_NAME}")
		else ()
			set(BYPRODUCT "${DEST}/${STAGED_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
		endif ()
	endif ()

	if (IS_BUNDLE)
		# copy_directory alone would leave behind files deleted since the last build (a stale
		# shader, a renamed resource), so the previous copy is removed first.
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E rm -rf
				"${DEST}/$<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}>"
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"$<TARGET_BUNDLE_DIR:${TARGET_NAME}>"
				"${DEST}/$<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}>"
			BYPRODUCTS ${BYPRODUCT}
			COMMENT "Staging $<TARGET_BUNDLE_DIR_NAME:${TARGET_NAME}> to ${DEST}"
			VERBATIM
		)
	else ()
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST}"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:${TARGET_NAME}>"
				"${DEST}/$<TARGET_FILE_NAME:${TARGET_NAME}>"
			BYPRODUCTS ${BYPRODUCT}
			COMMENT "Staging $<TARGET_FILE_NAME:${TARGET_NAME}> to ${DEST}"
			VERBATIM
		)
	endif ()
endfunction()

# Decides where a CE module SHARED library is built, and whether it is staged to Binaries/ at all.
#
# Windows/Linux: built into <build>/Out/<config>/DLL/ and staged to
# Binaries/<toolchain>/<config>/DLL/, the shared folder every executable there is taught to search
# (ce_enable_dll_bootstrap / ce_set_dll_search_rpath).
#
# macOS: built into <build>/Modules/<config>/ and NOT staged. Nothing on macOS loads them from a
# shared folder - CelestialEngine.app embeds its own copies into Contents/Frameworks
# (ce_bundle_apple_frameworks) and each Tests/ executable gets its own copies beside it
# (ce_embed_apple_modules_beside) - so a copy under Binaries/ would be pure clutter: that is exactly
# why libCE_*.dylib used to pile up next to CelestialEngine.app.
# Usage: ce_set_module_library_output(target_name)
function(ce_set_module_library_output TARGET_NAME)
	if (APPLE)
		set_target_properties(${TARGET_NAME} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/$<CONFIG>"
			LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/$<CONFIG>"
			ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/$<CONFIG>"
		)
	else ()
		ce_set_output_subdirectory(${TARGET_NAME} DLL)
		ce_stage_to_binaries(${TARGET_NAME} DLL)
	endif ()
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

# On macOS, makes sure every CE_APPLE_FRAMEWORK_MODULE_NAMES library is copied (and ad-hoc
# re-signed, same reasoning as ce_bundle_apple_frameworks) into DEST_DIR. Each copied library
# resolves its own sibling dependencies via its own @loader_path RPATH (see
# ce_configure_module_library_rpath), so once they are all sitting flat in one folder an executable
# there needs no further help finding them.
#
# The work is done by ONE shared custom target per destination folder, which callers then depend
# on, instead of POST_BUILD steps on each executable: all seven test executables share the same
# folder, so per-target POST_BUILD steps had them copying and codesigning the SAME seven .dylib
# files concurrently under a parallel build - one job replacing a file while another was signing
# it, which failed the build with "object file format unrecognized, invalid, or unsuitable" or
# "No such file or directory". A single target does the work once, serialised. Returns the target's
# name in OUT_TARGET.
# Usage: ce_apple_modules_copy_target("<dest dir>" OUT_VAR)
function(ce_apple_modules_copy_target DEST_DIR OUT_TARGET)
	# One copy target per destination folder, shared by every executable built into it. The name is
	# derived from the (possibly $<CONFIG>-dependent) path so two different folders cannot collide.
	string(MAKE_C_IDENTIFIER "${DEST_DIR}" DEST_ID)
	set(COPY_TARGET "CE_AppleModules_${DEST_ID}")

	if (NOT TARGET ${COPY_TARGET})
		add_custom_target(${COPY_TARGET}
			COMMENT "Embedding and signing the CE module libraries in ${DEST_DIR}"
		)

		add_dependencies(${COPY_TARGET} ${CE_APPLE_FRAMEWORK_MODULE_NAMES})

		foreach (MODULE_LIB ${CE_APPLE_FRAMEWORK_MODULE_NAMES})
			add_custom_command(TARGET ${COPY_TARGET} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"$<TARGET_FILE:${MODULE_LIB}>"
					"${DEST_DIR}/"
				COMMAND codesign --force --sign -
					"${DEST_DIR}/$<TARGET_FILE_NAME:${MODULE_LIB}>"
				VERBATIM
			)
		endforeach ()
	endif ()

	set(${OUT_TARGET} "${COPY_TARGET}" PARENT_SCOPE)
endfunction()

# On macOS, puts a copy of every module library beside TARGET_NAME - a plain (non-bundle)
# executable, such as one of the Tests/ binaries - in BOTH places it exists: its build-tree output
# folder (where CTest runs it from) and the staged Binaries/<toolchain>/<config>/<SUBDIR>/ copy.
# The module libraries are not built under Binaries/ at all on macOS (see
# ce_set_module_library_output), and there is no shared folder to point an RPATH at, so each
# executable's own folder has to hold them.
#
# SUBDIR must be the same subfolder passed to ce_set_output_subdirectory / ce_stage_to_binaries.
# No-op elsewhere - Windows and Linux use ce_enable_dll_bootstrap / ce_set_dll_search_rpath against
# the shared DLL/ folder instead.
# Usage: ce_embed_apple_modules_beside(target_name [subdir])
function(ce_embed_apple_modules_beside TARGET_NAME)
	if (NOT APPLE)
		return()
	endif ()

	set(SUBDIR "")
	if (ARGC GREATER 1)
		set(SUBDIR "${ARGV1}")
	endif ()

	if (SUBDIR STREQUAL "")
		set(BUILD_DIR "${CE_BUILD_OUTPUT_DIR}")
		set(STAGED_DIR "${CE_BINARIES_DIR}")
	else ()
		set(BUILD_DIR "${CE_BUILD_OUTPUT_DIR}/${SUBDIR}")
		set(STAGED_DIR "${CE_BINARIES_DIR}/${SUBDIR}")
	endif ()

	ce_apple_modules_copy_target("${BUILD_DIR}" BUILD_COPY_TARGET)
	ce_apple_modules_copy_target("${STAGED_DIR}" STAGED_COPY_TARGET)

	add_dependencies(${TARGET_NAME} ${BUILD_COPY_TARGET} ${STAGED_COPY_TARGET})
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
