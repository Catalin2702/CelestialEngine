#
# Module: CelestialEngine/App
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-03-09
#

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CE_App LANGUAGES CXX VERSION 0.0.5)

set(SOURCES
	src/App.cpp
)

# Create macOS bundle to avoid system warnings
if(APPLE)
	# Set the Asset Catalog for the app icon
	set(ASSET_CATALOG "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Assets.xcassets")

	add_executable(CE_App MACOSX_BUNDLE
		${SOURCES}
	)

	# Set bundle properties
	set_target_properties(CE_App PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "CelestialEngine App"
		MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
		MACOSX_BUNDLE_EXECUTABLE_NAME "CE_App"
		MACOSX_BUNDLE_GUI_IDENTIFIER "com.celestialengine.app"
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
		LINKER_LANGUAGE CXX
	)

	# Copy Info.plist to bundle (fallback if MACOSX_BUNDLE_INFO_PLIST doesn't work)
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
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

	# Sign the app with ad-hoc signature and entitlements (suppresses warnings)
	add_custom_command(TARGET CE_App POST_BUILD
		COMMAND codesign --force --sign -
			--entitlements "${CMAKE_CURRENT_SOURCE_DIR}/Entitlements.plist"
			--deep "$<TARGET_BUNDLE_DIR:CE_App>"
		COMMENT "Code signing CE_App with entitlements"
	)
else()
	add_executable(CE_App
		${SOURCES}
	)

	set_target_properties(CE_App PROPERTIES LINKER_LANGUAGE CXX)
endif()

target_link_libraries(CE_App PRIVATE
	CE_Config
	CE_Engine
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_App)

