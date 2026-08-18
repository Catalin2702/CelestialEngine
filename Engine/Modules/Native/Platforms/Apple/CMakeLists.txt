#
# Module: CelestialEngine/Engine/Modules/Native/Apple
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-08-18
#

cmake_minimum_required(VERSION 4.1)

set(INCLUDE src/include/Apple)
set(INTERFACE src/interface/Apple)
set(SOURCE src/source/Apple)

set(BRIDGE_HEADERS
	# ImGui
	${INCLUDE}/Bridge/ImGui/ImGuiBridge.h
)

set(METALCPP_HEADERS
	# AppKit
	${INCLUDE}/MetalCpp/AppKit/NsApplicationDelegate.hpp
	${INCLUDE}/MetalCpp/AppKit/NsViewEventDispatcher.hpp
	${INCLUDE}/MetalCpp/AppKit/NsWindowEventDispatcher.hpp

	# MetalKit
	${INCLUDE}/MetalCpp/MetalKit/MtkViewDelegate.hpp

	# QuartzCore
	${INCLUDE}/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp
)

set(BRIDGE_SOURCES
	# ImGui
	${SOURCE}/Bridge/ImGui/ImGuiBridge.mm
)

set(METALCPP_SOURCES
	# AppKit
	${SOURCE}/MetalCpp/AppKit/NsApplicationDelegate.cpp
	${SOURCE}/MetalCpp/AppKit/NsViewEventDispatcher.cpp
	${SOURCE}/MetalCpp/AppKit/NsWindowEventDispatcher.cpp

	# Implementation
	${SOURCE}/MetalCpp/Implementation/MetalCppImplementation.cpp

	# MetalKit
	${SOURCE}/MetalCpp/MetalKit/MtkViewDelegate.cpp

	# QuartzCore
	${SOURCE}/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.cpp
)

set(HEADERS
	${BRIDGE_HEADERS}
	${METALCPP_HEADERS}
)

set(SOURCES
	${BRIDGE_SOURCES}
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

target_link_libraries(CE_Apple PUBLIC
	CE_Config
	CE_Tools
	CE_Utility
)

# Vendor libraries
target_link_libraries(CE_Apple PUBLIC
	CE_Vendor_ImGui
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

# Link the platform-specific modules
ce_copy_to_last(CE_Apple)
