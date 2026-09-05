//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Mac/Metal
// File: MetalSwapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_METALSWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_METALSWAPCHAIN_HPP

#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>


namespace CA {
	class MetalDrawable;
	class MetalLayer;
}

namespace MTL {
	class Texture;
}

namespace CE::Core {

class I_MetalSurface;
class MetalGraphicDevice;

/**
 * @class MetalSwapchain
 * @brief The window's CAMetalLayer, driven as the chain of back buffers a frame is presented from
 * @details Core Animation already is the swapchain here - the layer holds the buffers, hands out one drawable at a
 *			time and recycles it once the compositor is done - so this class does not build a chain, it drives the one
 *			the layer already has. What it adds is everything the layer has no opinion about: the depth buffer that
 *			goes with the colour one, and telling the device which pair the current frame renders into.
 *
 *			It owns the depth texture and, for the length of one frame, a reference to the drawable. It does not own
 *			the layer (the window's view does), nor the device (the renderer does); both must outlive it.
 */
class CE_CORE_API MetalSwapchain: public I_SwapchainBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Binds the swapchain to a window's layer and to the device its buffers are allocated from
	 * @param surface The window's presentation surface, which must outlive this swapchain
	 * @param graphicDevice The device the drawables and the depth buffer belong to, which must outlive this swapchain
	 * @details Throws std::runtime_error when the window has no layer yet: a swapchain with nothing to present into
	 *			cannot become valid later, so failing here is better than answering false to every AcquireNextTarget.
	 */
	MetalSwapchain(I_MetalSurface& surface, MetalGraphicDevice& graphicDevice);

	MetalSwapchain(const MetalSwapchain&) = delete;
	MetalSwapchain(MetalSwapchain&&) noexcept = default;

	/**
	 * @brief Releases the frame's drawable and unpublishes it from the device
	 * @details A swapchain destroyed mid-frame would otherwise leave the device pointing at a drawable nobody holds a
	 *			reference to any more, and the next BeginRenderPass would read it.
	 */
	~MetalSwapchain() override;

public:
	MetalSwapchain& operator = (const MetalSwapchain&) = delete;
	MetalSwapchain& operator = (MetalSwapchain&&) noexcept = default;

public:
	/**
	 * @brief Takes the next drawable from the layer and publishes it, with the depth buffer, on the device
	 * @details The two fallible cases the interface promised: a zero-sized layer (a minimised window) and a layer with
	 *			no drawable free. The second is what the fourth call in a triple-buffered frame would see, and Metal
	 *			answers it by blocking for up to a second and then returning null - a skipped frame, not an error.
	 */
	[[nodiscard]] bool AcquireNextTarget() override;

	/**
	 * @brief Schedules the acquired drawable for presentation and lets go of it
	 */
	void Present() override;

	/**
	 * @brief Resizes the layer's buffers and rebuilds the depth texture to match
	 * @details A texture cannot be resized in any API, so the depth buffer is a new one afterwards - which is exactly
	 *			why the interface warns that render targets taken before a Resize are invalid after it.
	 */
	void Resize(u32 width, u32 height) override;

	/**
	 * @brief Switches presentation between waiting for the display's refresh and running free
	 * @details One flag on the layer, and no rebuild: Metal is the backend where this really is the cheap setter it
	 *			looks like, unlike Vulkan where the present mode is baked into the swapchain.
	 */
	void SetVSync(bool enabled) override;

public:
	[[nodiscard]] bool IsVSyncEnabled() const override;

	/**
	 * @brief Gets the colour format of the drawables
	 * @details Read back from the layer rather than stored: the view sets it at creation, and a pipeline whose
	 *			RenderTargetFormats disagree with it is rejected when it is compiled.
	 */
	[[nodiscard]] Types::PixelFormat GetColorFormat() const override;

	/**
	 * @brief Gets how many drawables the layer rotates through
	 * @details Metal allows 2 or 3 and defaults to 3: triple buffering, which costs one drawable's worth of memory to
	 *			stop a late frame from idling the GPU until the next refresh.
	 */
	[[nodiscard]] u32 GetBufferCount() const override;

	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }

public:
	/**
	 * @brief Gets the layer being presented into
	 * @details Not on the interface: it is here for the Metal-only code that needs the real object - the ImGui layer,
	 *			and anything that has to read the colour space or the EDR settings.
	 */
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return _nativeLayer; }

	/**
	 * @brief Gets the depth buffer the frames are rendered against
	 */
	[[nodiscard]] MTL::Texture* GetDepthTexture() const { return _nativeDepthTexture.get(); }

	/**
	 * @brief Spaces presents at least this far apart, without blocking the loop
	 * @param seconds The minimum time between two frames reaching the display; 0 presents as soon as possible
	 * @details The third option between waiting for the display and not waiting at all. VSync makes the acquire block
	 *			until the compositor is ready, which paces the whole loop; presenting as soon as possible does not
	 *			block but lets a frame land on top of one still being scanned out, which is what tearing is. This asks
	 *			Core Animation to hold each frame back instead: the loop keeps running free, and no two frames reach
	 *			the display closer together than this.
	 *
	 *			To be free of tearing the interval has to be at least one display refresh - anything shorter still puts
	 *			two frames inside one scan. Shorter values are still useful as a plain limiter on how often the display
	 *			is touched, with the CPU left running.
	 *
	 *			Metal-only, and deliberately not on I_Swapchain: OpenGL has nothing like it (the swap interval counts
	 *			refreshes, not time), and neither does Vulkan, whose present modes are a fixed set rather than a
	 *			duration.
	 */
	void SetMinimumPresentInterval(f32 seconds);

	[[nodiscard]] f32 GetMinimumPresentInterval() const { return _minimumPresentInterval; }

	/**
	 * @brief Gets the drawable this frame is being rendered into
	 * @return CA::MetalDrawable* The acquired drawable, or null outside a frame
	 * @details Borrowed for the length of the frame, exactly as the device's frame target is. It exists for the code
	 *			that has to open a second pass on the same back buffer - the ImGui overlay does, because ImGui's Metal
	 *			backend encodes its own draws rather than going through I_CommandEncoder. Whoever uses it must load
	 *			rather than clear, and must not present: presenting is this class's job and happens once per frame.
	 */
	[[nodiscard]] CA::MetalDrawable* GetCurrentDrawable() const { return _nativeDrawable.get(); }

private:
	/**
	 * @brief Allocates the depth buffer for a given size, replacing the previous one
	 * @details Throws std::runtime_error if the allocation fails: at this size it only fails when the device is out of
	 *			memory, and there is nothing sensible to render without a depth buffer.
	 */
	void _CreateDepthTexture(u32 width, u32 height);

private:
	I_MetalSurface* _surface = nullptr; ///< Borrowed; owned by the window
	MetalGraphicDevice* _graphicDevice = nullptr; ///< Borrowed; owned by the renderer
	CA::MetalLayer* _nativeLayer = nullptr; ///< Borrowed; owned by the window's view

	NS::SharedPtr<MTL::Texture> _nativeDepthTexture;

	/// Held only between AcquireNextTarget and Present, and retained for that span: the layer hands it back
	/// autoreleased, and the pool it lives in is drained long before the frame ends.
	NS::SharedPtr<CA::MetalDrawable> _nativeDrawable;

	u32 _width = 0;
	u32 _height = 0;

	/// Not asked of the caller: the renderer's default pass wants a depth buffer, and this is the format that exists
	/// on every GPU the engine targets. It becomes a parameter the day a pass wants a stencil too.
	Types::PixelFormat _depthFormat = Types::PixelFormat::Depth32Float;

	/// 0 means present as soon as the GPU is done, which is what VSync-off has always meant here. See the setter.
	f32 _minimumPresentInterval = 0.0f;
};

}

#endif //CE_CORE_RENDER_SWAPCHAIN_METALSWAPCHAIN_HPP
