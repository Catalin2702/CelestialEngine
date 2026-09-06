#
# Module: CelestialEngine/Engine/Vendor/ImGui
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-03
# Updated by: Catalin Chirosca
# Updated: 2026-09-06
#

cmake_minimum_required(VERSION 4.1)

# ImGui ships sources, not a library: upstream has no build system, so the target is ours. It used to come from vcpkg,
# whose port compiles the two Objective-C++ backends with -fobjc-weak and WITHOUT -fobjc-arc. Both files are written
# for ARC, so under manual retain/release every object they alloc leaks - one FramebufferDescriptor per frame out of
# ImGui_ImplMetal_NewFrame, plus the MetalBuffer and MTLRenderPipelineDescriptor allocations. Owning the target is what
# lets us set that one flag; see IMGUI_OBJCXX_SOURCES below.
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/Engine/Submodules/ImGui)

set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp

	# Backends
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_OBJCXX_SOURCES "")

if (APPLE)
	set(IMGUI_OBJCXX_SOURCES
		${IMGUI_DIR}/backends/imgui_impl_metal.mm
		${IMGUI_DIR}/backends/imgui_impl_osx.mm
	)
endif ()

# SHARED, not STATIC as the vcpkg archive was: ImGui keeps its whole state in one global (GImGui), and both CE_Core and
# CE_Apple call into it. A static archive gets linked into BOTH dylibs, so the process ends up with two contexts and the
# Metal backend reads the one CE_Core never draws into. That it worked at all with libimgui.a was an accident of link
# order - libCE_Apple.dylib happened to resolve CE_Core's ImGui symbols before the archive was searched. As a dylib
# there is exactly one copy, by construction. It is bundled and rpath'd like a CE module (see
# CE_APPLE_FRAMEWORK_MODULE_NAMES).
add_library(CE_Vendor_ImGui SHARED
	${IMGUI_SOURCES}
	${IMGUI_OBJCXX_SOURCES}
)

set_target_properties(CE_Vendor_ImGui PROPERTIES
	LINKER_LANGUAGE CXX
	POSITION_INDEPENDENT_CODE ON
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# ImGui declares its public API with IMGUI_API, which is empty by default - fine wherever default visibility already
# exports everything, but on Windows a DLL exports nothing unless asked.
if (WIN32)
	target_compile_definitions(CE_Vendor_ImGui PRIVATE "IMGUI_API=__declspec(dllexport)")
	target_compile_definitions(CE_Vendor_ImGui INTERFACE "IMGUI_API=__declspec(dllimport)")
endif ()

# Source-level options are appended after the target's own flags, which is what makes -w win over CE_Config's -Werror
# in Debug: third-party sources must never fail our build for warnings we cannot fix.
set_source_files_properties(${IMGUI_SOURCES} PROPERTIES
	COMPILE_OPTIONS "-w"
)

if (APPLE)
	# -fobjc-arc is the fix described at the top of this file. The port also passed -fobjc-weak; ARC implies it, and
	# clang warns that it is unused, so it is not repeated here.
	set_source_files_properties(${IMGUI_OBJCXX_SOURCES} PROPERTIES
		COMPILE_OPTIONS "-fobjc-arc;-w"
	)
endif ()

# Modules
target_link_libraries(CE_Vendor_ImGui PUBLIC
	CE_Config
)

# Third-party
target_link_libraries(CE_Vendor_ImGui PRIVATE
	CE_Vendor_Glfw
)

if (APPLE)
	target_link_libraries(CE_Vendor_ImGui PRIVATE
		"-framework AppKit"
		"-framework GameController"
		"-framework Metal"
		"-framework MetalKit"
		"-framework QuartzCore"
	)
endif ()

# SYSTEM: the engine must not inherit warnings from headers it does not own.
target_include_directories(CE_Vendor_ImGui SYSTEM PUBLIC
	${IMGUI_DIR}
	${IMGUI_DIR}/backends
	${IMGUI_DIR}/misc/cpp
)

# Built and deployed exactly like a CE module library: into the build tree, with an @loader_path RPATH, and copied into
# CelestialEngine.app/Contents/Frameworks (and beside each test executable) by the macros that walk
# CE_APPLE_FRAMEWORK_MODULE_NAMES.
ce_set_module_library_output(CE_Vendor_ImGui)
ce_configure_module_library_rpath(CE_Vendor_ImGui)

ce_copy_to_last(CE_Vendor_ImGui)
