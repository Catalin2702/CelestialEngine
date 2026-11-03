#
# Module: CelestialEngine/Apps/Sandbox
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

cmake_minimum_required(VERSION 4.1)

project(CE_Sandbox LANGUAGES CXX VERSION 0.1.3)

# The engine's test bed. Everything an application needs - shaders, bundle, Info.plist, embedded frameworks, signing,
# delivery to Binaries/ - comes from ce_add_application (Engine/cmake/CEApplication.cmake); an editor or a game is
# another directory under Apps/ with a call like this one and its own sources.
#
# The output keeps the engine's name, so the bundle is still Binaries/<toolchain>/<config>/CelestialEngine.app and the
# bundle id - under which macOS keeps the window frame and full-screen state - is the one it always had.
ce_add_application(CE_Sandbox
	OUTPUT_NAME CelestialEngine
	BUNDLE_NAME "Celestial Engine"
	BUNDLE_ID com.celestialengine.app
	ASSET_CATALOG Resources/Assets.xcassets
	SOURCES
		src/App.cpp
)
