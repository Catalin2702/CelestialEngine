//
// Module: Tests/Engine/Window
// File: MetalWindowTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Types/Window/WindowProps.hpp>
#include <Window/Platforms/Mac/MetalWindow.hpp>

#include <gtest/gtest.h>

using namespace CE::Window;
using namespace CE::Types::Window;

/**
 * @brief Test fixture for MetalWindow tests
 */
class MetalWindowTest : public ::testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}
};

// ============================================================================
// Constructor Tests
// ============================================================================

/**
 * @brief Test that MetalWindow can be constructed with default properties
 */
TEST_F(MetalWindowTest, Constructor_DefaultProperties_CreatesWindow) {
	const WindowProps props{"Test Window", 800, 600, false, GraphicsApi::Metal};

	EXPECT_NO_THROW({
		MetalWindow window(props);
	});
}

/**
 * @brief Test that MetalWindow constructor with valid properties succeeds
 */
TEST_F(MetalWindowTest, Constructor_ValidProperties_Succeeds) {
	const WindowProps props{"Metal Test Window", 1024, 768, true, GraphicsApi::Metal};

	EXPECT_NO_THROW({
		MetalWindow window(props);
	});
}

// ============================================================================
// Getter Tests
// ============================================================================

/**
 * @brief Test that GetWidth returns the correct width
 */
TEST_F(MetalWindowTest, GetWidth_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Width Test", 1280, 720, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_EQ(window.GetWidth(), 1280);
}

/**
 * @brief Test that GetHeight returns the correct height
 */
TEST_F(MetalWindowTest, GetHeight_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Height Test", 1280, 720, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_EQ(window.GetHeight(), 720);
}

/**
 * @brief Test that IsVSync returns the correct VSync state
 */
TEST_F(MetalWindowTest, IsVSync_VSyncEnabled_ReturnsTrue) {
	const WindowProps props{"VSync Test", 800, 600, true, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that IsVSync returns false when VSync is disabled
 */
TEST_F(MetalWindowTest, IsVSync_VSyncDisabled_ReturnsFalse) {
	const WindowProps props{"VSync Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_FALSE(window.IsVSync());
}

/**
 * @brief Test that GetGLFWwindow returns a valid pointer
 */
TEST_F(MetalWindowTest, GetGLFWwindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"GLFW Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetGLFWwindow(), nullptr);
}

// ============================================================================
// Metal-Specific Getter Tests
// ============================================================================

/**
 * @brief Test that GetCommandQueue returns a valid pointer
 */
TEST_F(MetalWindowTest, GetCommandQueue_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Command Queue Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetCommandQueue(), nullptr);
}

/**
 * @brief Test that GetDevice returns a valid pointer
 */
TEST_F(MetalWindowTest, GetDevice_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Device Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetDevice(), nullptr);
}

/**
 * @brief Test that GetMetalLayer returns a valid pointer
 */
TEST_F(MetalWindowTest, GetMetalLayer_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal Layer Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetMetalLayer(), nullptr);
}

/**
 * @brief Test that GetMetalWindow returns a valid pointer
 */
TEST_F(MetalWindowTest, GetMetalWindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal Window Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetMetalWindow(), nullptr);
}

// ============================================================================
// Setter Tests
// ============================================================================

/**
 * @brief Test that SetWidth updates the width correctly
 */
TEST_F(MetalWindowTest, SetWidth_NewValue_UpdatesWidth) {
	const WindowProps props{"Width Setter Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);
	window.SetWidth(1920);

	EXPECT_EQ(window.GetWidth(), 1920);
}

/**
 * @brief Test that SetHeight updates the height correctly
 */
TEST_F(MetalWindowTest, SetHeight_NewValue_UpdatesHeight) {
	const WindowProps props{"Height Setter Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);
	window.SetHeight(1080);

	EXPECT_EQ(window.GetHeight(), 1080);
}

/**
 * @brief Test that SetVSync updates the VSync state
 */
TEST_F(MetalWindowTest, SetVSync_EnableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);
	window.SetVSync(true);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that SetVSync can disable VSync
 */
TEST_F(MetalWindowTest, SetVSync_DisableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, true, GraphicsApi::Metal};
	MetalWindow window(props);
	window.SetVSync(false);

	EXPECT_FALSE(window.IsVSync());
}

// ============================================================================
// OnUpdate Tests
// ============================================================================

/**
 * @brief Test that OnUpdate can be called without errors
 */
TEST_F(MetalWindowTest, OnUpdate_Called_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NO_THROW(window.OnUpdate());
}

/**
 * @brief Test that OnUpdate can be called multiple times
 */
TEST_F(MetalWindowTest, OnUpdate_MultipleCalls_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

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
TEST_F(MetalWindowTest, SetEventCallback_WithCallback_NoThrow) {
	const WindowProps props{"Callback Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EventCallbackFn callback = [](CE::Events::I_Event& e) {
		// Empty callback
	};

	EXPECT_NO_THROW(window.SetEventCallback(callback));
}

/**
 * @brief Test that SetWindowCallbacks can be called
 */
TEST_F(MetalWindowTest, SetWindowCallbacks_Called_NoThrow) {
	const WindowProps props{"Callbacks Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NO_THROW(window.SetWindowCallbacks());
}

// ============================================================================
// Different Window Sizes Tests
// ============================================================================

/**
 * @brief Test that small window can be created
 */
TEST_F(MetalWindowTest, Constructor_SmallWindow_Succeeds) {
	const WindowProps props{"Small Window", 320, 240, false, GraphicsApi::Metal};

	EXPECT_NO_THROW({
		MetalWindow window(props);
		EXPECT_EQ(window.GetWidth(), 320);
		EXPECT_EQ(window.GetHeight(), 240);
	});
}

/**
 * @brief Test that large window can be created
 */
TEST_F(MetalWindowTest, Constructor_LargeWindow_Succeeds) {
	const WindowProps props{"Large Window", 2560, 1440, false, GraphicsApi::Metal};

	EXPECT_NO_THROW({
		MetalWindow window(props);
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
TEST_F(MetalWindowTest, MetalResources_AfterConstruction_AllValid) {
	const WindowProps props{"Resources Test", 800, 600, false, GraphicsApi::Metal};
	MetalWindow window(props);

	EXPECT_NE(window.GetDevice(), nullptr);
	EXPECT_NE(window.GetCommandQueue(), nullptr);
	EXPECT_NE(window.GetMetalLayer(), nullptr);
	EXPECT_NE(window.GetMetalWindow(), nullptr);
	EXPECT_NE(window.GetGLFWwindow(), nullptr);
}

