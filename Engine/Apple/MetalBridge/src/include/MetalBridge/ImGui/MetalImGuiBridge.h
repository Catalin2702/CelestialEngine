//
// Created by Catalin Chirosca on 2026-02-26.
//

#pragma once

#ifndef CE_APPLE_METAL_METALBRIDGE_METALIMGUIBRIDGE_H
#define CE_APPLE_METAL_METALBRIDGE_METALIMGUIBRIDGE_H

namespace CE::Bridge {

void ImGuiMetalInit(void* device);
void ImGuiMetalShutdown();
void ImGuiMetalNewFrame(void* renderPassDescriptor);
void ImGuiMetalRenderDrawData(void* drawData, void* commandBuffer, void* renderEncoder);

}


#endif //CE_APPLE_METAL_METALBRIDGE_METALIMGUIBRIDGE_H
