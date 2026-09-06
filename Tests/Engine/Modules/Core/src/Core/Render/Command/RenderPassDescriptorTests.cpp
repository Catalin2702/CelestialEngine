//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Command
// File: RenderPassDescriptorTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include <Core/Render/Command/RenderPassDescriptor.hpp>

#include <gtest/gtest.h>

using CE::Core::RenderPassColorAttachment;
using CE::Core::RenderPassDepthAttachment;
using CE::Core::RenderPassDescriptor;

namespace {

/**
 * @brief Test fixture for the attachment set a pass is opened with
 */
class RenderPassDescriptorTest: public ::testing::Test {};

}

/**
 * @brief Test that an attachment nobody aimed anywhere means the swapchain's back buffer
 * @details The contract the whole offscreen path rests on: the renderer's own passes name a texture, and only the
 *			composite leaves the target null. A backend that reads the drawable regardless of this field renders
 *			every pass into the back buffer while the scene texture stays untouched.
 */
TEST_F(RenderPassDescriptorTest, DefaultColorAttachment_TargetsTheBackBuffer) {
	constexpr RenderPassColorAttachment attachment{};

	EXPECT_EQ(attachment.target, nullptr);
}

/**
 * @brief Test that a colour attachment is cleared and kept unless it is told otherwise
 * @details The safe pair: a pass that forgets to say leaves the previous frame's contents unread and its own
 *			results written, which is wasteful but never wrong.
 */
TEST_F(RenderPassDescriptorTest, DefaultColorAttachment_ClearsAndStores) {
	constexpr RenderPassColorAttachment attachment{};

	EXPECT_EQ(attachment.loadAction, CE::Types::LoadAction::Clear);
	EXPECT_EQ(attachment.storeAction, CE::Types::StoreAction::Store);

	EXPECT_EQ(attachment.clearColor.a, 1.f);
}

/**
 * @brief Test that the depth attachment defaults to being scaffolding for its own pass
 * @details Cleared at the start and discarded at the end, because a depth buffer is usually not read by anything
 *			after the pass that wrote it - which is what lets a tile-based GPU keep it in tile memory throughout.
 */
TEST_F(RenderPassDescriptorTest, DefaultDepthAttachment_ClearsAndDiscards) {
	constexpr RenderPassDepthAttachment attachment{};

	EXPECT_TRUE(attachment.enabled);
	EXPECT_EQ(attachment.target, nullptr);
	EXPECT_EQ(attachment.loadAction, CE::Types::LoadAction::Clear);
	EXPECT_EQ(attachment.storeAction, CE::Types::StoreAction::DontCare);

	// The far plane under a conventional projection. A reversed-Z pass has to say so.
	EXPECT_EQ(attachment.clearDepth, 1.f);
}

/**
 * @brief Test that a descriptor describes a single-target pass until it is told about more
 * @details colorCount, not the array's length, is what a backend loops over: leaving it at the array's size would
 *			make every pass attach eight buffers it never asked for.
 */
TEST_F(RenderPassDescriptorTest, DefaultDescriptor_DescribesOneColorAttachment) {
	constexpr RenderPassDescriptor descriptor{};

	EXPECT_EQ(descriptor.colorCount, 1u);
	EXPECT_LT(descriptor.colorCount, descriptor.colors.size());
	EXPECT_EQ(descriptor.colors.size(), CE::Types::MAX_COLOR_ATTACHMENTS);

	// Zero, so a pass opened without a size is skipped rather than rendered into a one-pixel area.
	EXPECT_EQ(descriptor.width, 0u);
	EXPECT_EQ(descriptor.height, 0u);
}

/**
 * @brief Test that the composite's shape is expressible: back buffer, nothing loaded, no depth
 * @details The one pass in the engine that writes the drawable. It is spelled out here because every field of it is
 *			a deliberate departure from the defaults above.
 */
TEST_F(RenderPassDescriptorTest, CompositeShape_IsABackBufferPassWithNoDepth) {
	RenderPassDescriptor descriptor{};
	descriptor.width = 1280;
	descriptor.height = 720;

	descriptor.colors[0].target = nullptr;
	descriptor.colors[0].loadAction = CE::Types::LoadAction::DontCare;
	descriptor.colors[0].storeAction = CE::Types::StoreAction::Store;

	descriptor.depth.enabled = false;

	EXPECT_EQ(descriptor.colors[0].target, nullptr);
	EXPECT_EQ(descriptor.colors[0].loadAction, CE::Types::LoadAction::DontCare);
	EXPECT_FALSE(descriptor.depth.enabled);

	// The attachments past the first stay untouched, so a backend that trusts colorCount reads none of them.
	EXPECT_EQ(descriptor.colorCount, 1u);
	EXPECT_EQ(descriptor.colors[1].target, nullptr);
	EXPECT_EQ(descriptor.colors[1].loadAction, CE::Types::LoadAction::Clear);
}
