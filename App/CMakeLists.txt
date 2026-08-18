#
# Module: CelestialEngine/App
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

cmake_minimum_required(VERSION 4.1)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_App LANGUAGES CXX VERSION 0.1.3)

# The name of the produced binary (Contents/MacOS/<name> inside the bundle). Kept in ONE place
# because Info.plist's CFBundleExecutable has to match it exactly: when it does not, Launch
# Services cannot find the executable the plist advertises and Finder refuses to open the app with
# "you can't open the application because it may be damaged or incomplete" - even though the
# bundle is correctly signed and running Contents/MacOS/<name> from a terminal works fine.
set(CE_APP_OUTPUT_NAME "CelestialEngine")

set(SOURCES
	src/App.cpp
)

if (APPLE)
	# Shaders are listed as bundle resources instead of being copied in POST_BUILD
	# steps: that way they are real build dependencies of CE_App, so editing a
	# shader re-copies it into the bundle without needing a relink.
	set_source_files_properties(${CE_METAL_SHADERS_LIB_PATH} PROPERTIES
		GENERATED TRUE
		MACOSX_PACKAGE_LOCATION "Resources/Shaders/Metal"
	)

	# GLSL is loaded and compiled at runtime, so the .glsl files are bundled as-is.
	set_source_files_properties(${CE_OPENGL_SHADERS_SOURCES} PROPERTIES
		MACOSX_PACKAGE_LOCATION "Resources/Shaders/OpenGL"
	)

	add_executable(CE_App MACOSX_BUNDLE
		${SOURCES}
		${CE_METAL_SHADERS_LIB_PATH}
		${CE_OPENGL_SHADERS_SOURCES}
	)

	add_dependencies(CE_App CE_MetalShaders CE_OpenGLShaders)

	# Treating the shaders as link dependencies forces CE_App to relink when they
	# change, which is what re-runs the POST_BUILD steps below (code signing and the
	# copy to Binaries/Last). Without it the freshly built shaders would land in the
	# bundle but never reach Binaries/Last, and the ad-hoc signature would still
	# seal the previous ones.
	set_property(TARGET CE_App APPEND PROPERTY
		LINK_DEPENDS ${CE_METAL_SHADERS_LIB_PATH} ${CE_OPENGL_SHADERS_SOURCES}
	)
else ()
	add_executable(CE_App
		${SOURCES}
	)

	add_dependencies(CE_App CE_OpenGLShaders)

	# Mirrors the macOS branch above: treating the shaders as link dependencies forces CE_App to
	# relink (and thus re-run the POST_BUILD copy below) whenever a .glsl file changes, instead of
	# only picking up the change on the next full reconfigure.
	set_property(TARGET CE_App APPEND PROPERTY
		LINK_DEPENDS ${CE_OPENGL_SHADERS_SOURCES}
	)

	# GLSL is loaded and compiled at runtime, so the .glsl files are copied next to the executable
	# as-is, mirroring the macOS bundle's Resources/Shaders/OpenGL layout (see the platform-specific
	# relative path chosen in GlfwApplication.cpp).
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:CE_App>/Resources/Shaders/OpenGL"
		COMMENT "Creating Resources/Shaders/OpenGL"
	)

	foreach (SHADER_FILE ${CE_OPENGL_SHADERS_SOURCES})
		add_custom_command(TARGET CE_App POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${SHADER_FILE}"
				"$<TARGET_FILE_DIR:CE_App>/Resources/Shaders/OpenGL/"
			COMMENT "Copying ${SHADER_FILE} to Resources/Shaders/OpenGL"
		)
	endforeach ()
endif()

set_target_properties(CE_App PROPERTIES
	LINKER_LANGUAGE CXX
	OUTPUT_NAME "${CE_APP_OUTPUT_NAME}"
)

# Windows: delay-load the module DLLs and register DLL/ as their search directory.
ce_enable_dll_bootstrap(CE_App)

if (APPLE)
	# macOS ships CE_App as a self-contained .app bundle: embed every module .dylib into
	# Contents/Frameworks/ instead of pointing at the external DLL/ folder, so the bundle stays
	# portable if copied/moved elsewhere.
	ce_bundle_apple_frameworks(CE_App)
else ()
	# Linux: embed an $ORIGIN-relative RPATH pointing at DLL/.
	ce_set_dll_search_rpath(CE_App "DLL")
endif ()

# Create macOS bundle to avoid system warnings
if(APPLE)
	# Set the Asset Catalog for the app icon
	set(ASSET_CATALOG "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Assets.xcassets")

	# Resolve @CE_APP_OUTPUT_NAME@ / @PROJECT_VERSION@ once, into the build tree, and use THAT as
	# the bundle's Info.plist source everywhere below.
	set(CE_APP_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in" "${CE_APP_INFO_PLIST}" @ONLY)

	# Set bundle properties
	set_target_properties(CE_App PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "CelestialEngine App"
		MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
		MACOSX_BUNDLE_EXECUTABLE_NAME "${CE_APP_OUTPUT_NAME}"
		MACOSX_BUNDLE_GUI_IDENTIFIER "com.celestialengine.app"
		MACOSX_BUNDLE_INFO_PLIST "${CE_APP_INFO_PLIST}"
	)

	# Copy Info.plist to bundle (fallback if MACOSX_BUNDLE_INFO_PLIST doesn't work). It copies the
	# CONFIGURED plist, not the raw template: copying Info.plist.in verbatim would overwrite the
	# generated one with a file still full of @PLACEHOLDERS@.
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CE_APP_INFO_PLIST}"
			"$<TARGET_BUNDLE_CONTENT_DIR:CE_App>/Info.plist"
		COMMENT "Copying Info.plist to bundle"
	)

	# Compile Asset Catalog and copy to bundle
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_CONTENT_DIR:CE_App>/Resources"
		COMMAND xcrun actool "${ASSET_CATALOG}"
			--compile "$<TARGET_BUNDLE_CONTENT_DIR:CE_App>/Resources"
			--platform macosx
			--minimum-deployment-target 10.15
			--app-icon AppIcon
			--output-partial-info-plist "${CMAKE_CURRENT_BINARY_DIR}/AppIcon-Info.plist"
		COMMENT "Compiling Asset Catalog"
	)

	# Sign the app with ad-hoc signature and entitlements (suppresses warnings). Deliberately NOT
	# --deep: it resigns nested content (Contents/Frameworks) in an unspecified order and does not
	# reliably produce a valid seal - Apple's own codesign docs call it deprecated for exactly this
	# reason. The embedded module libraries are already signed individually as they're copied in
	# (see ce_bundle_apple_frameworks, registered - and so run - before this step), so a plain,
	# non-deep sign here only needs to seal the outer bundle around already-valid nested code.
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND codesign --force --sign -
			--entitlements "${CMAKE_CURRENT_SOURCE_DIR}/Entitlements.plist"
			"$<TARGET_BUNDLE_DIR:CE_App>"
		COMMENT "Code signing CE_App with entitlements"
	)
endif()

target_link_libraries(CE_App PRIVATE
	CE_Config
	CE_Engine
)

# Deliver the finished artifact to Binaries/<toolchain>/<config>/. Registered LAST on purpose: it
# is a POST_BUILD copy, and POST_BUILD steps run in registration order, so everything that fills
# the bundle (Info.plist, asset catalog, embedded frameworks) and finally signs it has already run.
ce_stage_to_binaries(CE_App)

if (NOT APPLE)
	# The bundle carries its Resources/ along; a plain executable does not, so the folder the
	# POST_BUILD steps above filled in the build tree is staged alongside the executable.
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"$<TARGET_FILE_DIR:CE_App>/Resources"
			"${CE_BINARIES_DIR}/Resources"
		COMMENT "Staging Resources to ${CE_BINARIES_DIR}"
		VERBATIM
	)
endif ()

# Copy to Binaries/Last folder (no-op unless CE_COPY_LAST is enabled)
ce_copy_to_last(CE_App)
