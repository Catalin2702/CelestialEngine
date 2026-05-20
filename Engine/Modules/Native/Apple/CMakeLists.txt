#
# Module: CelestialEngine/Engine/Modules/Native/Apple
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-03-02
# Updated by: Catalin Chirosca
# Updated: 2026-05-21
#

cmake_minimum_required(VERSION 3.21)

set(INCLUDE src/include)
set(INTERFACE src/interface)
set(SOURCE src/source)

set(APPKIT_HEADERS
	# Screen
	${INCLUDE}/AppKit/Screen/Screen.hpp

	# Utility
	${INCLUDE}/AppKit/Utility/AppKit.hpp

	# View
	${INCLUDE}/AppKit/View/RenderView.h

	# Window
	${INCLUDE}/AppKit/Window/WindowDelegate.h
)

set(BRIDGE_HEADERS
	# AppKit
	${INCLUDE}/Bridge/AppKit/Window/WindowBridge.h

	# ImGui
	${INCLUDE}/Bridge/ImGui/ImGuiBridge.h
)

set(COREANIMATION_HEADERS
	# DisplayLink
	${INCLUDE}/CoreAnimation/DisplayLink/DisplayLink.h
)

set(METALCPP_HEADERS
	# AppKit
	${INCLUDE}/MetalCpp/AppKit/AppKit.hpp
	${INCLUDE}/MetalCpp/AppKit/AppKitPrivate.hpp
	${INCLUDE}/MetalCpp/AppKit/NSApplication.hpp
	${INCLUDE}/MetalCpp/AppKit/NSEvent.hpp
	${INCLUDE}/MetalCpp/AppKit/NSMenu.hpp
	${INCLUDE}/MetalCpp/AppKit/NSMenuItem.hpp
	${INCLUDE}/MetalCpp/AppKit/NSOpenPanel.hpp
	${INCLUDE}/MetalCpp/AppKit/NSRunningApplication.hpp
	${INCLUDE}/MetalCpp/AppKit/NSView.hpp
	${INCLUDE}/MetalCpp/AppKit/NSWindow.hpp
	${INCLUDE}/MetalCpp/AppKit/RenderView.hpp
	${INCLUDE}/MetalCpp/AppKit/WindowDelegate.hpp

	# Foundation
	${INCLUDE}/MetalCpp/Foundation/Foundation.hpp
	${INCLUDE}/MetalCpp/Foundation/NSArray.hpp
	${INCLUDE}/MetalCpp/Foundation/NSAutoreleasePool.hpp
	${INCLUDE}/MetalCpp/Foundation/NSBundle.hpp
	${INCLUDE}/MetalCpp/Foundation/NSData.hpp
	${INCLUDE}/MetalCpp/Foundation/NSDate.hpp
	${INCLUDE}/MetalCpp/Foundation/NSDefines.hpp
	${INCLUDE}/MetalCpp/Foundation/NSDictionary.hpp
	${INCLUDE}/MetalCpp/Foundation/NSEnumerator.hpp
	${INCLUDE}/MetalCpp/Foundation/NSError.hpp
	${INCLUDE}/MetalCpp/Foundation/NSLock.hpp
	${INCLUDE}/MetalCpp/Foundation/NSNotification.hpp
	${INCLUDE}/MetalCpp/Foundation/NSNumber.hpp
	${INCLUDE}/MetalCpp/Foundation/NSObjCRuntime.hpp
	${INCLUDE}/MetalCpp/Foundation/NSObject.hpp
	${INCLUDE}/MetalCpp/Foundation/NSPrivate.hpp
	${INCLUDE}/MetalCpp/Foundation/NSProcessInfo.hpp
	${INCLUDE}/MetalCpp/Foundation/NSRange.hpp
	${INCLUDE}/MetalCpp/Foundation/NSSet.hpp
	${INCLUDE}/MetalCpp/Foundation/NSSharedPtr.hpp
	${INCLUDE}/MetalCpp/Foundation/NSString.hpp
	${INCLUDE}/MetalCpp/Foundation/NSTypes.hpp
	${INCLUDE}/MetalCpp/Foundation/NSURL.hpp

	# Metal
	${INCLUDE}/MetalCpp/Metal/Metal.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4AccelerationStructure.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4Archive.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4ArgumentTable.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4BinaryFunction.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4BinaryFunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CommandAllocator.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CommandBuffer.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CommandQueue.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CommitFeedback.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4Compiler.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4CompilerTask.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4ComputeCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4ComputePipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4Counters.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4FunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4LibraryDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4LibraryFunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4LinkingDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4MachineLearningCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4MachineLearningPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4MeshRenderPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4PipelineDataSetSerializer.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4PipelineState.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4RenderCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4RenderPass.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4RenderPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4SpecializedFunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4StitchedFunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTL4TileRenderPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTLAccelerationStructure.hpp
	${INCLUDE}/MetalCpp/Metal/MTLAccelerationStructureCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLAccelerationStructureTypes.hpp
	${INCLUDE}/MetalCpp/Metal/MTLAllocation.hpp
	${INCLUDE}/MetalCpp/Metal/MTLArgument.hpp
	${INCLUDE}/MetalCpp/Metal/MTLArgumentEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLBinaryArchive.hpp
	${INCLUDE}/MetalCpp/Metal/MTLBlitCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLBlitPass.hpp
	${INCLUDE}/MetalCpp/Metal/MTLBuffer.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCaptureManager.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCaptureScope.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCommandBuffer.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCommandQueue.hpp
	${INCLUDE}/MetalCpp/Metal/MTLComputeCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLComputePass.hpp
	${INCLUDE}/MetalCpp/Metal/MTLComputePipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTLCounters.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDataType.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDefines.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDepthStencil.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDevice.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDrawable.hpp
	${INCLUDE}/MetalCpp/Metal/MTLDynamicLibrary.hpp
	${INCLUDE}/MetalCpp/Metal/MTLEvent.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFence.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFunctionConstantValues.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFunctionDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFunctionHandle.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFunctionLog.hpp
	${INCLUDE}/MetalCpp/Metal/MTLFunctionStitching.hpp
	${INCLUDE}/MetalCpp/Metal/MTLGPUAddress.hpp
	${INCLUDE}/MetalCpp/Metal/MTLHeaderBridge.hpp
	${INCLUDE}/MetalCpp/Metal/MTLHeap.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIOCommandBuffer.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIOCommandQueue.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIOCompressor.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIndirectCommandBuffer.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIndirectCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLIntersectionFunctionTable.hpp
	${INCLUDE}/MetalCpp/Metal/MTLLibrary.hpp
	${INCLUDE}/MetalCpp/Metal/MTLLinkedFunctions.hpp
	${INCLUDE}/MetalCpp/Metal/MTLLogState.hpp
	${INCLUDE}/MetalCpp/Metal/MTLParallelRenderCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTLPixelFormat.hpp
	${INCLUDE}/MetalCpp/Metal/MTLPrivate.hpp
	${INCLUDE}/MetalCpp/Metal/MTLRasterizationRate.hpp
	${INCLUDE}/MetalCpp/Metal/MTLRenderCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLRenderPass.hpp
	${INCLUDE}/MetalCpp/Metal/MTLRenderPipeline.hpp
	${INCLUDE}/MetalCpp/Metal/MTLResidencySet.hpp
	${INCLUDE}/MetalCpp/Metal/MTLResource.hpp
	${INCLUDE}/MetalCpp/Metal/MTLResourceStateCommandEncoder.hpp
	${INCLUDE}/MetalCpp/Metal/MTLResourceStatePass.hpp
	${INCLUDE}/MetalCpp/Metal/MTLResourceViewPool.hpp
	${INCLUDE}/MetalCpp/Metal/MTLSampler.hpp
	${INCLUDE}/MetalCpp/Metal/MTLStageInputOutputDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTLTensor.hpp
	${INCLUDE}/MetalCpp/Metal/MTLTexture.hpp
	${INCLUDE}/MetalCpp/Metal/MTLTextureViewPool.hpp
	${INCLUDE}/MetalCpp/Metal/MTLTypes.hpp
	${INCLUDE}/MetalCpp/Metal/MTLVersion.hpp
	${INCLUDE}/MetalCpp/Metal/MTLVertexDescriptor.hpp
	${INCLUDE}/MetalCpp/Metal/MTLVisibleFunctionTable.hpp

	# MetalFX
	${INCLUDE}/MetalCpp/MetalFX/MetalFX.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTL4FXFrameInterpolator.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTL4FXSpatialScaler.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTL4FXTemporalDenoisedScaler.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTL4FXTemporalScaler.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXDefines.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXFrameInterpolator.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXPrivate.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXSpatialScaler.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXTemporalDenoisedScaler.hpp
	${INCLUDE}/MetalCpp/MetalFX/MTLFXTemporalScaler.hpp

	# MetalKit
	${INCLUDE}/MetalCpp/MetalKit/MetalKit.hpp
	${INCLUDE}/MetalCpp/MetalKit/MetalKitPrivate.hpp
	${INCLUDE}/MetalCpp/MetalKit/MTKTextureLoader.hpp
	${INCLUDE}/MetalCpp/MetalKit/MTKView.hpp

	# QuartzCore
	${INCLUDE}/MetalCpp/QuartzCore/CADefines.hpp
	${INCLUDE}/MetalCpp/QuartzCore/CAMetalDrawable.hpp
	${INCLUDE}/MetalCpp/QuartzCore/CAMetalLayer.hpp
	${INCLUDE}/MetalCpp/QuartzCore/CAPrivate.hpp
	${INCLUDE}/MetalCpp/QuartzCore/DisplayLink.hpp
	${INCLUDE}/MetalCpp/QuartzCore/QuartzCore.hpp
)

set(TYPES_HEADERS
	${INCLUDE}/Types/Types.hpp

	# EventHandlers
	${INCLUDE}/Types/EventHandlers/DisplayLinkEventHandler.hpp
	${INCLUDE}/Types/EventHandlers/WindowDelegateEventHandler.hpp
	${INCLUDE}/Types/EventHandlers/ViewEventHandler.hpp
)

set(TYPES_INTERFACES
	${INTERFACE}/Types/EventHandlers/I_DisplayLinkEventHandler.hpp
	${INTERFACE}/Types/EventHandlers/I_WindowDelegateEventHandler.hpp
	${INTERFACE}/Types/EventHandlers/I_ViewEventHandler.hpp
)

set(APPKIT_SOURCES
	# View
	${SOURCE}/AppKit/View/RenderView.mm

	# Window
	${SOURCE}/AppKit/Window/WindowDelegate.mm
)

set(BRIDGE_SOURCES
	# AppKit
	${SOURCE}/Bridge/AppKit/Window/WindowBridge.mm

	# ImGui
	${SOURCE}/Bridge/ImGui/ImGuiBridge.mm
)

set(COREANIMATION_SOURCES
	# DisplayLink
	${SOURCE}/CoreAnimation/DisplayLink/DisplayLink.mm
)

set(METALCPP_SOURCES
	# Define
	${SOURCE}/MetalCpp/Define/MetalCppDefine.cpp
)

set(TYPES_SOURCES
	# EventHandlers
	${SOURCE}/Types/EventHandlers/DisplayLinkEventHandler.cpp
	${SOURCE}/Types/EventHandlers/WindowDelegateEventHandler.cpp
	${SOURCE}/Types/EventHandlers/ViewEventHandler.cpp
)

set(HEADERS
	${APPKIT_HEADERS}
	${BRIDGE_HEADERS}
	${COREANIMATION_HEADERS}
	${METALCPP_HEADERS}
	${TYPES_HEADERS}
)

set(INTERFACES
	${TYPES_INTERFACES}
)

set(SOURCES
	${APPKIT_SOURCES}
	${BRIDGE_SOURCES}
	${COREANIMATION_SOURCES}
	${METALCPP_SOURCES}
	${TYPES_SOURCES}
)

# Create the CE_Apple library (interface library since it aggregates platform-specific APIs)
add_library(CE_Apple SHARED
	${HEADERS}
	${INTERFACES}
	${SOURCES}
)

set_target_properties(CE_Apple PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

target_link_libraries(CE_Apple PUBLIC
	CE_Config
)

target_link_libraries(CE_Apple PUBLIC
	"-framework AppKit"
	"-framework CoreVideo"
	"-framework Foundation"
	"-framework GameController"
	"-framework Metal"
	"-framework MetalKit"
	"-framework QuartzCore"
)

# Vendor libraries
target_link_libraries(CE_Apple PUBLIC
	CE_Vendor_ImGui
)

target_compile_definitions(CE_Apple PRIVATE
	CE_DEFINE_EXPORTS
)

target_compile_options(CE_Apple PUBLIC
	$<$<PLATFORM_ID:Darwin>:
	-ObjC++ -fno-objc-arc
	>
)

target_include_directories(CE_Apple PUBLIC
	${INCLUDE}
	${INTERFACE}
)

# Link the platform-specific modules
ce_copy_to_last(CE_Apple)
