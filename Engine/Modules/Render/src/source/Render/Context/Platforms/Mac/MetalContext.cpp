//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include "Render/Context/Platforms/Mac/MetalContext.hpp"

#include "Tools/Log/Log.hpp"

#include <cassert>
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CE::Render::Context {

MetalContext::MetalContext(const MetalContextProps& props): _props(props) {}

MetalContext::~MetalContext() = default;

void MetalContext::Init() {
	assert(_props.window && "MetalContext requires a valid NS::Window pointer");
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

	_layer->setDevice(_device.get());
	_layer->setPixelFormat(_props.pixelFormat);

	// ReSharper disable All
	if (const auto contentView = window->contentView()) {
		contentView->setLayer(_layer.get());
		contentView->setWantsLayer(true);
	}
	else {
		CE_CORE_ERROR("MetalContext::Init: Failed to get content view from Metal window!");
		throw std::runtime_error("MetalContext::Init: Failed to get content view from Metal window!");
	}

	_layer->setContentsScale(window->backingScaleFactor());
	_layer->setMaximumDrawableCount(3);
	_layer->setAllowsNextDrawableTimeout(false);
}

void MetalContext::SwapBuffers() {
	const auto drawable = _layer->nextDrawable();
	if (not drawable) {
		CE_CORE_WARN("MetalContext::SwapBuffers: Failed to get next drawable from Metal layer!");
		return;
	}

	const auto commandBuffer = _commandQueue->commandBuffer();
	if (not commandBuffer) {
		CE_CORE_WARN("MetalContext::SwapBuffers: Failed to create command buffer!");
		return;
	}

	commandBuffer->presentDrawable(drawable);
	commandBuffer->commit();
}
}
