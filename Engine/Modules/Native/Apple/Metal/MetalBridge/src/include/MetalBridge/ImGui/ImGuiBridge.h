//
// Module: CelestialEngine/Engine/Modules/MetalBridge/ImGui
// File: ImGuiBridge.h
// Created by: Catalin Chirosca
// Created: 2026-02-26
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#pragma once

#ifndef CE_APPLE_METAL_METALBRIDGE_IMGUIBRIDGE_H
#define CE_APPLE_METAL_METALBRIDGE_IMGUIBRIDGE_H

/**
 * @namespace CE::Apple::Bridge
 * @brief Bridge functions for ImGui Metal rendering backend
 * @details Provides C++ wrapper functions for the ImGui Metal backend (imgui_impl_metal).
 *			These functions handle the integration between ImGui and Metal rendering,
 *			converting between C++ and Objective-C Metal objects.
 */
namespace CE::Apple::Bridge {

/**
 * @brief Initializes the ImGui Metal backend
 * @param device Pointer to MTLDevice (as void*)
 * @details Sets up the Metal backend for ImGui rendering. Must be called before
 *			any ImGui Metal rendering operations. The device is used to create
 *			Metal resources for ImGui.
 */
void ImGuiMetalInit(void* device);

/**
 * @brief Shuts down the ImGui Metal backend
 * @details Cleans up all Metal resources allocated by ImGui. Should be called
 *			before destroying the Metal device.
 */
void ImGuiMetalShutdown();

/**
 * @brief Prepares ImGui for a new frame with Metal
 * @param renderPassDescriptor Pointer to MTLRenderPassDescriptor (as void*)
 * @details Must be called at the start of each frame before ImGui rendering.
 *			Sets up the render pass descriptor for ImGui drawing commands.
 */
void ImGuiMetalNewFrame(void* renderPassDescriptor);

/**
 * @brief Renders ImGui draw data using Metal
 * @param drawData Pointer to ImDrawData (as void*)
 * @param commandBuffer Pointer to MTLCommandBuffer (as void*)
 * @param renderEncoder Pointer to MTLRenderCommandEncoder (as void*)
 * @details Encodes all ImGui rendering commands into the Metal command buffer.
 *			Should be called after ImGui::Render() to actually draw the UI.
 */
void ImGuiMetalRenderDrawData(void* drawData, void* commandBuffer, void* renderEncoder);

/**
 * @brief Initializes the ImGui OSX/Cocoa platform backend
 * @param view Pointer to NSView (as void*)
 * @return bool True if initialization was successful, false otherwise
 * @details Sets up the OSX/Cocoa backend for ImGui platform handling. Must be called
 *			before any ImGui platform operations. The view is used to set up input handling
 *			and window management.
 */
bool ImGuiOSXInit(void* view);

/**
 * @brief Shuts down the ImGui OSX/Cocoa platform backend
 * @details Cleans up all platform resources allocated by ImGui. Should be called
 *			before destroying the window or view.
 */
void ImGuiOSXShutdown();

/**
 * @brief Prepares ImGui for a new frame with OSX/Cocoa
 * @param view Pointer to NSView (as void*, can be nullptr)
 * @details Must be called at the start of each frame before ImGui rendering.
 *			Updates input state and window properties from the Cocoa view.
 */
void ImGuiOSXNewFrame(void* view);

}

#endif //CE_APPLE_METAL_METALBRIDGE_IMGUIBRIDGE_H
