//
// Module: CelestialEngine/Engine/Tests/Runtime/RHI
// File: RHIHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

// Every umbrella of the RHI, on its own and together: each one has to compile with nothing included before it - in
// particular with no Core header at all, which is what being a module below Core means.
#include <RHI/Buffer/Buffer.hpp>
#include <RHI/Command/Command.hpp>
#include <RHI/Device/Device.hpp>
#include <RHI/Pipeline/Pipeline.hpp>
#include <RHI/Shader/Shader.hpp>
#include <RHI/Surface/Surface.hpp>
#include <RHI/Swapchain/Swapchain.hpp>
#include <RHI/Texture/Texture.hpp>

#include <RHI/RHI.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the RHI umbrella exposes a type from every part
 */
TEST(RHIHeadersTest, Umbrellas_ExposeEveryPart) {
	EXPECT_TRUE(std::is_class_v<CE::RHI::BufferLayout>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_CommandEncoder>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_GraphicDevice>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::PipelineDescriptor>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_ShaderModule>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_Surface>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_Swapchain>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::I_Texture>);

	// The backends' own types come along only when the backend is compiled in (CE_RENDER_BACKENDS).
#if CE_RENDER_OPENGL
	EXPECT_TRUE(std::is_class_v<CE::RHI::OpenGlGraphicDevice>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::OpenGlSwapchain>);
#endif

#if CE_RENDER_METAL
	EXPECT_TRUE(std::is_class_v<CE::RHI::MetalGraphicDevice>);
	EXPECT_TRUE(std::is_class_v<CE::RHI::MetalSwapchain>);
#endif
}

/**
 * @brief Test that every backend surface is an I_Surface, which is what MakeSwapchain receives
 * @details A backend surface that forgot the base would still compile on its own, and then fail at runtime with
 *			"does not offer a surface" the first time a window implementing it is handed to the renderer.
 */
TEST(RHIHeadersTest, BackendSurfaces_DeriveFromI_Surface) {
	EXPECT_TRUE(std::has_virtual_destructor_v<CE::RHI::I_Surface>);

#if CE_RENDER_OPENGL
	EXPECT_TRUE((std::is_base_of_v<CE::RHI::I_Surface, CE::RHI::I_OpenGlSurface>));
#endif

#if CE_RENDER_METAL
	EXPECT_TRUE((std::is_base_of_v<CE::RHI::I_Surface, CE::RHI::I_MetalSurface>));
#endif
}
