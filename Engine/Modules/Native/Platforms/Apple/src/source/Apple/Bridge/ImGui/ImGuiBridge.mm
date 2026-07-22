//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/Metal/Bridge/ImGui
// File: ImGuiBridge.mm
// Created by: Catalin Chirosca
// Created: 2026-02-26
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <imgui_impl_metal.h>
#import <imgui_impl_osx.h>


namespace CE::Native {

void ImGuiMetalInit(void* device) {
	ImGui_ImplMetal_Init(static_cast<id<MTLDevice>>(device));
}

void ImGuiMetalShutdown() {
	ImGui_ImplMetal_Shutdown();
}

void ImGuiMetalNewFrame(void* renderPassDescriptor) {
	ImGui_ImplMetal_NewFrame(static_cast<MTLRenderPassDescriptor*>(renderPassDescriptor));
}

void ImGuiMetalRenderDrawData(void* drawData, void* commandBuffer, void* renderEncoder) {
	ImGui_ImplMetal_RenderDrawData(
		static_cast<ImDrawData*>(drawData),
		static_cast<id<MTLCommandBuffer>>(commandBuffer),
		static_cast<id<MTLRenderCommandEncoder>>(renderEncoder)
	);
}

}
