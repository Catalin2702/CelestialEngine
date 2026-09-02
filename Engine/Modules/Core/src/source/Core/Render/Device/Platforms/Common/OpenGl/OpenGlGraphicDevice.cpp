//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Common/OpenGl
// File: OpenGlGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp"
#include "Core/Render/Command/RenderPassDescriptor.hpp"
#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <cassert>

namespace CE::Core {

namespace {

/// Applies the pass' load actions, which in OpenGL amounts to one glClear with the bits the Clear actions ask for.
void ApplyLoadActions(const RenderPassDescriptor& descriptor) {
	auto clearMask = Types::BufferBit{};
	bool clears = false;

	// Only attachment 0 is reachable while every target is the default framebuffer; several attachments would need
	// glClearBufferfv per index, since glClearColor holds a single colour for all of them.
	if (descriptor.colorCount > 0 and descriptor.colors[0].loadAction == Types::LoadAction::Clear) {
		const auto& color = descriptor.colors[0].clearColor;
		glClearColor(color.r, color.g, color.b, color.a);

		clearMask = clearMask | Types::BufferBit::Color;
		clears = true;
	}

	if (descriptor.depth.enabled and descriptor.depth.loadAction == Types::LoadAction::Clear) {
		glClearDepthf(descriptor.depth.clearDepth);
		glClearStencil(static_cast<GLint>(descriptor.depth.clearStencil));

		// The stencil bit is harmless when the framebuffer has no stencil buffer: GL ignores bits it cannot honour.
		clearMask = clearMask | Types::BufferBit::Depth | Types::BufferBit::Stencil;
		clears = true;
	}

	if (not clears)
		return;

	// glClear obeys the write masks and the scissor test, so a depth mask left false by the previous frame's pipeline
	// would make the depth clear do nothing at all, in silence. They are forced rather than saved and restored:
	// reading GL state back with glGet stalls the pipeline, and a pass is entitled to start from the defaults.
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0xFF);
	glDisable(GL_SCISSOR_TEST);

	glClear(static_cast<GLbitfield>(Types::ToInt(clearMask)));
}

}

std::shared_ptr<I_ShaderModule> OpenGlGraphicDevice::CreateShaderModule(const ShaderModuleDescriptor& descriptor) {
	return std::make_shared<OpenGlShaderModule>(descriptor);
}

std::shared_ptr<I_PipelineState> OpenGlGraphicDevice::CreatePipelineState(const PipelineDescriptor& descriptor) {
	return std::make_shared<OpenGlPipelineState>(descriptor);
}

std::shared_ptr<I_IndexBuffer> OpenGlGraphicDevice::CreateIndexBuffer(const std::span<const u32> indices) {
	return std::make_shared<OpenGlIndexBuffer>(indices.data(), indices.size());
}

std::shared_ptr<I_VertexBuffer> OpenGlGraphicDevice::CreateVertexBuffer(const std::span<const f32> data, const BufferLayout& layout) {
	return std::make_shared<OpenGlVertexBuffer>(data.data(), data.size(), layout);
}

std::unique_ptr<I_CommandEncoder> OpenGlGraphicDevice::BeginRenderPass(const RenderPassDescriptor& descriptor) {
	assert(descriptor.colorCount <= Types::MAX_COLOR_ATTACHMENTS && "OpenGlGraphicDevice::BeginRenderPass: More colour attachments than the APIs allow.");

	if (descriptor.width == 0 or descriptor.height == 0) [[unlikely]] {
		// A zero-sized pass is what a minimised window produces; drawing into it is wasted work, not an error.
		CE_CORE_WARN("OpenGlGraphicDevice::BeginRenderPass: The render area is {}x{}; the pass is skipped.", descriptor.width, descriptor.height);
		return nullptr;
	}

	// Render-to-texture lands here once there is a texture type: for now every attachment is the default framebuffer,
	// which is the window's back buffer.
	assert(descriptor.colors[0].target == nullptr && descriptor.depth.target == nullptr && "OpenGlGraphicDevice::BeginRenderPass: Rendering to a texture is not supported yet.");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ApplyLoadActions(descriptor);

	// Store actions have no OpenGL equivalent before 4.3's glInvalidateFramebuffer, and macOS stops at 4.1: the
	// results are always kept, whatever the descriptor asks for.
	return std::make_unique<OpenGlCommandEncoder>(descriptor.width, descriptor.height);
}

}
