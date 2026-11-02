//
// Module: CelestialEngine/Engine/Tests/Modules/Core
// File: CoreHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

// Every umbrella of Core, on its own and together: each one has to compile with nothing included before it, and none
// of them may clash with another. The checks below only prove that a type from each part is reachable through it.
#include <Core/Hub/Hub.hpp>
#include <Core/Layers/ImGui/ImGui.hpp>
#include <Core/Layers/Layers.hpp>
#include <Core/Render/Buffer/Buffer.hpp>
#include <Core/Render/Camera/Controller/Controller.hpp>
#include <Core/Render/Camera/Projection/Projection.hpp>
#include <Core/Render/Command/Command.hpp>
#include <Core/Render/Device/Device.hpp>
#include <Core/Render/Pipeline/Pipeline.hpp>
#include <Core/Render/Render.hpp>
#include <Core/Render/Renderer/Renderer.hpp>
#include <Core/Render/Shader/Shader.hpp>
#include <Core/Render/Surface/Surface.hpp>
#include <Core/Render/Swapchain/Swapchain.hpp>
#include <Core/Render/Texture/Texture.hpp>
#include <Core/RunLoop/RunLoop.hpp>
#include <Core/Window/Window.hpp>

#include <Core/Core.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the Core umbrellas expose a type from every sub-module
 */
TEST(CoreHeadersTest, Umbrellas_ExposeEverySubModule) {
	EXPECT_TRUE(std::is_class_v<CE::Core::Application>);
	EXPECT_TRUE(std::is_class_v<CE::Core::I_EventHubDispatcher>);
	EXPECT_TRUE(std::is_class_v<CE::Core::Input>);
	EXPECT_TRUE(std::is_class_v<CE::Core::KeyShortcut>);
	EXPECT_TRUE(std::is_class_v<CE::Core::LayerStack>);
	EXPECT_TRUE(std::is_class_v<CE::Core::Camera>);
	EXPECT_TRUE(std::is_class_v<CE::Core::FreeCameraController>);
	EXPECT_TRUE(std::is_class_v<CE::Core::ForwardRenderer>);
	EXPECT_TRUE(std::is_class_v<CE::Core::I_Window>);

	// The backends' own types come along only when the backend is compiled in (CE_RENDER_BACKENDS).
#if CE_WINDOW_GLFW
	EXPECT_TRUE(std::is_class_v<CE::Core::GlfwEventHubDispatcher>);
	EXPECT_TRUE(std::is_class_v<CE::Core::GlfwWindow>);
#endif

#if CE_WINDOW_COCOA
	EXPECT_TRUE(std::is_class_v<CE::Core::CocoaWindow>);
	EXPECT_TRUE(std::is_class_v<CE::Core::CocoaEventHubDispatcher>);
#endif

#if CE_RENDER_OPENGL
	EXPECT_TRUE(std::is_class_v<CE::Core::OpenGlGraphicDevice>);
	EXPECT_TRUE(std::is_class_v<CE::Core::OpenGlSwapchain>);
#endif

#if CE_RENDER_METAL
	EXPECT_TRUE(std::is_class_v<CE::Core::MetalGraphicDevice>);
	EXPECT_TRUE(std::is_class_v<CE::Core::MetalSwapchain>);
#endif
}
