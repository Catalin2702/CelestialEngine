//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Mac/Metal
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
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
	metalContextCreatedDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchMetalContextInitialized() const {
	metalContextDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchMetalContextWillShutdown() const {
	metalContextWillShutdownDispatcher.Dispatch();
}

void MetalContextEventDispatcher::DispatchVSyncChanged(const bool vsync) const {
	vsyncChangedDispatcher.Dispatch(vsync);
}

MetalContext::MetalContext() {
	metalContextEventDispatcher = std::make_unique<MetalContextEventDispatcher>();
	metalContextEventDispatcher->metalContextCreatedDispatcher.Dispatch();
}

MetalContext::~MetalContext() {
	if (_displayLink) {
		_displayLink->removeFromRunLoop(NS::RunLoop::mainRunLoop(), NS::RunLoop::defaultMode());
		_displayLink->invalidate();
	}
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

	_shaderLibrary = std::make_unique<MetalShaderLibrary>(_device.get());

	_CreateView();

	// Only attach a CAMetalDisplayLink when VSync is on: it is what paces frames in that mode. With VSync off the tick loop
	// drives frames and pulls drawables via CAMetalLayer::nextDrawable(), which Core Animation forbids once a display link
	// exists for the layer (`-nextDrawable should not be called when using CAMetalDisplayLink`). So leave the layer
	// display-link-free in that mode.
	if (Utility::Config::Config::StGetWindowProps().VSync)
		_CreateDisplayLink();

	metalContextEventDispatcher->metalContextDispatcher.Dispatch();
}

void MetalContext::_CreateView() {
	const auto& windowProps = Utility::Config::Config::StGetWindowProps();
	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(windowProps.width), static_cast<CGFloat>(windowProps.height)}
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

	_viewDelegate = std::make_unique<Native::MtkViewDelegate>();
	_view->setDelegate(_viewDelegate.get());

	_view->setEventDispatcher(metalContextEventDispatcher.get());
}

void MetalContext::_CreateDisplayLink() {
	if (not (_view and _view->layer())) {
		CE_CORE_ERROR("MetalContext::_CreateDisplayLink: Cannot create display link because the view or its layer is not initialized.");
		throw std::runtime_error("MetalContext::_CreateDisplayLink: Cannot create display link because the view or its layer is not initialized.");
	}

	_displayLink = NS::TransferPtr(CA::MetalDisplayLink::alloc()->init(_view->layer()));
	if (not _displayLink) {
		CE_CORE_ERROR("MetalContext::_CreateDisplayLink: Could not create CAMetalDisplayLink!");
		throw std::runtime_error("MetalContext::_CreateDisplayLink: Could not create CAMetalDisplayLink!");
	}

	_displayLinkDelegate = std::make_unique<Native::CaMetalDisplayLinkDelegate>();
	_displayLinkDelegate->SetMetalDisplayLinkNeedsUpdateDelegate(
		EventDelegate<CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*>::FromMethod<MetalContext, &MetalContext::_OnMetalLinkNeedsUpdate>(this)
	);
	_displayLink->setDelegate(_displayLinkDelegate.get());

	// Start paused: the application unpauses it in SetRunning once the app is actually running.
	_displayLink->setPaused(true);
	_displayLink->addToRunLoop(NS::RunLoop::mainRunLoop(), NS::RunLoop::defaultMode());
}

void MetalContext::SetDrawDelegate(const EventDelegate<MTK::View*>& delegate) {
	assert(delegate.IsValid() and "MetalContext::SetDrawDelegate: The delegate is not valid!");
	_drawDispatcher.Bind(delegate);
}

CA::MetalDrawable* MetalContext::AcquireDrawable() const {
	// Prefer the drawable vended by the current display-link update; fall back to dequeuing one from the layer when no
	// update is in flight (e.g. the VSync-off tick loop drives frames without the display link).
	if (_displayLinkDrawable)
		return _displayLinkDrawable;

	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::AcquireDrawable: Cannot acquire drawable because the view or its layer is not initialized.");
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
	assert(_drawDispatcher.IsBound() and "MetalContext::_OnMetalLinkNeedsUpdate:  The delegate is not bound.");
	_displayLinkDrawable = update ? update->drawable() : nullptr;

	_drawDispatcher.Execute(_view.get());

	_displayLinkDrawable = nullptr;
}

void MetalContext::SetDrawableResizeDelegate(const EventDelegate<MTK::View*, CGSize>& delegate) const {
	if (not _viewDelegate) {
		CE_CORE_WARN("MetalContext::SetDrawableResizeDelegate: View delegate is not initialized. Call Init() first.");
		return;
	}
	_viewDelegate->SetDrawableSizeWillChangeDelegate(delegate);
}

void MetalContext::HandleContentSizeChange(const std::pair<float, float>& size) const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::HandleContentSizeChange: Cannot handle content size change because the view is not initialized or does not have a layer.");
		return;
	}

	const auto [width, height] = size;
	_view->layer()->setDrawableSize(CGSizeMake(width, height));
}

void MetalContext::SetVSync(const bool enabled) const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::HandleVSyncChange: Cannot handle VSync change because RenderView is not initialized or does not have a layer.");
		return;
	}

	const auto layer = _view->layer();
	layer->setDisplaySyncEnabled(enabled);
	metalContextEventDispatcher->vsyncChangedDispatcher.Dispatch(enabled);
}

bool MetalContext::IsVSyncEnabled() const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::IsVSyncEnabled: Cannot get VSync state because RenderView is not initialized or does not have a layer.");
		return false;
	}

	const auto layer = _view->layer();
	return layer->displaySyncEnabled();
}

}
