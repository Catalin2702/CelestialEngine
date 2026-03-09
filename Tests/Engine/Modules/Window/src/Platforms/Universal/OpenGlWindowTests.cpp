//
// Module: CelestialEngine/Tests/Engine/Modules/Window
// File: OpenGlWindowTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#include <Events/I_Event.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>
#include <Window/Platforms/Universal/OpenGlWindow.hpp>

#include <gtest/gtest.h>

using namespace CE::Tools::Log;
using namespace CE::Types::Window;
using namespace CE::Window;
using namespace CE::Events;

/**
 * @brief Test fixture for OpenGL tests
 */
class OpenGlWindowTest: public ::testing::Test {
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
 * @brief Test that OpenGlWindow can be constructed with default properties
 */
TEST_F(OpenGlWindowTest, Constructor_DefaultProperties_CreatesWindow) {
	const WindowProps props{"Test-Window", 800, 600, false, GraphicsApi::OpenGL};

	EXPECT_NO_THROW({
		OpenGlWindow window(props);
	});
}

/**
 * @brief Test that OpenGlWindow constructor with valid properties succeeds
 */
TEST_F(OpenGlWindowTest, Constructor_ValidProperties_Succeeds) {
	const WindowProps props{"OpenGL Test Window", 1024, 768, true, GraphicsApi::OpenGL};

	EXPECT_NO_THROW({
		OpenGlWindow window(props);
	});
}

// ============================================================================
// Getter Tests
// ============================================================================

/**
 * @brief Test that GetWidth returns the correct width
 */
TEST_F(OpenGlWindowTest, GetWidth_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Width Test", 1280, 720, false, GraphicsApi::OpenGL};
	const OpenGlWindow window(props);

	EXPECT_EQ(window.GetWidth(), 1280);
}

/**
 * @brief Test that GetHeight returns the correct height
 */
TEST_F(OpenGlWindowTest, GetHeight_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Height Test", 1280, 720, false, GraphicsApi::OpenGL};
	const OpenGlWindow window(props);

	EXPECT_EQ(window.GetHeight(), 720);
}

/**
 * @brief Test that IsVSync returns the correct VSync state
 */
TEST_F(OpenGlWindowTest, IsVSync_VSyncEnabled_ReturnsTrue) {
	const WindowProps props{"VSync Test", 800, 600, true, GraphicsApi::OpenGL};
	const OpenGlWindow window(props);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that IsVSync returns false when VSync is disabled
 */
TEST_F(OpenGlWindowTest, IsVSync_VSyncDisabled_ReturnsFalse) {
	const WindowProps props{"VSync Test", 800, 600, false, GraphicsApi::OpenGL};
	const OpenGlWindow window(props);

	EXPECT_FALSE(window.IsVSync());
}

/**
 * @brief Test that GetGLFWwindow returns a valid pointer
 */
TEST_F(OpenGlWindowTest, GetGLFWwindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"GLFW Test", 800, 600, false, GraphicsApi::OpenGL};
	const OpenGlWindow window(props);

	EXPECT_NE(window.GetNativeWindow(), nullptr);
}

// ============================================================================
// Setter Tests
// ============================================================================

/**
 * @brief Test that SetWidth updates the width correctly
 */
TEST_F(OpenGlWindowTest, SetWidth_NewValue_UpdatesWidth) {
	const WindowProps props{"Width Setter Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);
	window.SetWidth(1920);

	EXPECT_EQ(window.GetWidth(), 1920);
}

/**
 * @brief Test that SetHeight updates the height correctly
 */
TEST_F(OpenGlWindowTest, SetHeight_NewValue_UpdatesHeight) {
	const WindowProps props{"Height Setter Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);
	window.SetHeight(1080);

	EXPECT_EQ(window.GetHeight(), 1080);
}

/**
 * @brief Test that SetVSync updates the VSync state
 */
TEST_F(OpenGlWindowTest, SetVSync_EnableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);
	window.SetVSync(true);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that SetVSync can disable VSync
 */
TEST_F(OpenGlWindowTest, SetVSync_DisableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, true, GraphicsApi::OpenGL};
	OpenGlWindow window(props);
	window.SetVSync(false);

	EXPECT_FALSE(window.IsVSync());
}

// ============================================================================
// OnUpdate Tests
// ============================================================================

/**
 * @brief Test that OnUpdate can be called without errors
 */
TEST_F(OpenGlWindowTest, OnUpdate_Called_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);

	// Set an event callback before calling OnUpdate
	window.SetEventCallback([](I_Event&) {});

	EXPECT_NO_THROW(window.OnUpdate());
}

/**
 * @brief Test that OnUpdate can be called multiple times
 */
TEST_F(OpenGlWindowTest, OnUpdate_MultipleCalls_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);

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
TEST_F(OpenGlWindowTest, SetEventCallback_WithCallback_NoThrow) {
	const WindowProps props{"Callback Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);

	EventCallbackFn callback = []([[maybe_unused]] CE::Events::I_Event& e) {
		// Empty callback
	};

	EXPECT_NO_THROW(window.SetEventCallback(callback));
}

/**
 * @brief Test that SetWindowCallbacks can be called
 */
TEST_F(OpenGlWindowTest, SetWindowCallbacks_Called_NoThrow) {
	const WindowProps props{"Callbacks Test", 800, 600, false, GraphicsApi::OpenGL};
	OpenGlWindow window(props);

	EXPECT_NO_THROW(window.SetWindowCallbacks());
}

// ============================================================================
// Different Window Sizes Tests
// ============================================================================

/**
 * @brief Test that small window can be created
 */
TEST_F(OpenGlWindowTest, Constructor_SmallWindow_Succeeds) {
	const WindowProps props{"Small Window", 320, 240, false, GraphicsApi::OpenGL};

	EXPECT_NO_THROW({
		const OpenGlWindow window(props);
		EXPECT_EQ(window.GetWidth(), 320);
		EXPECT_EQ(window.GetHeight(), 240);
	});
}

/**
 * @brief Test that large window can be created
 */
TEST_F(OpenGlWindowTest, Constructor_LargeWindow_Succeeds) {
	const WindowProps props{"Large Window", 2560, 1440, false, GraphicsApi::OpenGL};

	EXPECT_NO_THROW({
		const OpenGlWindow window(props);
		EXPECT_EQ(window.GetWidth(), 2560);
		EXPECT_EQ(window.GetHeight(), 1440);
	});
}

