//
// Module: CelestialEngine/Tests/Engine/Modules/Window/Platforms/Mac
// File: MetalGlfwWindowTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include <Events/I_Event.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>
#include <Window/Platforms/Mac/MetalGlfwWindow.hpp>

#include <gtest/gtest.h>

using namespace CE::Tools::Log;
using namespace CE::Types::Window;
using namespace CE::Window;
using namespace CE::Events;

/**
 * @brief Test fixture for MetalGlfwWindow tests
 */
class MetalGlfwWindowTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}
	void TearDown() override {
		Log::Shutdown();
	}
};

// ============================================================================
// Constructor Tests
// ============================================================================

/**
 * @brief Test that MetalGlfwWindow can be constructed with default properties
 */
TEST_F(MetalGlfwWindowTest, Constructor_DefaultProperties_CreatesWindow) {
	const WindowProps props{"Test-Window", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};

	EXPECT_NO_THROW({
		const MetalGlfwWindow window(props);
	});
}

/**
 * @brief Test that MetalGlfwWindow constructor with valid properties succeeds
 */
TEST_F(MetalGlfwWindowTest, Constructor_ValidProperties_Succeeds) {
	const WindowProps props{"Metal Test Window", 1024, 768, true, GraphicsApi::Metal, WindowApi::GLFW};

	EXPECT_NO_THROW({
		const MetalGlfwWindow window(props);
	});
}

// ============================================================================
// Getter Tests
// ============================================================================

/**
 * @brief Test that GetWidth returns the correct width
 */
TEST_F(MetalGlfwWindowTest, GetWidth_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Width Test", 1280, 720, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_EQ(window.GetWidth(), 1280);
}

/**
 * @brief Test that GetHeight returns the correct height
 */
TEST_F(MetalGlfwWindowTest, GetHeight_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Height Test", 1280, 720, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_EQ(window.GetHeight(), 720);
}

/**
 * @brief Test that IsVSync returns the correct VSync state
 */
TEST_F(MetalGlfwWindowTest, IsVSync_VSyncEnabled_ReturnsTrue) {
	const WindowProps props{"VSync Test", 800, 600, true, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that IsVSync returns false when VSync is disabled
 */
TEST_F(MetalGlfwWindowTest, IsVSync_VSyncDisabled_ReturnsFalse) {
	const WindowProps props{"VSync Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_FALSE(window.IsVSync());
}

/**
 * @brief Test that GetGLFWwindow returns a valid pointer
 */
TEST_F(MetalGlfwWindowTest, GetGLFWwindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"GLFW Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetNativeWindow(), nullptr);
}

// ============================================================================
// Metal-Specific Getter Tests
// ============================================================================

/**
 * @brief Test that GetCommandQueue returns a valid pointer
 */
TEST_F(MetalGlfwWindowTest, GetCommandQueue_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Command Queue Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetCommandQueue(), nullptr);
}

/**
 * @brief Test that GetDevice returns a valid pointer
 */
TEST_F(MetalGlfwWindowTest, GetDevice_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Device Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetDevice(), nullptr);
}

/**
 * @brief Test that GetMetalLayer returns a valid pointer
 */
TEST_F(MetalGlfwWindowTest, GetMetalLayer_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal Layer Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetMetalLayer(), nullptr);
}

/**
 * @brief Test that GetMetalGlfwWindow returns a valid pointer
 */
TEST_F(MetalGlfwWindowTest, GetMetalGlfwWindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal Window Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetMetalWindow(), nullptr);
}

// ============================================================================
// Setter Tests
// ============================================================================

/**
 * @brief Test that SetWidth updates the width correctly
 */
TEST_F(MetalGlfwWindowTest, SetWidth_NewValue_UpdatesWidth) {
	const WindowProps props{"Width Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);
	window.SetWidth(1920);

	EXPECT_EQ(window.GetWidth(), 1920);
}

/**
 * @brief Test that SetHeight updates the height correctly
 */
TEST_F(MetalGlfwWindowTest, SetHeight_NewValue_UpdatesHeight) {
	const WindowProps props{"Height Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);
	window.SetHeight(1080);

	EXPECT_EQ(window.GetHeight(), 1080);
}

/**
 * @brief Test that SetVSync updates the VSync state
 */
TEST_F(MetalGlfwWindowTest, SetVSync_EnableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);
	window.SetVSync(true);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that SetVSync can disable VSync
 */
TEST_F(MetalGlfwWindowTest, SetVSync_DisableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, true, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);
	window.SetVSync(false);

	EXPECT_FALSE(window.IsVSync());
}

// ============================================================================
// OnUpdate Tests
// ============================================================================

/**
 * @brief Test that OnUpdate can be called without errors
 */
TEST_F(MetalGlfwWindowTest, OnUpdate_Called_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);

	// Set an event callback before calling OnUpdate
	window.SetEventCallback([](I_Event&) {});

	EXPECT_NO_THROW(window.OnUpdate());
}

/**
 * @brief Test that OnUpdate can be called multiple times
 */
TEST_F(MetalGlfwWindowTest, OnUpdate_MultipleCalls_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);

	// Set an event callback before calling OnUpdate
	window.SetEventCallback([](I_Event&) {});

	EXPECT_NO_THROW({
		window.OnUpdate();
		window.OnUpdate();
		window.OnUpdate();
	});
}

// ============================================================================
// Event Callback Tests
// ============================================================================

/**
 * @brief Test that SetEventCallback can be called
 */
TEST_F(MetalGlfwWindowTest, SetEventCallback_WithCallback_NoThrow) {
	const WindowProps props{"Callback Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);

	EventCallbackFn callback = []([[maybe_unused]] CE::Events::I_Event& e) {
		// Empty callback
	};

	EXPECT_NO_THROW(window.SetEventCallback(callback));
}

/**
 * @brief Test that SetWindowCallbacks can be called
 */
TEST_F(MetalGlfwWindowTest, SetWindowCallbacks_Called_NoThrow) {
	const WindowProps props{"Callbacks Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	MetalGlfwWindow window(props);

	EXPECT_NO_THROW(window.SetWindowCallbacks());
}

// ============================================================================
// Different Window Sizes Tests
// ============================================================================

/**
 * @brief Test that small window can be created
 */
TEST_F(MetalGlfwWindowTest, Constructor_SmallWindow_Succeeds) {
	const WindowProps props{"Small Window", 320, 240, false, GraphicsApi::Metal, WindowApi::GLFW};

	EXPECT_NO_THROW({
		const MetalGlfwWindow window(props);
		EXPECT_EQ(window.GetWidth(), 320);
		EXPECT_EQ(window.GetHeight(), 240);
	});
}

/**
 * @brief Test that large window can be created
 */
TEST_F(MetalGlfwWindowTest, Constructor_LargeWindow_Succeeds) {
	const WindowProps props{"Large Window", 2560, 1440, false, GraphicsApi::Metal, WindowApi::GLFW};

	EXPECT_NO_THROW({
		const MetalGlfwWindow window(props);
		EXPECT_EQ(window.GetWidth(), 2560);
		EXPECT_EQ(window.GetHeight(), 1440);
	});
}

// ============================================================================
// Metal Resources Tests
// ============================================================================

/**
 * @brief Test that all Metal resources are properly initialized
 */
TEST_F(MetalGlfwWindowTest, MetalResources_AfterConstruction_AllValid) {
	const WindowProps props{"Resources Test", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW};
	const MetalGlfwWindow window(props);

	EXPECT_NE(window.GetDevice(), nullptr);
	EXPECT_NE(window.GetCommandQueue(), nullptr);
	EXPECT_NE(window.GetMetalLayer(), nullptr);
	EXPECT_NE(window.GetMetalWindow(), nullptr);
	EXPECT_NE(window.GetNativeWindow(), nullptr);
}

