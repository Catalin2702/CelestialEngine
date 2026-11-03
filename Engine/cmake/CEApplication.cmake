#
# Module: CelestialEngine/Engine/cmake
# File: CEApplication.cmake
# Created by: Catalin Chirosca
# Created: 2026-11-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-03
#

# ce_configure_application(<target>
#     [OUTPUT_NAME <name>]               executable / bundle name, Contents/MacOS/<name> (default: <target>)
#     [BUNDLE_NAME <name>]               CFBundleName, what Finder and the menu bar show (default: OUTPUT_NAME)
#     [BUNDLE_ID <reverse.dns.id>]       CFBundleIdentifier (default: com.celestialengine.<output name, lowercase>)
#     [ASSET_CATALOG <dir.xcassets>]     compiled with actool; must hold an "AppIcon" set (default: none)
#     [INFO_PLIST <template.plist.in>]   default: templates/Info.plist.in next to this file
#     [ENTITLEMENTS <file.plist>]        default: templates/Entitlements.plist next to this file
# )
#
# Turns an executable the application already created into one built on the engine. The application keeps the plain
# CMake part - add_executable() with its sources, target_include_directories(), any target_* of its own - so IDEs see
# a standard target they can edit (CLion's "add to target" works on add_executable, not on a custom function's
# arguments). This call adds what every engine executable needs, so a sandbox, an editor and a game differ only in what
# they pass here and in their own sources:
#
#   add_executable(MyGame src/source/Main.cpp ...)
#   target_include_directories(MyGame PRIVATE src/include)
#   ce_configure_application(MyGame OUTPUT_NAME "My Game" ...)
#
#   - links CE_Engine (and CE_Config);
#   - macOS: makes the target a MACOSX_BUNDLE - a self-contained .app with Info.plist (two variants, Dist without the
#     Metal tooling keys), asset catalog, embedded module dylibs in Contents/Frameworks, ad-hoc signature with the
#     entitlements;
#   - bundles the shaders of every compiled-in backend (CE_RENDER_BACKENDS): as bundle resources on macOS, copied into
#     Resources/Shaders/<API>/ next to the executable elsewhere;
#   - Windows/Linux: DLL/ search path for the module libraries;
#   - delivery to Binaries/<toolchain>/<config>/ and, with CE_COPY_LAST, to Binaries/Last/.
#
# Relative paths are taken from the calling CMakeLists.txt's directory. Requires CelestialEngineMacros.cmake (which
# includes this file) and the engine targets to exist.

set(CE_APPLICATION_TEMPLATES_DIR "${CMAKE_CURRENT_LIST_DIR}/templates")

function(ce_configure_application target)
	cmake_parse_arguments(PARSE_ARGV 1 ARG
		""
		"OUTPUT_NAME;BUNDLE_NAME;BUNDLE_ID;ASSET_CATALOG;INFO_PLIST;ENTITLEMENTS"
		""
	)

	if (ARG_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "ce_configure_application(${target}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
	endif ()
	if (NOT TARGET ${target})
		message(FATAL_ERROR "ce_configure_application(${target}): no such target - create it first with add_executable()")
	endif ()
	get_target_property(_ce_target_type ${target} TYPE)
	if (NOT _ce_target_type STREQUAL "EXECUTABLE")
		message(FATAL_ERROR "ce_configure_application(${target}): is a ${_ce_target_type}, expected an add_executable() target")
	endif ()
	# Source file properties (MACOSX_PACKAGE_LOCATION on the shaders, below) are scoped to a directory: they are set in
	# the calling directory, so the target has to live there too or the bundle silently loses its shaders.
	get_target_property(_ce_target_dir ${target} SOURCE_DIR)
	if (NOT _ce_target_dir STREQUAL CMAKE_CURRENT_SOURCE_DIR)
		message(FATAL_ERROR "ce_configure_application(${target}): call it from the CMakeLists.txt that creates the target (${_ce_target_dir})")
	endif ()

	# The name of the produced binary (Contents/MacOS/<name> inside the bundle). Kept in ONE place because Info.plist's
	# CFBundleExecutable has to match it exactly: when it does not, Launch Services cannot find the executable the plist
	# advertises and Finder refuses to open the app with "you can't open the application because it may be damaged or
	# incomplete" - even though the bundle is correctly signed and running Contents/MacOS/<name> from a terminal works.
	if (NOT ARG_OUTPUT_NAME)
		set(ARG_OUTPUT_NAME "${target}")
	endif ()
	if (NOT ARG_BUNDLE_NAME)
		set(ARG_BUNDLE_NAME "${ARG_OUTPUT_NAME}")
	endif ()
	if (NOT ARG_BUNDLE_ID)
		string(TOLOWER "${ARG_OUTPUT_NAME}" _ce_lower_name)
		string(REGEX REPLACE "[^a-z0-9.-]" "-" _ce_lower_name "${_ce_lower_name}")
		set(ARG_BUNDLE_ID "com.celestialengine.${_ce_lower_name}")
	endif ()
	if (NOT ARG_INFO_PLIST)
		set(ARG_INFO_PLIST "${CE_APPLICATION_TEMPLATES_DIR}/Info.plist.in")
	endif ()
	if (NOT ARG_ENTITLEMENTS)
		set(ARG_ENTITLEMENTS "${CE_APPLICATION_TEMPLATES_DIR}/Entitlements.plist")
	endif ()

	foreach (_ce_path_arg IN ITEMS ASSET_CATALOG INFO_PLIST ENTITLEMENTS)
		if (ARG_${_ce_path_arg})
			get_filename_component(ARG_${_ce_path_arg} "${ARG_${_ce_path_arg}}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
		endif ()
	endforeach ()

	# The shaders of every compiled-in graphics backend (CE_RENDER_BACKENDS), and the targets that produce them. Selected
	# by the CE_RENDER_* switches rather than by whether the variables are set: those are cache variables, and a
	# re-configure that drops a backend would otherwise still find the previous selection's value in them.
	set(_ce_shader_resources "")
	set(_ce_shader_targets "")

	if (CE_RENDER_METAL)
		# Shaders are listed as bundle resources instead of being copied in POST_BUILD steps: that way they are real
		# build dependencies of the application, so editing a shader re-copies it into the bundle without a relink.
		set_source_files_properties(${CE_METAL_SHADERS_LIB_PATH} PROPERTIES
			GENERATED TRUE
			MACOSX_PACKAGE_LOCATION "Resources/Shaders/Metal"
		)

		list(APPEND _ce_shader_resources ${CE_METAL_SHADERS_LIB_PATH})
		list(APPEND _ce_shader_targets CE_MetalShaders)
	endif ()

	if (CE_RENDER_OPENGL)
		# GLSL is loaded and compiled at runtime, so the .glsl files are bundled as-is.
		if (APPLE)
			set_source_files_properties(${CE_OPENGL_SHADERS_SOURCES} PROPERTIES
				MACOSX_PACKAGE_LOCATION "Resources/Shaders/OpenGL"
			)
		endif ()

		list(APPEND _ce_shader_resources ${CE_OPENGL_SHADERS_SOURCES})
		list(APPEND _ce_shader_targets CE_OpenGLShaders)
	endif ()

	# On macOS the executable becomes a bundle, and the shaders become its resources (MACOSX_PACKAGE_LOCATION above).
	# MACOSX_BUNDLE is a target property, so setting it after add_executable() is the same as passing it there.
	if (APPLE)
		set_target_properties(${target} PROPERTIES
			MACOSX_BUNDLE TRUE
		)
		target_sources(${target} PRIVATE
			${_ce_shader_resources}
		)
	endif ()

	add_dependencies(${target} ${_ce_shader_targets})

	# Treating the shaders as link dependencies forces the application to relink when they change, which is what
	# re-runs the POST_BUILD steps below (code signing, the non-Apple copy into Resources/, the copy to Binaries/Last).
	# Without it the freshly built shaders would land in the bundle but never reach Binaries/Last, and the ad-hoc
	# signature would still seal the previous ones.
	set_property(TARGET ${target} APPEND PROPERTY
		LINK_DEPENDS ${_ce_shader_resources}
	)

	if (NOT APPLE)
		# A plain executable has no bundle to carry its resources, so they are copied next to it, mirroring the macOS
		# bundle's Resources/Shaders/<API> layout (see the platform-specific relative path chosen in FileSystem).
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/Resources/Shaders"
			COMMENT "Creating Resources/Shaders"
		)

		if (CE_RENDER_OPENGL)
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/Resources/Shaders/OpenGL"
				COMMENT "Creating Resources/Shaders/OpenGL"
			)

			foreach (_ce_shader_file ${CE_OPENGL_SHADERS_SOURCES})
				add_custom_command(TARGET ${target} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
						"${_ce_shader_file}"
						"$<TARGET_FILE_DIR:${target}>/Resources/Shaders/OpenGL/"
					COMMENT "Copying ${_ce_shader_file} to Resources/Shaders/OpenGL"
				)
			endforeach ()
		endif ()
	endif ()

	set_target_properties(${target} PROPERTIES
		LINKER_LANGUAGE CXX
		OUTPUT_NAME "${ARG_OUTPUT_NAME}"
	)

	# Windows: delay-load the module DLLs and register DLL/ as their search directory.
	ce_enable_dll_bootstrap(${target})

	if (APPLE)
		# macOS ships the application as a self-contained .app bundle: every module .dylib is embedded into
		# Contents/Frameworks/ instead of pointing at an external DLL/ folder, so the bundle stays portable.
		ce_bundle_apple_frameworks(${target})
	else ()
		# Linux: embed an $ORIGIN-relative RPATH pointing at DLL/.
		ce_set_dll_search_rpath(${target} "DLL")
	endif ()

	if (APPLE)
		# Values the Info.plist template reads (@CE_APP_...@ / @PROJECT_VERSION@ - PROJECT_VERSION is the calling
		# project's). Configured once, into the build tree, and THAT is the bundle's Info.plist source everywhere below.
		set(CE_APP_OUTPUT_NAME "${ARG_OUTPUT_NAME}")
		set(CE_APP_BUNDLE_NAME "${ARG_BUNDLE_NAME}")
		set(CE_APP_BUNDLE_ID "${ARG_BUNDLE_ID}")

		# Two of them, because the Metal tooling keys are gated per configuration and configure_file runs ONCE for all
		# three under a multi-config generator - it cannot see $<CONFIG>. Writing the genex into the plist instead is
		# not an option either: a generator expression ends at the first '>', which every XML tag in the fragment
		# contains. So the variant is chosen at build time, by the POST_BUILD copy below, out of two files configured here.
		set(CE_APP_METAL_TOOLING_KEYS
"	<key>MetalHudEnabled</key>
	<true/>
	<key>MetalCaptureEnabled</key>
	<true/>")

		set(_ce_info_plist "${CMAKE_CURRENT_BINARY_DIR}/${target}.Info.plist")
		configure_file("${ARG_INFO_PLIST}" "${_ce_info_plist}" @ONLY)

		# Dist ships without them: MetalHudEnabled alone maps libMTLHud.dylib into every user's process, idle but
		# present, and MetalCaptureEnabled opens the app to GPU capture.
		set(CE_APP_METAL_TOOLING_KEYS "	<!-- Metal HUD and GPU capture are Debug/Release only. -->")

		set(_ce_info_plist_dist "${CMAKE_CURRENT_BINARY_DIR}/${target}.Info.Dist.plist")
		configure_file("${ARG_INFO_PLIST}" "${_ce_info_plist_dist}" @ONLY)

		set_target_properties(${target} PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "${ARG_BUNDLE_NAME}"
			MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
			MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
			MACOSX_BUNDLE_EXECUTABLE_NAME "${ARG_OUTPUT_NAME}"
			MACOSX_BUNDLE_GUI_IDENTIFIER "${ARG_BUNDLE_ID}"
			MACOSX_BUNDLE_INFO_PLIST "${_ce_info_plist}"
		)

		# Same reasoning as the shaders above: the plists are only ever installed by a POST_BUILD step, and POST_BUILD
		# steps run only when the target relinks. Without this, editing the template reconfigured and rewrote the file
		# in the build tree but left the .app holding the previous one until something unrelated forced a relink.
		set_property(TARGET ${target} APPEND PROPERTY
			LINK_DEPENDS "${_ce_info_plist}" "${_ce_info_plist_dist}"
		)

		# Copies the CONFIGURED plist, not the raw template: copying the template verbatim would overwrite the
		# generated one with a file still full of @PLACEHOLDERS@.
		#
		# This is also where the per-configuration choice is made. MACOSX_BUNDLE_INFO_PLIST above takes no generator
		# expression - it is consumed when the bundle skeleton is generated, before any configuration is picked - so the
		# skeleton always gets the Debug/Release plist and this step, which runs before the signing step below, puts the
		# right one in place.
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<IF:$<CONFIG:Dist>,${_ce_info_plist_dist},${_ce_info_plist}>"
				"$<TARGET_BUNDLE_CONTENT_DIR:${target}>/Info.plist"
			COMMENT "Copying Info.plist to the ${target} bundle"
		)

		if (ARG_ASSET_CATALOG)
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_CONTENT_DIR:${target}>/Resources"
				COMMAND xcrun actool "${ARG_ASSET_CATALOG}"
					--compile "$<TARGET_BUNDLE_CONTENT_DIR:${target}>/Resources"
					--platform macosx
					--minimum-deployment-target 10.15
					--app-icon AppIcon
					--output-partial-info-plist "${CMAKE_CURRENT_BINARY_DIR}/${target}.AppIcon-Info.plist"
				COMMENT "Compiling the ${target} asset catalog"
			)
		endif ()

		# Running the app leaves ImGui's imgui.ini next to the executable (its CWD is Contents/MacOS). A non-executable
		# file in the bundle root makes the next codesign fail ("unsealed contents present in the bundle root"), so the
		# leftovers are swept before signing - it is user state, nothing that belongs in a signed bundle. Registered
		# before the signing step on purpose.
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E rm -f
				"$<TARGET_BUNDLE_CONTENT_DIR:${target}>/MacOS/imgui.ini"
				"$<TARGET_BUNDLE_CONTENT_DIR:${target}>/Resources/imgui.ini"
			COMMENT "Removing stray imgui.ini from the ${target} bundle"
			VERBATIM
		)

		# Ad-hoc signature with the entitlements. Deliberately NOT --deep: it resigns nested content (Contents/Frameworks)
		# in an unspecified order and does not reliably produce a valid seal - Apple's own codesign docs call it
		# deprecated for exactly this reason. The embedded module libraries are already signed individually as they are
		# copied in (see ce_bundle_apple_frameworks, registered - and so run - before this step), so a plain, non-deep
		# sign here only needs to seal the outer bundle around already-valid nested code.
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND codesign --force --sign -
				--entitlements "${ARG_ENTITLEMENTS}"
				"$<TARGET_BUNDLE_DIR:${target}>"
			COMMENT "Code signing ${target} with entitlements"
		)
	endif ()

	target_link_libraries(${target} PRIVATE
		CE_Config
		CE_Engine
	)

	# Deliver the finished artifact to Binaries/<toolchain>/<config>/. Registered LAST on purpose: it is a POST_BUILD
	# copy, and POST_BUILD steps run in registration order, so everything that fills the bundle (Info.plist, asset
	# catalog, embedded frameworks) and finally signs it has already run.
	ce_stage_to_binaries(${target})

	if (NOT APPLE)
		# The bundle carries its Resources/ along; a plain executable does not, so the folder the POST_BUILD steps above
		# filled in the build tree is staged alongside the executable.
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"$<TARGET_FILE_DIR:${target}>/Resources"
				"${CE_BINARIES_DIR}/Resources"
			COMMENT "Staging Resources to ${CE_BINARIES_DIR}"
			VERBATIM
		)
	endif ()

	# Copy to Binaries/Last folder (no-op unless CE_COPY_LAST is enabled)
	ce_copy_to_last(${target})
endfunction()
