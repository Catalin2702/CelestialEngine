#
# Module: CelestialEngine/Engine/Source/Runtime/Native
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-27
#

cmake_minimum_required(VERSION 4.1)

add_library(CE_Native INTERFACE)

# One folder per platform under src/include/Platforms and src/source/Platforms: Apple (CE_Apple, below) and Windows
# (DllSearchBootstrap.cpp, compiled into each executable by ce_enable_dll_bootstrap, not into a library).
if(APPLE)
	set(APPLE_HEADERS
		src/include/Platforms/Apple/Apple.hpp
	)

	# CE_Apple is the platform layer - AppKit, Foundation, MetalKit, QuartzCore through metal-cpp, and metal-cpp's own
	# implementation - built whatever the backends. It knows no ImGui: the bridge to ImGui's Metal backend lives in Core,
	# beside the layer that uses it, so the RHI (which links CE_Apple for metal-cpp) does not drag ImGui along.
	set(METALCPP_HEADERS
		src/include/Platforms/Apple/MetalCpp/MetalCpp.hpp

		# AppKit
		src/include/Platforms/Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp
		src/include/Platforms/Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp
		src/include/Platforms/Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp

		# Foundation
		src/include/Platforms/Apple/MetalCpp/Foundation/NsAutoreleasePool.hpp

		# MetalKit
		src/include/Platforms/Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp

		# QuartzCore
		src/include/Platforms/Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp
	)

	set(METALCPP_SOURCES
		# AppKit
		src/source/Platforms/Apple/MetalCpp/AppKit/NsApplicationDelegate.cpp
		src/source/Platforms/Apple/MetalCpp/AppKit/NsViewEventDispatcher.cpp
		src/source/Platforms/Apple/MetalCpp/AppKit/NsWindowEventDispatcher.cpp

		# Implementation
		src/source/Platforms/Apple/MetalCpp/Implementation/MetalCppImplementation.cpp

		# MetalKit
		src/source/Platforms/Apple/MetalCpp/MetalKit/MtkViewDelegate.cpp

		# QuartzCore
		src/source/Platforms/Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.cpp
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

	# The root is src/include/Platforms, not src/include: the headers keep being included as <Apple/...>.
	target_include_directories(CE_Apple PUBLIC
		src/include/Platforms
	)

	# Built into the build tree like every other module library, so nothing lands next to
	# CelestialEngine.app (see ce_set_module_library_output); ce_bundle_apple_frameworks /
	# ce_embed_apple_modules_beside copy it out of there on demand. It still needs its own
	# @loader_path RPATH so it finds its sibling module libraries wherever it ends up.
	ce_set_module_library_output(CE_Apple)
	ce_configure_module_library_rpath(CE_Apple)
endif()

target_link_libraries(CE_Native INTERFACE
	CE_Config
)

# Platform-specific
target_link_libraries(CE_Native INTERFACE
	$<$<PLATFORM_ID:Darwin>:
		CE_Apple
	>
)
