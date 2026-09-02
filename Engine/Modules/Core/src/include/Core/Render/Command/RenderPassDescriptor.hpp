//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: RenderPassDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_RENDERPASSDESCRIPTOR_HPP
#define CE_CORE_RENDER_COMMAND_RENDERPASSDESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>

#include <array>


namespace CE::Core {

class I_Texture;

/**
 * @struct RenderPassColorAttachment
 * @brief One of the colour buffers a render pass writes to
 * @details Fragment shader output `location = i` lands in colour attachment `i`, so a pass with several attachments
 *			is how one rasterisation of the geometry fills several buffers at once. Each attachment gets its own load
 *			and store action because they rarely want the same treatment: a G-buffer target is cleared and stored,
 *			while an accumulation buffer is loaded and stored.
 */
struct CE_CORE_API RenderPassColorAttachment {
	/// Where the attachment renders to. Null means the swapchain's current back buffer, which stays the only option
	/// until there is a texture type to render into.
	const I_Texture* target = nullptr;

	Types::LoadAction loadAction = Types::LoadAction::Clear;
	Types::StoreAction storeAction = Types::StoreAction::Store;

	/// Used only when loadAction is Clear.
	glm::vec4 clearColor = {0.f, 0.f, 0.f, 1.f};
};

/**
 * @struct RenderPassDepthAttachment
 * @brief The depth/stencil buffer a render pass tests and writes against
 * @details Singular where the colour attachments are many: the hardware runs one depth test per fragment, no matter
 *			how many colour buffers that fragment ends up in. It defaults to being discarded at the end of the pass
 *			because a depth buffer is usually scaffolding for the pass rather than something a later pass reads.
 */
struct CE_CORE_API RenderPassDepthAttachment {
	/// Null means the swapchain's own depth buffer; see RenderPassColorAttachment::target.
	const I_Texture* target = nullptr;

	Types::LoadAction loadAction = Types::LoadAction::Clear;
	Types::StoreAction storeAction = Types::StoreAction::DontCare;

	/// Used only when loadAction is Clear. 1 is the far plane under a conventional projection, 0 under a reversed-Z one.
	f32 clearDepth = 1.f;
	u32 clearStencil = 0;

	/// False for a pass that does no depth testing at all, such as a full-screen composite.
	bool enabled = true;
};

/**
 * @struct RenderPassDescriptor
 * @brief The complete set of attachments a batch of draw calls writes to
 * @details This is the object BeginRenderPass consumes, and the reason render passes exist as a concept at all: a
 *			tile-based GPU has to know every attachment up front, because it allocates tile memory for all of them,
 *			runs every draw of the pass tile by tile, and only then writes the results out. OpenGL, which predates
 *			that hardware, lets buffers be bound ad hoc instead - which is why the OpenGL backend can implement this
 *			as a framebuffer bind plus a glClear, and ignores half of what the descriptor says.
 */
struct CE_CORE_API RenderPassDescriptor {
	std::array<RenderPassColorAttachment, Types::MAX_COLOR_ATTACHMENTS> colors;
	u32 colorCount = 1;

	RenderPassDepthAttachment depth;

	/// Size of the region being rendered into: Vulkan's renderArea, and the target height a Viewport needs to flip
	/// its origin for OpenGL.
	u32 width = 0;
	u32 height = 0;
};

}

#endif //CE_CORE_RENDER_COMMAND_RENDERPASSDESCRIPTOR_HPP
