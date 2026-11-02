#
# Module: config/cmake
# File: CERenderBackends.cmake
# Created by: Catalin Chirosca
# Created: 2026-11-02
# Updated by: Catalin Chirosca
# Updated: 2026-11-02
#

# Decides which graphics backends - and, from them, which window backends - are compiled into the engine.
#
#   -DCE_RENDER_BACKENDS=All                 every backend this OS supports AND the engine implements (default)
#   -DCE_RENDER_BACKENDS=Native              the OS's own API: Metal on macOS, DirectX12 on Windows, Vulkan on Linux
#   -DCE_RENDER_BACKENDS="Metal;OpenGL"      an explicit list; "Metal,OpenGL" works too and needs no shell quoting
#
# The FIRST backend of the resolved list is the runtime default, used when -g is not passed on the command line - so
# "OpenGL;Metal" and "Metal;OpenGL" compile the same code and differ only in what starts without arguments. All and
# Native expand in place, so "Metal;All" means "everything, Metal first".
#
# Included once from the root CMakeLists.txt BEFORE project(), for vcpkg's sake: the toolchain file runs `vcpkg install`
# inside the first project() call, and by then it must already know which optional dependencies the selection needs
# (see "vcpkg features" at the bottom). That early, the target-platform variables are not reliable yet, so the OS is read
# from the CMAKE_HOST_* ones - the engine builds for the machine it is built on. The results below are ordinary variables
# that project() and every subdirectory inherit:
#
#   CE_RENDER_RESOLVED_BACKENDS   the validated, expanded, de-duplicated list
#   CE_RENDER_DEFAULT_BACKEND     its first entry
#   CE_RENDER_OPENGL / _METAL / _VULKAN / _DIRECTX11 / _DIRECTX12    ON or OFF, one per backend
#   CE_WINDOW_GLFW / CE_WINDOW_COCOA                                 ON or OFF, derived - see below
#   VCPKG_MANIFEST_FEATURES       extended with the vcpkg.json features the selection needs
#
# CE_Config turns the same values into 0/1 compile definitions (see CelestialEngineMacros.cmake), so C++ reads the
# selection through the same names with #if or `if constexpr`.
#
# It is a cache variable read at configure time, so changing it needs a re-configure. A restricted selection still
# delivers to Binaries/<toolchain>/<config>, overwriting what an All build put there: keep such builds in their own
# build directory (a CMakeUserPresets.json preset with its own binaryDir and CE_TOOLCHAIN_NAME).

set(CE_RENDER_BACKENDS "All" CACHE STRING
	"Graphics backends to compile in: All, Native, or a list of OpenGL, Metal, Vulkan, DirectX11, DirectX12 - the first one is the runtime default")
set_property(CACHE CE_RENDER_BACKENDS PROPERTY STRINGS All Native OpenGL Metal Vulkan DirectX11 DirectX12)

# Every backend the GraphicsApi enum names, in the casing used everywhere else (CLI, enum, docs).
set(_CE_RENDER_KNOWN_BACKENDS OpenGL Metal Vulkan DirectX11 DirectX12)

# The ones that actually have code behind them. The order is the order All expands to, and so decides the default of an
# All build: OpenGL first keeps a plain launch on GLFW + OpenGL, as it has always been.
set(_CE_RENDER_IMPLEMENTED_BACKENDS OpenGL Metal)

if (CMAKE_HOST_APPLE)
	set(_CE_RENDER_OS_BACKENDS OpenGL Metal Vulkan)
	set(_CE_RENDER_NATIVE_BACKEND Metal)
elseif (CMAKE_HOST_WIN32)
	set(_CE_RENDER_OS_BACKENDS OpenGL Vulkan DirectX11 DirectX12)
	set(_CE_RENDER_NATIVE_BACKEND DirectX12)
else ()
	set(_CE_RENDER_OS_BACKENDS OpenGL Vulkan)
	set(_CE_RENDER_NATIVE_BACKEND Vulkan)
endif ()

# All = supported here AND implemented, in implementation order.
set(_CE_RENDER_ALL_BACKENDS "")
foreach (_backend IN LISTS _CE_RENDER_IMPLEMENTED_BACKENDS)
	if (_backend IN_LIST _CE_RENDER_OS_BACKENDS)
		list(APPEND _CE_RENDER_ALL_BACKENDS ${_backend})
	endif ()
endforeach ()

# Human-readable forms of the lists, for the messages below. A raw list would not do: its ';' separators would split
# an error message into several list entries.
string(REPLACE ";" ", " _CE_RENDER_KNOWN_TEXT "${_CE_RENDER_KNOWN_BACKENDS}")
string(REPLACE ";" ", " _CE_RENDER_ALL_TEXT "${_CE_RENDER_ALL_BACKENDS}")

# Commas are accepted as separators because a ';' has to be quoted on every shell and is easy to lose in an IDE field.
string(REPLACE "," ";" _CE_RENDER_REQUESTED "${CE_RENDER_BACKENDS}")

set(CE_RENDER_RESOLVED_BACKENDS "")
set(_CE_RENDER_ERRORS "")

foreach (_entry IN LISTS _CE_RENDER_REQUESTED)
	string(STRIP "${_entry}" _entry)
	if (_entry STREQUAL "")
		continue()
	endif ()

	string(TOLOWER "${_entry}" _entry_lower)

	if (_entry_lower STREQUAL "all")
		list(APPEND CE_RENDER_RESOLVED_BACKENDS ${_CE_RENDER_ALL_BACKENDS})
		continue()
	endif ()

	if (_entry_lower STREQUAL "native")
		string(TOLOWER "${_CE_RENDER_NATIVE_BACKEND}" _entry_lower)
	endif ()

	# Case-insensitive match against the canonical names, so "metal" and "METAL" both become "Metal".
	set(_canonical "")
	foreach (_known IN LISTS _CE_RENDER_KNOWN_BACKENDS)
		string(TOLOWER "${_known}" _known_lower)
		if (_entry_lower STREQUAL _known_lower)
			set(_canonical ${_known})
			break()
		endif ()
	endforeach ()

	if (NOT _canonical)
		list(APPEND _CE_RENDER_ERRORS "  '${_entry}' is not a graphics backend (expected All, Native, ${_CE_RENDER_KNOWN_TEXT})")
	elseif (NOT _canonical IN_LIST _CE_RENDER_OS_BACKENDS)
		list(APPEND _CE_RENDER_ERRORS "  ${_canonical} is not available on ${CMAKE_HOST_SYSTEM_NAME}")
	elseif (NOT _canonical IN_LIST _CE_RENDER_IMPLEMENTED_BACKENDS)
		list(APPEND _CE_RENDER_ERRORS "  ${_canonical} is not implemented yet")
	else ()
		list(APPEND CE_RENDER_RESOLVED_BACKENDS ${_canonical})
	endif ()
endforeach ()

# Keeps the first occurrence, so the default survives "Metal;All".
list(REMOVE_DUPLICATES CE_RENDER_RESOLVED_BACKENDS)

if (_CE_RENDER_ERRORS)
	string(REPLACE ";" "\n" _CE_RENDER_ERRORS "${_CE_RENDER_ERRORS}")
	message(FATAL_ERROR
		"CE_RENDER_BACKENDS='${CE_RENDER_BACKENDS}' cannot be built:\n${_CE_RENDER_ERRORS}\n"
		"Implemented on ${CMAKE_HOST_SYSTEM_NAME}: ${_CE_RENDER_ALL_TEXT}"
	)
endif ()

if (NOT CE_RENDER_RESOLVED_BACKENDS)
	message(FATAL_ERROR "CE_RENDER_BACKENDS='${CE_RENDER_BACKENDS}' selects no graphics backend; the engine needs at least one.")
endif ()

list(GET CE_RENDER_RESOLVED_BACKENDS 0 CE_RENDER_DEFAULT_BACKEND)

foreach (_backend IN LISTS _CE_RENDER_KNOWN_BACKENDS)
	string(TOUPPER "${_backend}" _backend_upper)
	if (_backend IN_LIST CE_RENDER_RESOLVED_BACKENDS)
		set(CE_RENDER_${_backend_upper} ON)
	else ()
		set(CE_RENDER_${_backend_upper} OFF)
	endif ()
endforeach ()

# Window backends follow from the graphics ones rather than being a second option: each pairing that works is fixed by
# the code (a GLFW window has no Metal surface, a Cocoa window has no OpenGL context), so letting the two be picked
# independently would only add combinations that compile and then fail at startup.
#   GLFW  - OpenGL today; Vulkan too, once it exists (GLFW hands out a VkSurfaceKHR on every platform).
#   Cocoa - Metal.
if (CE_RENDER_OPENGL OR CE_RENDER_VULKAN)
	set(CE_WINDOW_GLFW ON)
else ()
	set(CE_WINDOW_GLFW OFF)
endif ()

if (CMAKE_HOST_APPLE AND CE_RENDER_METAL)
	set(CE_WINDOW_COCOA ON)
else ()
	set(CE_WINDOW_COCOA OFF)
endif ()

set(_CE_WINDOW_BACKENDS "")
if (CE_WINDOW_GLFW)
	list(APPEND _CE_WINDOW_BACKENDS GLFW)
endif ()
if (CE_WINDOW_COCOA)
	list(APPEND _CE_WINDOW_BACKENDS Cocoa)
endif ()

string(REPLACE ";" ", " _CE_RENDER_SUMMARY "${CE_RENDER_RESOLVED_BACKENDS}")
string(REPLACE ";" ", " _CE_WINDOW_SUMMARY "${_CE_WINDOW_BACKENDS}")
message(STATUS "CelestialEngine graphics backends: ${_CE_RENDER_SUMMARY} (default: ${CE_RENDER_DEFAULT_BACKEND})")
message(STATUS "CelestialEngine window backends: ${_CE_WINDOW_SUMMARY}")

# vcpkg features. The optional third parties are vcpkg.json features rather than plain dependencies, so a selection that
# does not need them never downloads, builds or installs them:
#   opengl - glad, the OpenGL loader
#   glfw   - GLFW, the window of OpenGL (and, later, Vulkan)
# Appended to whatever VCPKG_MANIFEST_FEATURES already holds, so features asked for by hand are kept. Each build
# directory has its own vcpkg_installed/, and vcpkg installs exactly the requested set into it on every configure - so
# narrowing the selection also removes what is no longer needed.
set(_CE_VCPKG_FEATURES "")
if (CE_RENDER_OPENGL)
	list(APPEND _CE_VCPKG_FEATURES opengl)
endif ()
if (CE_WINDOW_GLFW)
	list(APPEND _CE_VCPKG_FEATURES glfw)
endif ()

list(APPEND VCPKG_MANIFEST_FEATURES ${_CE_VCPKG_FEATURES})
list(REMOVE_DUPLICATES VCPKG_MANIFEST_FEATURES)

string(REPLACE ";" ", " _CE_VCPKG_SUMMARY "${VCPKG_MANIFEST_FEATURES}")
if (NOT _CE_VCPKG_SUMMARY)
	set(_CE_VCPKG_SUMMARY "none")
endif ()
message(STATUS "CelestialEngine vcpkg features: ${_CE_VCPKG_SUMMARY}")
