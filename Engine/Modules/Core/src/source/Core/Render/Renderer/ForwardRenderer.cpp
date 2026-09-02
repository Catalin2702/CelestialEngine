//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: ForwardRenderer.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Core/Render/Command/RenderPassDescriptor.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Renderer/ForwardRenderer.hpp"
#include "Core/Render/Renderer/DrawCommand.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Tools/Log/Log.hpp"

#include <cassert>


namespace CE::Core {

ForwardRenderer::ForwardRenderer(std::unique_ptr<I_GraphicDevice> graphicDevice, std::unique_ptr<I_Swapchain> swapchain):
	_graphicDevice(std::move(graphicDevice)), _swapchain(std::move(swapchain))
{
	assert(_graphicDevice != nullptr && "ForwardRenderer::ForwardRenderer: The renderer was given no graphic device.");
	assert(_swapchain != nullptr && "ForwardRenderer::ForwardRenderer: The renderer was given no swapchain.");
	assert(_graphicDevice->GetGraphicApi() == _swapchain->GetGraphicApi() && "ForwardRenderer::ForwardRenderer: The device and the swapchain were created for different graphics APIs.");
}

ForwardRenderer::~ForwardRenderer() {
	// The encoder holds live backend state - a bound vertex array on OpenGL, an open encoder on Metal - so a renderer
	// destroyed mid-frame has to close the pass while the device is still alive.
	ForwardRenderer::EndPass();
}

bool ForwardRenderer::BeginFrame() {
	assert(not _inFrame && "ForwardRenderer::BeginFrame: A frame is already open; every BeginFrame needs its EndFrame.");
	if (_inFrame) [[unlikely]]
		return false;

	if (not _swapchain->AcquireNextTarget())
		return false;

	_frameStats.Reset();

	_inFrame = true;

	return _inFrame;
}

void ForwardRenderer::EndFrame() {
	// Not an error: it is what a caller does after a BeginFrame that answered false, and presenting a frame that was
	// never drawn would put whatever the back buffer happened to hold on screen.
	if (not _inFrame)
		return;

	EndPass();

	_swapchain->Present();

	_stats = _frameStats;
	_inFrame = false;
}

void ForwardRenderer::BeginPass() {
	// Backing pixels, which is what a render area and a viewport are measured in - not the window's screen coordinates.
	const auto [width, height] = _swapchain->GetSize();

	RenderPassDescriptor descriptor{};
	descriptor.width = width;
	descriptor.height = height;
	auto& color0 = descriptor.colors[0];
	color0.loadAction = Types::LoadAction::Clear;
	color0.storeAction = Types::StoreAction::Store;
	color0.clearColor = _clearColor;

	descriptor.depth.enabled = true;
	descriptor.depth.loadAction = Types::LoadAction::Clear;
	descriptor.depth.storeAction = Types::StoreAction::DontCare;

	BeginPass(descriptor);
}

void ForwardRenderer::BeginPass(const RenderPassDescriptor& descriptor) {
	assert(_inFrame && "ForwardRenderer::BeginPass: A pass outside a frame draws into a target nobody will present.");
	if (not _inFrame) [[unlikely]]
		return;

	// One pass at a time: a tile-based backend cannot interleave two encoders on one command buffer, so opening a
	// second has to close the first rather than nest inside it.
	EndPass();

	_commandEncoder = _graphicDevice->BeginRenderPass(descriptor);

	// A null encoder is a pass the device skipped - a zero-sized render area - and not a failure to report twice.
	if (not _commandEncoder) [[unlikely]]
		return;

	// The whole render area, in this backend's own convention: the Viewport constructor is what flips the origin for
	// OpenGL, and it needs the target height to do it.
	_commandEncoder->SetViewport(Viewport{
		GetGraphicApi(),
		0.0_f32, 0.0_f32,
		static_cast<f32>(descriptor.width),
		static_cast<f32>(descriptor.height),
		static_cast<f32>(descriptor.height)
	});

	++_frameStats.passes;
}

void ForwardRenderer::EndPass() {
	// Idempotent on purpose: EndFrame, the next BeginPass and the destructor all call it, and only one of them is the
	// one that actually closes the pass.
	if (not _commandEncoder)
		return;

	// Ended before it is dropped, not instead of: on OpenGL this is what unbinds the encoder's vertex array and
	// program, so the ImGui overlay that draws next does not inherit them.
	_commandEncoder->End();
	_commandEncoder.reset();
}

void ForwardRenderer::Submit(const DrawCommand& command) {
	if (not _commandEncoder) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: No render pass is open, so the command has nowhere to go.");
		return;
	}

	if (not command.pipelineState or not command.vertexBuffer or not command.indexBuffer) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: The command is missing its pipeline, vertex buffer or index buffer.");
		return;
	}

	const auto availableIndices = static_cast<u32>(command.indexBuffer->GetCount());
	if (command.firstIndex >= availableIndices) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: firstIndex {} is past the end of a {}-index buffer.", command.firstIndex, availableIndices);
		return;
	}

	const auto indexCount = command.indexCount != 0 ? command.indexCount : availableIndices - command.firstIndex;

	_commandEncoder->SetPipelineState(*command.pipelineState);
	_commandEncoder->SetVertexBuffer(*command.vertexBuffer);
	_commandEncoder->SetIndexBuffer(*command.indexBuffer);

	_commandEncoder->DrawIndexed(indexCount, command.firstIndex, command.baseVertex);

	++_frameStats.drawCalls;
	_frameStats.indices += indexCount;
}

void ForwardRenderer::OnResize(const u32 width, const u32 height) {
	assert(not _inFrame && "ForwardRenderer::OnResize: The swapchain cannot be resized inside a frame - the target the frame acquired would be invalidated.");

	_swapchain->Resize(width, height);
}

void ForwardRenderer::SetVSync(const bool enabled) {
	_swapchain->SetVSync(enabled);
}

void ForwardRenderer::SetClearColor(const glm::vec4 color) {
	_clearColor = color;
}

}
