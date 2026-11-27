#
# Module: CelestialEngine/Engine/CMake
# File: CEApplication.cmake
# Created by: Catalin Chirosca
# Created: 2026-11-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-27
#

# ce_configure_application(<target>
#     [OUTPUT_NAME <name>]               executable / bundle name, Contents/MacOS/<name> (default: <target>)
#     [BUNDLE_NAME <name>]               CFBundleName, what Finder and the menu bar show (default: OUTPUT_NAME)
#     [BUNDLE_ID <reverse.dns.id>]       CFBundleIdentifier (default: com.celestialengine.<output name, lowercase>)
#     [ASSET_CATALOG <dir.xcassets>]     compiled with actool; must hold an "AppIcon" set (default: none)
#     [INFO_PLIST <template.plist.in>]   default: templates/Info.plist.in next to this file
#     [ENTITLEMENTS <file.plist>]        default: templates/Entitlements.plist next to this file
#     [CONTENT <dir>]                    the application's own files, shipped in Resources/Content (resources://)
#     [NO_ENTRY_POINT]                   the application defines its own main(); EntryPoint.cpp is not added
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
#   - adds the engine's main() (Engine/Source/Launch/source/EntryPoint.cpp) to the executable's sources, unless NO_ENTRY_POINT:
#     the application only defines CE::CreateApplicationClient();
#   - macOS: makes the target a MACOSX_BUNDLE - a self-contained .app with Info.plist (two variants, Dist without the
#     Metal tooling keys), asset catalog, embedded module dylibs in Contents/Frameworks, ad-hoc signature with the
#     entitlements;
#   - bundles the shaders of every compiled-in backend (CE_RENDER_BACKENDS): as bundle resources on macOS, copied into
#     Resources/Engine/Shaders/<API>/ next to the executable elsewhere - Resources/Engine is the engine's, mounted as
#     engine://, and Resources/Content the application's, mounted as resources:// (IO::FileSystem::Initialize);
#   - CONTENT: copies every file of <dir> (subfolders kept, hidden files such as .DS_Store skipped) into
#     Resources/Content, which FileSystem mounts as resources:// - bundle resources on macOS, a copy next to the
#     executable elsewhere;
#   - defines CE_APPLICATION_NAME (the OUTPUT_NAME) for the executable's sources: EntryPoint.cpp hands it to the file
#     system, which keeps this application's user data (config://) in a folder of that name; and
#     CE_APPLICATION_DISPLAY_NAME (the BUNDLE_NAME), which EntryPoint.cpp makes the window's title;
#   - Windows/Linux: DLL/ search path for the module libraries;
#   - the result stays where it is built, <build>/Out/<config>/ (no copy elsewhere; distribution is a separate step).
#
# Relative paths are taken from the calling CMakeLists.txt's directory. Requires CelestialEngineMacros.cmake (which
# includes this file) and the engine targets to exist.

set(CE_APPLICATION_TEMPLATES_DIR "${CMAKE_CURRENT_LIST_DIR}/templates")

# ce_collect_application_content(<dir> <out_var>)
#
# Lists the files of an application's content folder, as paths relative to it, sorted. Hidden files and folders -
# any path component starting with '.' - are left out: .DS_Store, which Finder drops into every folder it opens, would
# otherwise be shipped and sealed into the signed bundle. A function of its own so the rule can be tested without
# configuring an application (Engine/Tests/CMake).
function(ce_collect_application_content dir out_var)
	# LIST_DIRECTORIES false: only files, the folders are recreated from their paths. CONFIGURE_DEPENDS: a file added to
	# or removed from the folder re-runs the configure step at the next build, so the list never goes stale. It only
	# exists while configuring a project - a script run with cmake -P (the test of this function) has no build to
	# re-run - so it is left out there.
	set(_ce_glob_options LIST_DIRECTORIES false RELATIVE "${dir}")
	if (NOT CMAKE_SCRIPT_MODE_FILE)
		list(APPEND _ce_glob_options CONFIGURE_DEPENDS)
	endif ()

	file(GLOB_RECURSE _ce_content_files ${_ce_glob_options} "${dir}/*")

	set(_ce_kept "")

	foreach (_ce_file IN LISTS _ce_content_files)
		if (NOT _ce_file MATCHES "(^|/)\\.")
			list(APPEND _ce_kept "${_ce_file}")
		endif ()
	endforeach ()

	list(SORT _ce_kept)
	set(${out_var} "${_ce_kept}" PARENT_SCOPE)
endfunction()

function(ce_configure_application target)
	cmake_parse_arguments(PARSE_ARGV 1 ARG
		"NO_ENTRY_POINT"
		"OUTPUT_NAME;BUNDLE_NAME;BUNDLE_ID;ASSET_CATALOG;INFO_PLIST;ENTITLEMENTS;CONTENT"
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

	foreach (_ce_path_arg IN ITEMS ASSET_CATALOG INFO_PLIST ENTITLEMENTS CONTENT)
		if (ARG_${_ce_path_arg})
			get_filename_component(ARG_${_ce_path_arg} "${ARG_${_ce_path_arg}}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
		endif ()
	endforeach ()

	# The application's content: listed now, while the relative paths are known, and installed below with the shaders.
	set(_ce_content_files "")
	if (ARG_CONTENT)
		if (NOT IS_DIRECTORY "${ARG_CONTENT}")
			message(FATAL_ERROR "ce_configure_application(${target}): CONTENT '${ARG_CONTENT}' is not a directory")
		endif ()
		ce_collect_application_content("${ARG_CONTENT}" _ce_content_files)
	endif ()

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
			MACOSX_PACKAGE_LOCATION "Resources/Engine/Shaders/Metal"
		)

		list(APPEND _ce_shader_resources ${CE_METAL_SHADERS_LIB_PATH})
		list(APPEND _ce_shader_targets CE_MetalShaders)
	endif ()

	if (CE_RENDER_OPENGL)
		# GLSL is compiled by the driver at run time, so the .glsl files CE_ShaderCompiler generated are bundled as they
		# are. GENERATED, like the Metal library: they do not exist until the build has run the shader compiler.
		if (APPLE)
			set_source_files_properties(${CE_OPENGL_SHADERS_SOURCES} PROPERTIES
				GENERATED TRUE
				MACOSX_PACKAGE_LOCATION "Resources/Engine/Shaders/OpenGL"
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

		# Each content file becomes a bundle resource at its own subfolder of Resources/Content. HEADER_FILE_ONLY so a
		# .c or .metal among them is shipped as data, never compiled as one of the application's sources.
		foreach (_ce_file IN LISTS _ce_content_files)
			get_filename_component(_ce_subdir "${_ce_files}" DIRECTORY)
			set(_ce_location "Resources/Content")
			if (_ce_subdir)
				string(APPEND _ce_location "${_ce_subdir}")
			endif ()

			set_source_files_properties("${ARG_CONTENT}/${_ce_file}" PROPERTIES
				MACOSX_PACKAGE_LOCATION "${_ce_location}"
				HEADER_FILE_ONLY TRUE
			)
			target_sources(${target} PRIVATE
				"${ARG_CONTENT}/${_ce_file}"
			)
		endforeach ()
	endif ()

	add_dependencies(${target} ${_ce_shader_targets})

	# Treating the shaders as link dependencies forces the application to relink when they change, which is what
	# re-runs the POST_BUILD steps below (code signing, the non-Apple copy into Resources/). Without it the freshly
	# built shaders would land in the bundle while the ad-hoc signature still sealed the previous ones.
	set_property(TARGET ${target} APPEND PROPERTY
		LINK_DEPENDS ${_ce_shader_resources}
	)

	# Same for the content: an edited file must reach the bundle and be sealed by a new signature.
	foreach (_ce_file IN LISTS _ce_content_files)
		set_property(TARGET ${target} APPEND PROPERTY
			LINK_DEPENDS "${ARG_CONTENT}/${_ce_file}"
		)
	endforeach ()

	if (NOT APPLE)
		# A plain executable has no bundle to carry its resources, so they are copied next to it, mirroring the macOS
		# bundle's Resources/Shaders/<API> layout (see the platform-specific relative path chosen in FileSystem).
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/Resources/Engine/Shaders"
			COMMENT "Creating Resources/Shaders"
		)

		if (CE_RENDER_OPENGL)
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/Resources/Engine/Shaders/OpenGL"
				COMMENT "Creating Resources/Shaders/OpenGL"
			)

			foreach (_ce_shader_file ${CE_OPENGL_SHADERS_SOURCES})
				add_custom_command(TARGET ${target} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
						"${_ce_shader_file}"
						"$<TARGET_FILE_DIR:${target}>/Resources/Engine/Shaders/OpenGL/"
					COMMENT "Copying ${_ce_shader_file} to Resources/Engine/Shaders/OpenGL"
				)
			endforeach ()
		endif ()

		# The content mirrors the bundle's Resources/Content, subfolders included.
		foreach (_ce_file IN LISTS _ce_content_files)
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"${ARG_CONTENT}/${_ce_file}"
					"$<TARGET_FILE_DIR:${target}>/Resources/Content/${_ce_file}"
				COMMENT "Copying ${_ce_file} to Resources/Content"
			)
		endforeach ()
	endif ()

	set_target_properties(${target} PROPERTIES
		LINKER_LANGUAGE CXX
		OUTPUT_NAME "${ARG_OUTPUT_NAME}"
	)

	# The application's name as the file system needs it: the folder its user data goes in (config://). PRIVATE, so it
	# reaches the executable's own sources - EntryPoint.cpp among them - and never the engine libraries, which serve
	# every application at once. It ends up as a folder name, so it must stay one path segment.
	if (ARG_OUTPUT_NAME MATCHES "[/\\:]")
		message(FATAL_ERROR "ce_configure_application(${target}): OUTPUT_NAME '${ARG_OUTPUT_NAME}' cannot contain '/', '\\' or ':'")
	endif ()

	# The display name (BUNDLE_NAME, what Finder and the menu bar show) is also the window's title: EntryPoint.cpp
	# hands it to InitFromCommandLine. A '"' or '\' would end or escape the string literal it becomes.
	if (ARG_BUNDLE_NAME MATCHES "[\"\\\\]")
		message(FATAL_ERROR "ce_configure_application(${target}): BUNDLE_NAME '${ARG_BUNDLE_NAME}' cannot contain '\"' or '\\'")
	endif ()

	target_compile_definitions(${target} PRIVATE
		CE_APPLICATION_NAME="${ARG_OUTPUT_NAME}"
		CE_APPLICATION_DISPLAY_NAME="${ARG_BUNDLE_NAME}"
	)

	# main() belongs in the executable - macOS takes the entry point from the main binary only, never from a dylib - so
	# it is compiled here, as one of the application's own sources, and not into an engine library. That also hands it
	# this application's CE_APPLICATION_NAME, which a library compiled once for every application could not have.
	if (NOT ARG_NO_ENTRY_POINT)
		target_sources(${target} PRIVATE
			"${CE_ENGINE_ROOT_DIR}/Source/Launch/source/EntryPoint.cpp"
		)
	endif ()

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
		# project's - and @CMAKE_OSX_DEPLOYMENT_TARGET@, the minimum macOS of the top-level CMakeLists.txt). Configured once, into the build tree, and THAT is the bundle's Info.plist source everywhere below.
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
					--minimum-deployment-target "${CMAKE_OSX_DEPLOYMENT_TARGET}"
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
endfunction()
