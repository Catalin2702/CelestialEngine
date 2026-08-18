//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Mac/Metal
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"

#include "Tools/Log/Log.hpp"
#include "Utility/Config/Config.hpp"

#include "Foundation/Foundation.hpp"
#include "QuartzCore/CAMetalLayer.hpp"

#include <cassert>
#include <stdexcept>
#include <utility>


namespace CE::Core {

void MetalContextEventDispatcher::DispatchMetalContextCreated() const {
	metalContextLifeCycleEvents.onCreatedDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchMetalContextInitialized() const {
	metalContextLifeCycleEvents.onInitializedDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchMetalContextWillShutdown() const {
	metalContextLifeCycleEvents.onWillShutdownDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchVSyncChanged(const bool vsync) const {
	metalContextLifeCycleEvents.onVSyncChangedDispatcher.Dispatch(vsync);
}

void MetalContextEventDispatcher::DispatchResizeEvent(const CGFloat width, const CGFloat height) const {
	metalContextLifeCycleEvents.onResizeDispatcher.Dispatch(width, height);
}

MetalContext::MetalContext() {
	metalContextEventDispatcher.DispatchMetalContextCreated();
}

MetalContext::~MetalContext() {
	_DestroyDisplayLink();
}

void MetalContext::Init() {
	_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	if (not _device) {
		CE_CORE_ERROR("MetalContext::Init: Could not create MetalDevice!");
		throw std::runtime_error("MetalContext::Init: Could not create MetalDevice!");
	}

	_commandQueue = NS::TransferPtr(_device->newCommandQueue());
	if (not _commandQueue) {
		CE_CORE_ERROR("MetalContext::Init: Could not create Metal Command Queue!");
		throw std::runtime_error("MetalContext::Init: Could not create Metal Command Queue!");
	}

	_shaderLibrary = MetalShaderLibrary(_device.get());

	_CreateView();

	// The CAMetalDisplayLink is created lazily by SetVSync (which the application calls after wiring up the draw delegate),
	// and only when VSync is on: it is what paces frames in that mode. With VSync off the tick loop drives frames and pulls
	// drawables via CAMetalLayer::nextDrawable(), which Core Animation forbids once a display link exists for the layer
	// (`-nextDrawable should not be called when using CAMetalDisplayLink`), so the layer is left display-link-free.
	metalContextEventDispatcher.DispatchMetalContextInitialized();
}

void MetalContext::_CreateView() {
	const auto& windowProps = Utility::Config::StGetWindowProps();
	const CGRect frame = {
		.origin = {.x = static_cast<CGFloat>(0), .y = static_cast<CGFloat>(0)},
		.size = {.width = static_cast<CGFloat>(windowProps.width), .height = static_cast<CGFloat>(windowProps.height)}
	};

	_view = NS::TransferPtr(MTK::View::alloc()->init(frame, _device.get()));
	if (not _view) {
		CE_CORE_ERROR("MetalContext::_CreateView: Could not create MetalKit view!");
		throw std::runtime_error("MetalContext::_CreateView: Could not create MetalKit view!");
	}

	_view->setColorPixelFormat(props.pixelFormat);
	_view->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));

	// The application drives the frame pace explicitly (either through the display-link timer when VSync is on, or
	// through the tick loop when it is off), so the view starts paused and does not repaint on `setNeedsDisplay`.
	_view->setPaused(true);
	_view->setEnableSetNeedsDisplay(false);

	_viewDelegate.SetDrawableSizeWillChangeDelegate(
		EventDelegate<MTK::View*, CGSize>::FromConstMethod<MetalContext, &MetalContext::_OnDrawableResize>(this)
	);
	_view->setDelegate(&_viewDelegate);

	_view->setEventDispatcher(&metalContextEventDispatcher);
}

void MetalContext::_CreateDisplayLink() {
	if (not (_view and _view->layer())) {
		constexpr auto error = "MetalContext::_CreateDisplayLink: Cannot create display link because MTKView or its layer are not initialized.";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_displayLink = NS::TransferPtr(CA::MetalDisplayLink::alloc()->init(_view->layer()));
	if (not _displayLink) {
		constexpr auto error = "MetalContext::_CreateDisplayLink: Could not create CAMetalDisplayLink.";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_displayLinkDelegate.SetMetalDisplayLinkNeedsUpdateDelegate(
		EventDelegate<CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*>::FromMethod<MetalContext, &MetalContext::_OnMetalLinkNeedsUpdate>(this)
	);
	_displayLink->setDelegate(&_displayLinkDelegate);

	// Start paused: the application unpauses it in SetRunning once the app is actually running.
	_displayLink->setPaused(true);
	_displayLink->addToRunLoop(NS::RunLoop::mainRunLoop(), NS::RunLoop::defaultMode());
}

void MetalContext::_DestroyDisplayLink() {
	if (not _displayLink)
		return;

	_displayLink->setPaused(true);
	_displayLink->removeFromRunLoop(NS::RunLoop::mainRunLoop(), NS::RunLoop::defaultMode());
	_displayLink->invalidate();
	_displayLink.reset();

	_displayLinkDrawable = nullptr;
}

void MetalContext::SetDrawDelegate(const EventDelegate<MTK::View*>& delegate) {
	assert(delegate.IsValid() and "MetalContext::SetDrawDelegate: The delegate is not valid.");
	_drawDispatcher.Bind(delegate);
}

CA::MetalDrawable* MetalContext::AcquireDrawable() const {
	// Prefer the drawable vended by the current display-link update; fall back to de-queuing one from the layer when no
	// update is in flight (e.g. the VSync-off tick loop drives frames without the display link).
	if (_displayLinkDrawable)
		return _displayLinkDrawable;

	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::AcquireDrawable: Cannot acquire drawable because MTKView or its layer are not initialized.");
		return nullptr;
	}

	return _view->layer()->nextDrawable();
}

void MetalContext::SetDisplayLinkPaused(const bool paused) const {
	if (not _displayLink) {
		CE_CORE_WARN("MetalContext::SetDisplayLinkPaused: Cannot change display link state because it is not initialized.");
		return;
	}
	_displayLink->setPaused(paused);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef because it's needed for the callback
void MetalContext::_OnMetalLinkNeedsUpdate(CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate* update) {
	assert(_drawDispatcher.IsBound() and "MetalContext::_OnMetalLinkNeedsUpdate: The delegate is not bound.");
	_displayLinkDrawable = update ? update->drawable() : nullptr;

	_drawDispatcher.Execute(_view.get());

	_displayLinkDrawable = nullptr;
}

void MetalContext::_OnDrawableResize(MTK::View*, const CGSize size) const {
	HandleContentSizeChange({static_cast<float>(size.width), static_cast<float>(size.height)});

	// `size` is already in backing pixels; fire it to whoever the application wired to the context's resize dispatcher.
	metalContextEventDispatcher.DispatchResizeEvent(size.width, size.height);
}

void MetalContext::HandleContentSizeChange(const std::pair<float, float>& size) const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::HandleContentSizeChange: Cannot handle content size change because MTKView or its layer are not initialized.");
		return;
	}

	const auto [width, height] = size;
	_view->layer()->setDrawableSize(CGSizeMake(width, height));
}

void MetalContext::SetVSync(const bool enabled) {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::SetVSync: Cannot handle VSync change because MTKView or its layer are not initialized.");
		return;
	}

	const auto layer = _view->layer();
	layer->setDisplaySyncEnabled(enabled);

	// Keep the display link in lockstep with the VSync state: it paces frames when VSync is on and must not exist when it is
	// off, otherwise CAMetalLayer::nextDrawable() (used by the VSync-off tick loop) is rejected by Core Animation.
	if (enabled) {
		if (not _displayLink)
			_CreateDisplayLink();
	}
	else
		_DestroyDisplayLink();

	metalContextEventDispatcher.DispatchVSyncChanged(enabled);
}

bool MetalContext::IsVSyncEnabled() const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::IsVSyncEnabled: Cannot get the VSync state because MTKView or its layer are not initialized.");
		return false;
	}

	const auto layer = _view->layer();
	return layer->displaySyncEnabled();
}

std::pair<float, float> MetalContext::GetContentScale() const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::GetContentScale: Cannot get the content scale because MTKView or its layer are not initialized.");
		return {1., 1.};
	}
	const auto scale = static_cast<float>(_view->layer()->contentsScale());
	return {scale, scale};
}

}
