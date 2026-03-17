//
// Module: CelestialEngine/Engine/Modules/MetalBridge/ImGui
// File: ImGuiBridge.mm
// Created by: Catalin Chirosca
// Created: 2026-02-26
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <imgui_impl_metal.h>
#import <imgui_impl_osx.h>


namespace CE::Apple::Bridge {

/**
 * @brief Initializes the ImGui Metal rendering backend
 * @param device Void pointer to MTLDevice (will be cast to id<MTLDevice>)
 * @details Wraps ImGui_ImplMetal_Init() from the ImGui Metal backend.
 *			Casts the void pointer to the appropriate Objective-C Metal device type
 *			and initializes all Metal resources needed for ImGui rendering.
 */
void ImGuiMetalInit(void* device) {
	ImGui_ImplMetal_Init(static_cast<id<MTLDevice>>(device));
}

/**
 * @brief Shuts down the ImGui Metal rendering backend
 * @details Wraps ImGui_ImplMetal_Shutdown() from the ImGui Metal backend.
 *			Releases all Metal resources allocated for ImGui rendering.
 *			Should be called before destroying the Metal device.
 */
void ImGuiMetalShutdown() {
	ImGui_ImplMetal_Shutdown();
}

/**
 * @brief Prepares ImGui for rendering a new frame with Metal
 * @param renderPassDescriptor Void pointer to MTLRenderPassDescriptor
 * @details Wraps ImGui_ImplMetal_NewFrame() from the ImGui Metal backend.
 *			Casts the void pointer to MTLRenderPassDescriptor and sets up
 *			the Metal render pass for ImGui drawing. Must be called at the
 *			start of each frame before any ImGui rendering commands.
 */
void ImGuiMetalNewFrame(void* renderPassDescriptor) {
	ImGui_ImplMetal_NewFrame(static_cast<MTLRenderPassDescriptor*>(renderPassDescriptor));
}

/**
 * @brief Renders ImGui draw data using Metal command encoding
 * @param drawData Void pointer to ImDrawData structure
 * @param commandBuffer Void pointer to MTLCommandBuffer (id<MTLCommandBuffer>)
 * @param renderEncoder Void pointer to MTLRenderCommandEncoder (id<MTLRenderCommandEncoder>)
 * @details Wraps ImGui_ImplMetal_RenderDrawData() from the ImGui Metal backend.
 *			Casts all void pointers to their appropriate types and encodes all
 *			ImGui rendering commands into the Metal command buffer using the
 *			provided render encoder. Should be called after ImGui::Render()
 *			to actually draw the ImGui interface.
 */
void ImGuiMetalRenderDrawData(void* drawData, void* commandBuffer, void* renderEncoder) {
	ImGui_ImplMetal_RenderDrawData(
		static_cast<ImDrawData*>(drawData),
		static_cast<id<MTLCommandBuffer>>(commandBuffer),
		static_cast<id<MTLRenderCommandEncoder>>(renderEncoder)
	);
}

/**
 * @brief Initializes the ImGui OSX/Cocoa platform backend
 * @param view Void pointer to NSView (will be cast to NSView*)
 * @return bool True if initialization was successful, false otherwise
 * @details Wraps ImGui_ImplOSX_Init() from the ImGui OSX backend.
 *			Casts the void pointer to the appropriate Objective-C NSView type
 *			and initializes all platform resources needed for ImGui input handling.
 */
bool ImGuiOSXInit(void* view) {
	return ImGui_ImplOSX_Init(static_cast<NSView*>(view));
}

/**
 * @brief Shuts down the ImGui OSX/Cocoa platform backend
 * @details Wraps ImGui_ImplOSX_Shutdown() from the ImGui OSX backend.
 *			Releases all platform resources allocated for ImGui.
 *			Should be called before destroying the window or view.
 */
void ImGuiOSXShutdown() {
	ImGui_ImplOSX_Shutdown();
}

/**
 * @brief Prepares ImGui for a new frame with OSX/Cocoa
 * @param view Void pointer to NSView (can be nullptr)
 * @details Wraps ImGui_ImplOSX_NewFrame() from the ImGui OSX backend.
 *			Casts the void pointer to NSView and updates ImGui's input state
 *			from the Cocoa view. Must be called at the start of each frame
 *			before any ImGui rendering commands.
 */
void ImGuiOSXNewFrame(void* view) {
	ImGui_ImplOSX_NewFrame(static_cast<NSView*>(view));
}

}
