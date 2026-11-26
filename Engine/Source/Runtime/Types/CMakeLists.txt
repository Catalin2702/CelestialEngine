#
# Module: CelestialEngine/Engine/Source/Runtime/Types
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-15
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

# region Headers

set(BASE_HEADERS
	src/include/Types/Types.hpp
)

set(BUILD_HEADERS
	src/include/Types/Build/Build.hpp
)

set(KEYCODE_HEADERS
	src/include/Types/KeyCode/KeyboardKeyCode.hpp
	src/include/Types/KeyCode/KeyCode.hpp
	src/include/Types/KeyCode/MouseButtonCode.hpp
)

# glm: Math/Glm.hpp is the only header that includes it
set(MATH_HEADERS
	src/include/Types/Math/Glm.hpp
)

set(RENDER_HEADERS
	src/include/Types/Render/AttachmentAction.hpp
	src/include/Types/Render/BlendFactor.hpp
	src/include/Types/Render/Camera.hpp
	src/include/Types/Render/Compare.hpp
	src/include/Types/Render/CullMode.hpp
	src/include/Types/Render/GraphicsApi.hpp
	src/include/Types/Render/PixelFormat.hpp
	src/include/Types/Render/PrimitiveTopology.hpp
	src/include/Types/Render/Render.hpp
	src/include/Types/Render/Shader.hpp
	src/include/Types/Render/ShaderConstants.hpp
	src/include/Types/Render/ShaderDataType.hpp
	src/include/Types/Render/TextureUsage.hpp
)

# Not CE_Types' own: CE_Vars (below) carries it, from a folder of its own.
set(VAR_HEADERS
	Vars/include/Types/Var/Vars.hpp
)

set(WINDOW_HEADERS
	src/include/Types/Window/CursorMode.hpp
	src/include/Types/Window/Window.hpp
	src/include/Types/Window/WindowProps.hpp
)

# endregion

# region Sources

set(BUILD_SOURCES
	src/source/Types/Build/Build.cpp
)

set(KEYCODE_SOURCES
	src/source/Types/KeyCode/KeyboardKeyCode.cpp
)

set(RENDER_SOURCES
	src/source/Types/Render/BlendFactor.cpp
	src/source/Types/Render/GraphicsApi.cpp
	src/source/Types/Render/PixelFormat.cpp
	src/source/Types/Render/PrimitiveTopology.cpp
	src/source/Types/Render/Shader.cpp
)

set(WINDOW_SOURCES
	src/source/Types/Window/WindowProps.cpp
)

# endregion

set(HEADERS
	${BASE_HEADERS}
	${BUILD_HEADERS}
	${KEYCODE_HEADERS}
	${MATH_HEADERS}
	${RENDER_HEADERS}
	${VAR_HEADERS}
	${WINDOW_HEADERS}
)

set(SOURCES
	${BUILD_SOURCES}
	${KEYCODE_SOURCES}
	${RENDER_SOURCES}
	${WINDOW_SOURCES}
)

# CE_Vars: the integer aliases, literals, float comparisons and BIT (Types/Var/Vars.hpp), header-only and dependency-
# free, for a target that must not load the CE_Types dylib - a build-time program such as CE_ShaderCompiler, which runs
# before any application folder holds the engine's dylibs. Its own include root (Vars/include) exposes that one header
# and nothing else of Types. CE_Types links it PUBLIC, so every user of CE_Types keeps including Types/Var/Vars.hpp.
add_library(CE_Vars INTERFACE
	${VAR_HEADERS}
)

target_include_directories(CE_Vars INTERFACE
	Vars/include
)

add_library(CE_Types SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Types PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Out/<config>/ down to just executables: the library goes to its DLL/ subfolder on Windows and Linux, and to
# <build>/Modules/<config>/ on macOS (see ce_set_module_library_output)
ce_set_module_library_output(CE_Types)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_Types)

# Types is the engine's vocabulary - integers, enums, WindowProps, the glm math types - and nothing else: it knows no
# window library, no graphics API and no ImGui, so anything (IO, Tools, a command-line tool) can depend on it without
# pulling them in. Conversions to a backend's own types live with that backend (Core, later the RHI module).
# glm comes from here only (see ce_restrict_link in Engine/ThirdParty/Glm/CMakeLists.txt).
target_link_libraries(CE_Types PUBLIC
	CE_Config
	CE_Define
	CE_Vars
	CE_Vendor_Glm
)

target_link_libraries(CE_Types PRIVATE
	CE_Internal
)

target_include_directories(CE_Types PUBLIC
	src/include
)

target_compile_definitions(CE_Types PRIVATE
	CE_TYPES_EXPORTS
)
