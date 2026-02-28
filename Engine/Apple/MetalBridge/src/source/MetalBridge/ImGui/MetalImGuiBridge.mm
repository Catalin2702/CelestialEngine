//
// Module: MetalBridge/ImGui
// File: MetalImGuiBridge.mm
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-26
// Updated: 2026-02-28
//

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <imgui_impl_metal.h>


namespace CE::Bridge {

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
