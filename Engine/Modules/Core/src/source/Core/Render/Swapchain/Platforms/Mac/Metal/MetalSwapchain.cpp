//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Mac/Metal
// File: MetalSwapchain.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#include "Core/Render/Surface/Mac/Metal/I_MetalSurface.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

#include <stdexcept>

#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"


namespace CE::Core {

MetalSwapchain::MetalSwapchain(I_MetalSurface& surface, MetalGraphicDevice& graphicDevice):
	_surface(&surface), _graphicDevice(&graphicDevice)
{
	_nativeLayer = _surface->GetMetalLayer();
	if (not _nativeLayer) [[unlikely]] {
		constexpr auto error = "MetalSwapchain::MetalSwapchain: The window offers no CA::MetalLayer to present into!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// The layer was pointed at a device by whoever created the view. Pointing it at ours makes that explicit and
	// survives the day the two stop being the same object: a drawable's texture belongs to the layer's device, and a
	// pipeline compiled on a different one cannot render into it.
	_nativeLayer->setDevice(_graphicDevice->GetDevice());

	// Presenting is all these buffers are ever used for. Saying so lets Metal pick the cheapest layout for them, and
	// the day something does want to read the drawable back - a screenshot, a post-process - this is the flag that has
	// to change first, with the memory cost that comes with it.
	_nativeLayer->setFramebufferOnly(true);

	// Sized from the window, not from the layer: a CAMetalLayer reports a drawableSize of zero until something has laid
	// its view out, and on a window created and rendered into in the same turn that is one frame too late - every frame
	// would be skipped for a target that is not actually zero-sized. Resize is also what allocates the depth buffer,
	// and it does nothing at all for a window that really is minimised.
	MetalSwapchain::Resize(0, 0);
}

MetalSwapchain::~MetalSwapchain() {
	if (not _nativeDrawable)
		return;

	// Dying between an acquire and a present: the drawable is dropped without being shown, and the device must stop
	// pointing at it before it goes.
	_graphicDevice->SetFrameTarget(nullptr, nullptr);
	_nativeDrawable.reset();
}

bool MetalSwapchain::AcquireNextTarget() {
	// Asked every frame rather than trusted from the last Resize: AppKit resizes the window on its own, and a frame
	// that ran before the resize event reached us would otherwise draw at the previous size and come out stretched.
	// A no-op when nothing changed, which is every frame but the ones that follow a resize.
	Resize(0, 0);

	if (_width == 0 or _height == 0)
		return false;

	// Where VSync actually happens: with display sync on, this blocks until the compositor has released a buffer, and
	// that block is what paces the whole loop to the refresh rate. It gives up after about a second and answers null.
	//
	// Autoreleased, and needed until Present - which is several calls and possibly an autorelease-pool drain away - so
	// it is retained rather than transferred: RetainPtr adds a reference of ours, TransferPtr would adopt one we were
	// never given and over-release it.
	_nativeDrawable = NS::RetainPtr(_nativeLayer->nextDrawable());
	if (not _nativeDrawable) [[unlikely]] {
		CE_CORE_WARN("MetalSwapchain::AcquireNextTarget: The layer had no drawable free; the frame is skipped.");
		return false;
	}

	// The seam: the swapchain owns the back buffers, the device builds the render passes, and this is where the two
	// meet. Cleared again in Present, so a pass opened outside a frame cannot draw into a drawable that is gone.
	_graphicDevice->SetFrameTarget(_nativeDrawable.get(), _nativeDepthTexture.get());

	return true;
}

void MetalSwapchain::Present() {
	// A frame that was skipped, or one presented twice: neither is an error, and neither should put whatever the back
	// buffer happened to hold on screen.
	if (not _nativeDrawable)
		return;

	// The buffer that drew the frame, not one of our own. presentDrawable shows the drawable when its own command
	// buffer is *scheduled*, and a buffer carrying nothing but a present is scheduled at once - so it could put the
	// drawable on screen while the pass that filled it was still running. Adding the present to the last buffer that
	// wrote it is the only arrangement where that cannot happen.
	//
	// Null when the frame opened no pass at all, which is legal: a frame can be acquired and presented untouched, and
	// then a bare command buffer is exactly right because there is nothing to wait for.
	auto nativeCommandBuffer = _graphicDevice->TakeFrameCommandBuffer();
	if (not nativeCommandBuffer)
		nativeCommandBuffer = NS::RetainPtr(_graphicDevice->GetCommandQueue()->commandBuffer());

	if (nativeCommandBuffer) [[likely]] {
		// The scheduling is Core Animation's either way; the interval only tells it how long to hold this frame back
		// before showing it. Zero is the plain call, and the two are separate methods rather than one with a default
		// because "as soon as possible" is not the same request as "not before now".
		if (_minimumPresentInterval > 0.0f)
			nativeCommandBuffer->presentDrawableAfterMinimumDuration(_nativeDrawable.get(), _minimumPresentInterval);
		else
			nativeCommandBuffer->presentDrawable(_nativeDrawable.get());

		nativeCommandBuffer->commit();
	}
	else {
		CE_CORE_WARN("MetalSwapchain::Present: The queue handed back no command buffer; the frame is dropped.");
	}

	_graphicDevice->SetFrameTarget(nullptr, nullptr);

	// Released so Core Animation can recycle it. Holding one drawable longer than its frame is the classic way to
	// starve the layer and halve the frame rate.
	_nativeDrawable.reset();
}

void MetalSwapchain::Resize(const u32 width, const u32 height) {
	// Zero means "whatever the window is now". That is how the constructor and every acquire ask for the current size
	// without duplicating this, and a caller that really meant zero is a minimised window - for which the window
	// answers zero anyway, and the early return below is the right behaviour either way.
	const auto [surfaceWidth, surfaceHeight] = _surface->GetDrawableSize();
	const auto newWidth = width != 0 ? width : surfaceWidth;
	const auto newHeight = height != 0 ? height : surfaceHeight;

	// Nothing changed and the depth buffer already exists: the common case, since this runs once per frame.
	if (newWidth == _width and newHeight == _height and _nativeDepthTexture)
		return;

	_width = newWidth;
	_height = newHeight;

	// A minimised window. Nothing to size the layer to and nothing to allocate; AcquireNextTarget keeps answering
	// false until a real size comes back, and the old depth texture is kept rather than thrown away for nothing.
	if (_width == 0 or _height == 0)
		return;

	_nativeLayer->setDrawableSize(CGSizeMake(_width, _height));

	// No texture can be resized, in any API: the depth buffer is a new object, and every render target handed out
	// before this call is stale - which is what the interface warns about.
	_CreateDepthTexture(_width, _height);
}

void MetalSwapchain::SetVSync(const bool enabled) {
	// The entire implementation of VSync here. With it on, nextDrawable() blocks until the compositor frees a buffer,
	// pacing the loop to the display; with it off it returns as soon as any buffer is free and frames are dropped by
	// the compositor instead.
	_nativeLayer->setDisplaySyncEnabled(enabled);
}

void MetalSwapchain::SetMinimumPresentInterval(const f32 seconds) {
	_minimumPresentInterval = seconds > 0.0f ? seconds : 0.0f;
}

bool MetalSwapchain::IsVSyncEnabled() const {
	// Asked, not cached: the layer is the one that holds this state, and it is reachable from outside this class.
	return _nativeLayer->displaySyncEnabled();
}

Types::PixelFormat MetalSwapchain::GetColorFormat() const {
	return Types::FromMetal(_nativeLayer->pixelFormat());
}

u32 MetalSwapchain::GetBufferCount() const {
	return static_cast<u32>(_nativeLayer->maximumDrawableCount());
}

void MetalSwapchain::_CreateDepthTexture(const u32 width, const u32 height) {
	const auto textureDescriptor = NS::TransferPtr(MTL::TextureDescriptor::alloc()->init());
	textureDescriptor->setTextureType(MTL::TextureType2D);
	textureDescriptor->setPixelFormat(Types::ToMetal(_depthFormat));
	textureDescriptor->setWidth(width);
	textureDescriptor->setHeight(height);

	// Private even on unified memory, and it is not a contradiction: shared memory means the CPU *could* see it, and
	// nothing ever wants to. Saying private lets the driver keep it in whatever layout the GPU reads fastest - and,
	// together with the DontCare store action the renderer asks for, lets a tile-based GPU keep the whole depth buffer
	// in tile memory and never write a byte of it to RAM.
	textureDescriptor->setStorageMode(MTL::StorageModePrivate);
	textureDescriptor->setUsage(MTL::TextureUsageRenderTarget);

	_nativeDepthTexture = NS::TransferPtr(_graphicDevice->GetDevice()->newTexture(textureDescriptor.get()));
	if (not _nativeDepthTexture) [[unlikely]] {
		constexpr auto error = "MetalSwapchain::_CreateDepthTexture: Could not allocate the depth buffer!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

}
