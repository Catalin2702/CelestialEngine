//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"

#include "Tools/Log/Log.hpp"
#include "Utility/Config/Config.hpp"

#include "Foundation/Foundation.hpp"

#include <stdexcept>
#include <utility>

namespace CE::Core::Render::Context {

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

	_shaderLibrary = std::make_unique<Shader::MetalShaderLibrary>(_device.get());

	_CreateView();
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

	_viewDelegate = std::make_unique<MTK::ViewDelegate>();
	_view->setDelegate(_viewDelegate.get());

	viewEventDispatcher = std::make_unique<NS::ViewEventDispatcher>();
	_view->setEventDispatcher(viewEventDispatcher.get());
}

void MetalContext::SetDrawCallback(std::function<void(MTK::View*)> callback) const {
	if (not _viewDelegate) {
		CE_CORE_WARN("MetalContext::SetDrawCallback: View delegate is not initialized. Call Init() first.");
		return;
	}
	_viewDelegate->SetDrawInMtkViewCallback(std::move(callback));
}

void MetalContext::SetResizeCallback(std::function<void(MTK::View*, CGSize)> callback) const {
	if (not _viewDelegate) {
		CE_CORE_WARN("MetalContext::SetResizeCallback: View delegate is not initialized. Call Init() first.");
		return;
	}
	_viewDelegate->SetDrawableSizeWillChange(std::move(callback));
}

void MetalContext::HandleContentSizeChange(const std::pair<float, float>& size) const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::HandleContentSizeChange: Cannot handle content size change because the view is not initialized or does not have a layer.");
		return;
	}

	const auto [width, height] = size;
	_view->layer()->setDrawableSize(CGSizeMake(width, height));
}

void MetalContext::HandleVSyncChange(const bool enabled) const {
	if (not (_view and _view->layer())) {
		CE_CORE_WARN("MetalContext::HandleVSyncChange: Cannot handle VSync change because RenderView is not initialized or does not have a layer.");
		return;
	}

	const auto layer = _view->layer();
	layer->setDisplaySyncEnabled(enabled);
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
