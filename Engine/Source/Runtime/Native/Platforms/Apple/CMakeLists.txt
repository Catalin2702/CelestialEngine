#
# Module: CelestialEngine/Engine/Source/Runtime/Native/Apple
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

cmake_minimum_required(VERSION 4.1)

set(APPLE_HEADERS
	src/include/Apple/Apple.hpp
)

# CE_Apple is the platform layer - AppKit, Foundation, MetalKit, QuartzCore through metal-cpp, and metal-cpp's own
# implementation - built whatever the backends. It knows no ImGui: the bridge to ImGui's Metal backend lives in Core,
# beside the layer that uses it, so the RHI (which links CE_Apple for metal-cpp) does not drag ImGui along.
set(METALCPP_HEADERS
	src/include/Apple/MetalCpp/MetalCpp.hpp

	# AppKit
	src/include/Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp
	src/include/Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp
	src/include/Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp

	# Foundation
	src/include/Apple/MetalCpp/Foundation/NsAutoreleasePool.hpp

	# MetalKit
	src/include/Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp

	# QuartzCore
	src/include/Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp
)

set(METALCPP_SOURCES
	# AppKit
	src/source/Apple/MetalCpp/AppKit/NsApplicationDelegate.cpp
	src/source/Apple/MetalCpp/AppKit/NsViewEventDispatcher.cpp
	src/source/Apple/MetalCpp/AppKit/NsWindowEventDispatcher.cpp

	# Implementation
	src/source/Apple/MetalCpp/Implementation/MetalCppImplementation.cpp

	# MetalKit
	src/source/Apple/MetalCpp/MetalKit/MtkViewDelegate.cpp

	# QuartzCore
	src/source/Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.cpp
)

set(HEADERS
	${APPLE_HEADERS}
	${METALCPP_HEADERS}
)

set(SOURCES
	${METALCPP_SOURCES}
)

# Create the CE_Apple library (interface library since it aggregates platform-specific APIs)
add_library(CE_Apple SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Apple PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Define for the export macros its headers use, Utility for the Dispatcher its delegates expose. No Tools: nothing in
# CE_Apple logs.
target_link_libraries(CE_Apple PUBLIC
	CE_Config
	CE_Define
	CE_Utility
)

target_link_libraries(CE_Apple PRIVATE
	CE_Internal
)

# Vendor libraries
target_link_libraries(CE_Apple PUBLIC
	CE_Vendor_MetalCpp
)

target_compile_definitions(CE_Apple PRIVATE
	CE_APPLE_EXPORTS
)

target_include_directories(CE_Apple PUBLIC
	src/include
)

# Built into the build tree like every other module library, so nothing lands next to
# CelestialEngine.app (see ce_set_module_library_output); ce_bundle_apple_frameworks /
# ce_embed_apple_modules_beside copy it out of there on demand. It still needs its own
# @loader_path RPATH so it finds its sibling module libraries wherever it ends up.
ce_set_module_library_output(CE_Apple)
ce_configure_module_library_rpath(CE_Apple)
