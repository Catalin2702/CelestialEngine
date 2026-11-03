#
# Module: CelestialEngine/Apps/Sandbox
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-03
#

cmake_minimum_required(VERSION 4.1)

project(CE_Sandbox LANGUAGES CXX VERSION 0.1.3)

# The engine's test bed: a plain executable with its sources and include directories, then ce_configure_application
# (Engine/cmake/CEApplication.cmake) for everything an engine executable needs - shaders, bundle, Info.plist, embedded
# frameworks, signing, delivery to Binaries/. An editor or a game is another directory under Apps/ shaped like this one.
#
# The output keeps the engine's name, so the bundle is still Binaries/<toolchain>/<config>/CelestialEngine.app and the
# bundle id - under which macOS keeps the window frame and full-screen state - is the one it always had.

add_executable(CE_Sandbox
	src/include/SandboxLayer.hpp
	src/source/App.cpp
	src/source/SandboxLayer.cpp
)

target_include_directories(CE_Sandbox PRIVATE
	src/include
)

ce_configure_application(CE_Sandbox
	OUTPUT_NAME "CelestialEngine"
	BUNDLE_NAME "Celestial Engine"
	BUNDLE_ID "com.celestialengine.app"
	ASSET_CATALOG "Resources/Assets.xcassets"
)
