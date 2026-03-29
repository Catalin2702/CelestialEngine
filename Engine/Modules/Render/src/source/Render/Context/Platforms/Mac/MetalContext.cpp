//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#include "Render/Context/Platforms/Mac/MetalContext.hpp"
#include "AppKit/View/RenderView.hpp"

#include "Tools/Log/Log.hpp"

#include <cassert>
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include "Window/Platforms/Mac/CocoaWindow.hpp"


namespace CE::Render::Context {

MetalContext::MetalContext(const MetalContextProps& props): _props(props) {}

MetalContext::MetalContext(void* window) {
	if (not window) {
		CE_CORE_ERROR("MetalContext constructor: Invalid window pointer provided.");
		throw std::invalid_argument("MetalContext constructor: Invalid window pointer provided.");
	}

	_props = MetalContextProps {
		.window = static_cast<Window::CocoaWindow*>(window),
		.pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm
	};
}

MetalContext::~MetalContext() = default;

void MetalContext::Init() {
	assert(_props.window && "MetalContext requires a valid NS::Window pointer");
	assert(_props.window->GetCocoaWindow() && "MetalContext requires a valid NS::Window pointer from CocoaWindow");
	assert(_props.window->GetCocoaView() && "MetalContext requires a valid NS::View pointer from CocoaWindow");
	const auto window = _props.window;
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

	_layer = NS::TransferPtr(CA::MetalLayer::layer());
	if (not _layer) {
		CE_CORE_ERROR("MetalContext::Init: Could not create CAMetalLayer!");
		throw std::runtime_error("MetalContext::Init: Could not create CAMetalLayer!");
	}

	// ReSharper disable All
	const auto metalView = window->GetCocoaView();
	metalView->setLayer(_layer.get());
	metalView->setWantsLayer(true);

	const auto metalWindow = window->GetCocoaWindow();

	_layer->setDevice(_device.get());
	_layer->setPixelFormat(_props.pixelFormat);
	_layer->setContentsScale(metalWindow->backingScaleFactor());
	_layer->setMaximumDrawableCount(3);
	_layer->setAllowsNextDrawableTimeout(false);
	// ReSharper restore All
}

void MetalContext::SwapBuffers() {}

void MetalContext::HandleContentSizeChange(const std::pair<float, float>& size) {
	if (not _layer) {
		CE_CORE_WARN("MetalContext::HandleContentSizeChange: Cannot handle content scale change because Metal layer is not initialized.");
		return;
	}

	const auto [width, height] = size;
	_layer->setDrawableSize(CGSizeMake(width, height));
}

void MetalContext::HandleVSyncChange(const bool enabled) {
	if (not _layer) {
		CE_CORE_WARN("MetalContext::HandleVSyncChange: Cannot handle VSync change because Metal layer is not initialized.");
		return;
	}

	_layer->setDisplaySyncEnabled(enabled);
}

}
