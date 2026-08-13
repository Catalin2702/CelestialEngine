//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render/Platforms/Common/OpenGl
// File: OpenGlTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Types/Render/Platforms/Common/OpenGl/OpenGl.hpp>

#include <glad/glad.h>
#include <gtest/gtest.h>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the OpenGL type mappings
 */
class OpenGlTypesTest: public ::testing::Test {};

}

/**
 * @brief Test that each buffer bit matches the OpenGL constant it stands for
 * @details The enum hard-codes the GL values so the render code can build clear masks without including glad in
 *			every header: if the two ever drift apart the frame would be cleared with the wrong buffers.
 */
TEST_F(OpenGlTypesTest, BufferBit_MatchesTheOpenGlConstants) {
	EXPECT_EQ(ToInt(BufferBit::Color), GL_COLOR_BUFFER_BIT);
	EXPECT_EQ(ToInt(BufferBit::Depth), GL_DEPTH_BUFFER_BIT);
	EXPECT_EQ(ToInt(BufferBit::Stencil), GL_STENCIL_BUFFER_BIT);
}

/**
 * @brief Test that the buffer bits can be combined into a clear mask
 */
TEST_F(OpenGlTypesTest, BufferBit_CombinesIntoAClearMask) {
	const int mask = ToInt(BufferBit::Color) | ToInt(BufferBit::Depth);

	EXPECT_EQ(mask, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	EXPECT_TRUE(mask & GL_COLOR_BUFFER_BIT);
	EXPECT_FALSE(mask & GL_STENCIL_BUFFER_BIT);
}
