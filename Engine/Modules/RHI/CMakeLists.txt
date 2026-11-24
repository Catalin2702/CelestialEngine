#
# Module: CelestialEngine/Engine/Modules/RHI
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-24
#

# The Render Hardware Interface: devices, buffers, textures, pipelines, shaders, command encoders and swapchains, with
# one implementation per graphics backend. Nothing here knows about windows, cameras, layers or the application - the
# renderer in Core is written against these interfaces, and a tool that only needs the GPU can link this alone.
# Backend-specific files are compiled only when their backend is selected (CE_RENDER_BACKENDS, see
# Engine/cmake/CERenderBackends.cmake).

# region Headers

set(BASE_HEADERS
	src/include/RHI/RHI.hpp
)

set(BUFFER_HEADERS
	src/include/RHI/Buffer/Buffer.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp
		src/include/RHI/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp
	>
)

set(COMMAND_HEADERS
	src/include/RHI/Command/Command.hpp
	src/include/RHI/Command/RenderPassDescriptor.hpp
	src/include/RHI/Command/Viewport.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp
	>
)

set(CONVERSION_HEADERS
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Conversion/Platforms/Mac/Metal/MetalConversion.hpp
	>
)

set(DEVICE_HEADERS
	src/include/RHI/Device/Device.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp
	>
)

set(PIPELINE_HEADERS
	src/include/RHI/Pipeline/Pipeline.hpp
	src/include/RHI/Pipeline/PipelineDescriptor.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp
	>
)

set(SHADER_HEADERS
	src/include/RHI/Shader/Shader.hpp
	src/include/RHI/Shader/ShaderModuleDescriptor.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp
		src/include/RHI/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp
		src/include/RHI/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp
	>
)

set(SURFACE_HEADERS
	src/include/RHI/Surface/Surface.hpp
)

set(SWAPCHAIN_HEADERS
	src/include/RHI/Swapchain/Swapchain.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp
	>
)

set(TEXTURE_HEADERS
	src/include/RHI/Texture/Texture.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/include/RHI/Texture/Platforms/Common/OpenGl/OpenGlTexture.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/include/RHI/Texture/Platforms/Mac/Metal/MetalTexture.hpp
	>
)

# endregion

# region Interfaces

set(BUFFER_INTERFACES
	src/interface/RHI/Buffer/I_Buffer.hpp
	src/interface/RHI/Buffer/I_VertexArray.hpp
)

set(COMMAND_INTERFACES
	src/interface/RHI/Command/I_CommandEncoder.hpp
)

set(DEVICE_INTERFACES
	src/interface/RHI/Device/I_GraphicDevice.hpp
)

set(PIPELINE_INTERFACES
	src/interface/RHI/Pipeline/I_PipelineState.hpp
)

set(SHADER_INTERFACES
	src/interface/RHI/Shader/I_ShaderModule.hpp
)

set(SURFACE_INTERFACES
	src/interface/RHI/Surface/I_Surface.hpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/interface/RHI/Surface/Common/OpenGl/I_OpenGlSurface.hpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/interface/RHI/Surface/Mac/Metal/I_MetalSurface.hpp
	>
)

set(SWAPCHAIN_INTERFACES
	src/interface/RHI/Swapchain/I_Swapchain.hpp
)

set(TEXTURE_INTERFACES
	src/interface/RHI/Texture/I_Texture.hpp
)

# endregion

# region Sources

set(BUFFER_SOURCES
	src/source/RHI/Buffer/Buffer.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.cpp
		src/source/RHI/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Buffer/Platforms/Mac/Metal/MetalBuffer.cpp
	>
)

set(COMMAND_SOURCES
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Command/Platforms/Mac/Metal/MetalCommandEncoder.cpp
	>
)

set(CONVERSION_SOURCES
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Conversion/Platforms/Mac/Metal/MetalConversion.cpp
	>
)

set(DEVICE_SOURCES
	src/source/RHI/Device/GraphicDeviceFactory.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Device/Platforms/Mac/Metal/MetalGraphicDevice.cpp
	>
)

set(PIPELINE_SOURCES
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Pipeline/Platforms/Mac/Metal/MetalPipelineState.cpp
	>
)

set(SHADER_SOURCES
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.cpp
		src/source/RHI/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Shader/Platforms/Mac/Metal/MetalShaderLibrary.cpp
		src/source/RHI/Shader/Platforms/Mac/Metal/MetalShaderModule.cpp
	>
)

set(SWAPCHAIN_SOURCES
	src/source/RHI/Swapchain/SwapchainFactory.cpp
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Swapchain/Platforms/Mac/Metal/MetalSwapchain.cpp
	>
)

set(TEXTURE_SOURCES
	$<$<BOOL:${CE_RENDER_OPENGL}>:
		src/source/RHI/Texture/Platforms/Common/OpenGl/OpenGlTexture.cpp
	>

	$<$<BOOL:${CE_RENDER_METAL}>:
		src/source/RHI/Texture/Platforms/Mac/Metal/MetalTexture.cpp
	>
)

# endregion

set(HEADERS
	${BASE_HEADERS}
	${BUFFER_HEADERS}
	${COMMAND_HEADERS}
	${CONVERSION_HEADERS}
	${DEVICE_HEADERS}
	${PIPELINE_HEADERS}
	${SHADER_HEADERS}
	${SURFACE_HEADERS}
	${SWAPCHAIN_HEADERS}
	${TEXTURE_HEADERS}
)

set(INTERFACES
	${BUFFER_INTERFACES}
	${COMMAND_INTERFACES}
	${DEVICE_INTERFACES}
	${PIPELINE_INTERFACES}
	${SHADER_INTERFACES}
	${SURFACE_INTERFACES}
	${SWAPCHAIN_INTERFACES}
	${TEXTURE_INTERFACES}
)

set(SOURCES
	${BUFFER_SOURCES}
	${COMMAND_SOURCES}
	${CONVERSION_SOURCES}
	${DEVICE_SOURCES}
	${PIPELINE_SOURCES}
	${SHADER_SOURCES}
	${SWAPCHAIN_SOURCES}
	${TEXTURE_SOURCES}
)

add_library(CE_RHI SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_RHI PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

# Keeps Out/<config>/ down to just executables: the library goes to its DLL/ subfolder on Windows and Linux, and to
# <build>/Modules/<config>/ on macOS (see ce_set_module_library_output)
ce_set_module_library_output(CE_RHI)

# Lets it find its own sibling module libraries wherever it ends up (DLL/, beside a Tests/*.exe, or
# CelestialEngine.app/Contents/Frameworks)
ce_configure_module_library_rpath(CE_RHI)

# Modules: what the public headers need - the platform and backend macros, CE_RHI_API, and the engine's render types
# (PixelFormat, ShaderType, GraphicsApi...) that every RHI signature is written in.
target_link_libraries(CE_RHI PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

# Engine-internal, used by the .cpp files only: the core logger (CE_Internal + CE_Tools), the file system the shaders
# are read through (CE_IO), and Utility's Enumerate.
target_link_libraries(CE_RHI PRIVATE
	CE_Internal
	CE_IO
	CE_Tools
	CE_Utility
)

# Each backend's third party, only when the backend is compiled in. glad is PRIVATE: no public header includes it.
# metal-cpp is PUBLIC because the Metal headers hold NS::SharedPtr members; CE_Native brings CE_Apple, the one library
# that compiles metal-cpp's implementation (NS_PRIVATE_IMPLEMENTATION), which every Metal call here links against.
if (CE_RENDER_OPENGL)
	target_link_libraries(CE_RHI PRIVATE
		CE_Vendor_Glad
	)
endif ()

if (CE_RENDER_METAL)
	target_link_libraries(CE_RHI PUBLIC
		CE_Native
		CE_Vendor_MetalCpp
	)
endif ()

target_include_directories(CE_RHI PUBLIC
	src/include
	src/interface
)

target_compile_definitions(CE_RHI PRIVATE
	CE_RHI_EXPORTS
)
