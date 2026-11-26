//
// Module: CelestialEngine/Engine/Tests/Runtime/Core
// File: CoreHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

// Every umbrella of Core, on its own and together: each one has to compile with nothing included before it, and none
// of them may clash with another. The checks below only prove that a type from each part is reachable through it.
#include <Core/Hub/Hub.hpp>
#include <Core/Layers/ImGui/ImGui.hpp>
#include <Core/Layers/Layers.hpp>
#include <Core/Render/Camera/Controller/Controller.hpp>
#include <Core/Render/Camera/Projection/Projection.hpp>
#include <Core/Render/Render.hpp>
#include <Core/Render/Renderer/Renderer.hpp>
#include <Core/RunLoop/RunLoop.hpp>
#include <Core/Window/Window.hpp>
#include <RHI/Surface/I_Surface.hpp>

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
}

/**
 * @brief Test that every window a renderer can draw into is an RHI surface
 * @details I_Renderer::MakeRenderer asks the window for its RHI::I_Surface; a window that is not one would only be
 *			found out at runtime, when the renderer is created.
 */
TEST(CoreHeadersTest, Windows_AreRhiSurfaces) {
#if CE_WINDOW_GLFW
	EXPECT_TRUE((std::is_base_of_v<CE::RHI::I_Surface, CE::Core::GlfwWindow>));
#endif

#if CE_WINDOW_COCOA
	EXPECT_TRUE((std::is_base_of_v<CE::RHI::I_Surface, CE::Core::CocoaWindow>));
#endif

	SUCCEED();
}
