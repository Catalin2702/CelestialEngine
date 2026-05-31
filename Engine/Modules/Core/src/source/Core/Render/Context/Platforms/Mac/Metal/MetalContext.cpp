//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-31
//

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Apple/MetalCpp/Metal/Metal.hpp"
#include "Apple/MetalCpp/MetalKit/MetalKit.hpp"
#include "Tools/Log/Log.hpp"

#include <cassert>

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
}

void MetalContext::HandleContentSizeChange(const std::pair<float, float>&) const {
	// if (not _layer) {
	// 	CE_CORE_WARN("MetalContext::HandleContentSizeChange: Cannot handle content scale change because Metal layer is not initialized.");
	// 	return;
	// }
	//
	// const auto [width, height] = size;
	// _layer->setDrawableSize(CGSizeMake(width, height));
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

void MetalContext::SetView(MTK::RenderView* view) {
	if (not view) {
		CE_CORE_ERROR("MetalContext::SetView: Cannot set null view!");
		throw std::runtime_error("MetalContext::SetView: Cannot set null view!");
	}

	// The release of the previous view (if any) will be handled by the assign operator of NS::SharedPtr
	_view = NS::TransferPtr(view);
}

}
