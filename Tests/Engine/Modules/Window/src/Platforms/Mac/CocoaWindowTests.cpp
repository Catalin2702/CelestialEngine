//
// Module: CelestialEngine/Tests/Engine/Modules/Window/Platforms/Mac
// File: CocoaWindowTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-18
// Updated by: Catalin Chirosca
// Updated: 2026-03-20
//

#include <Events/I_Event.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>
#include <Window/Platforms/Mac/CocoaWindow.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;
using namespace CE::Tools::Log;
using namespace CE::Types::Render;
using namespace CE::Types::Window;
using namespace CE::Window;

/**
 * @brief Test fixture for CocoaWindow tests
 */
class CocoaWindowTest: public ::testing::Test {
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
 * @brief Test that CocoaWindow can be constructed with default properties
 */
TEST_F(CocoaWindowTest, Constructor_DefaultProperties_CreatesWindow) {
	const WindowProps props{"Test-Window", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_NO_THROW({
		const CocoaWindow window(props);
	});
}

/**
 * @brief Test that CocoaWindow constructor with valid properties succeeds
 */
TEST_F(CocoaWindowTest, Constructor_ValidProperties_Succeeds) {
	const WindowProps props{"Metal Test Window", 1024, 768, true, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_NO_THROW({
		const CocoaWindow window(props);
	});
}

// ============================================================================
// Getter Tests
// ============================================================================

/**
 * @brief Test that GetWidth returns the correct width
 */
TEST_F(CocoaWindowTest, GetWidth_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Width Test", 1280, 720, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_EQ(window.GetWidth(), 1280);
}

/**
 * @brief Test that GetHeight returns the correct height
 */
TEST_F(CocoaWindowTest, GetHeight_AfterConstruction_ReturnsCorrectValue) {
	const WindowProps props{"Height Test", 1280, 720, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_EQ(window.GetHeight(), 720);
}

/**
 * @brief Test that GetSize returns the correct dimensions
 */
TEST_F(CocoaWindowTest, GetSize_AfterConstruction_ReturnsCorrectValues) {
	const WindowProps props{"Size Test", 1280, 720, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	const auto [width, height] = window.GetSize();
	EXPECT_EQ(width, 1280);
	EXPECT_EQ(height, 720);
}

/**
 * @brief Test that IsVSync returns the correct VSync state
 */
TEST_F(CocoaWindowTest, IsVSync_VSyncEnabled_ReturnsTrue) {
	const WindowProps props{"VSync Test", 800, 600, true, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that IsVSync returns false when VSync is disabled
 */
TEST_F(CocoaWindowTest, IsVSync_VSyncDisabled_ReturnsFalse) {
	const WindowProps props{"VSync Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_FALSE(window.IsVSync());
}

/**
 * @brief Test that GetNativeWindow returns a valid pointer
 */
TEST_F(CocoaWindowTest, GetNativeWindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Native Window Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_NE(window.GetNativeWindow(), nullptr);
}

// ============================================================================
// Metal-Specific Getter Tests
// ============================================================================

/**
 * @brief Test that GetMetalWindow returns a valid pointer
 */
TEST_F(CocoaWindowTest, GetMetalWindow_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal Window Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_NE(window.GetMetalWindow(), nullptr);
}

/**
 * @brief Test that GetContentView returns a valid pointer
 */
TEST_F(CocoaWindowTest, GetMetalView_AfterConstruction_ReturnsValidPointer) {
	const WindowProps props{"Metal View Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_NE(window.GetMetalView(), nullptr);
}

// ============================================================================
// Setter Tests
// ============================================================================

/**
 * @brief Test that SetWidth updates the width correctly
 */
TEST_F(CocoaWindowTest, SetWidth_NewValue_UpdatesWidth) {
	const WindowProps props{"Width Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);
	window.SetWidth(1920);

	EXPECT_EQ(window.GetWidth(), 1920);
}

/**
 * @brief Test that SetHeight updates the height correctly
 */
TEST_F(CocoaWindowTest, SetHeight_NewValue_UpdatesHeight) {
	const WindowProps props{"Height Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);
	window.SetHeight(1080);

	EXPECT_EQ(window.GetHeight(), 1080);
}

/**
 * @brief Test that SetVSync updates the VSync state
 */
TEST_F(CocoaWindowTest, SetVSync_EnableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);
	window.SetVSync(true);

	EXPECT_TRUE(window.IsVSync());
}

/**
 * @brief Test that SetVSync can disable VSync
 */
TEST_F(CocoaWindowTest, SetVSync_DisableVSync_UpdatesState) {
	const WindowProps props{"VSync Setter Test", 800, 600, true, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);
	window.SetVSync(false);

	EXPECT_FALSE(window.IsVSync());
}

// ============================================================================
// OnUpdate Tests
// ============================================================================

/**
 * @brief Test that OnUpdate can be called without errors
 */
TEST_F(CocoaWindowTest, OnUpdate_Called_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);

	// Set an event callback before calling OnUpdate
	window.SetEventCallback([](I_Event&) {});

	EXPECT_NO_THROW(window.OnUpdate());
}

/**
 * @brief Test that OnUpdate can be called multiple times
 */
TEST_F(CocoaWindowTest, OnUpdate_MultipleCalls_NoThrow) {
	const WindowProps props{"Update Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);

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
TEST_F(CocoaWindowTest, SetEventCallback_WithCallback_NoThrow) {
	const WindowProps props{"Callback Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);

	EventCallbackFn callback = []([[maybe_unused]] CE::Events::I_Event& e) {
		// Empty callback
	};

	EXPECT_NO_THROW(window.SetEventCallback(callback));
}

// ============================================================================
// Different Window Sizes Tests
// ============================================================================

/**
 * @brief Test that small window can be created
 */
TEST_F(CocoaWindowTest, Constructor_SmallWindow_Succeeds) {
	const WindowProps props{"Small Window", 320, 240, false, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_NO_THROW({
		const CocoaWindow window(props);
		EXPECT_EQ(window.GetWidth(), 320);
		EXPECT_EQ(window.GetHeight(), 240);
	});
}

/**
 * @brief Test that large window can be created
 */
TEST_F(CocoaWindowTest, Constructor_LargeWindow_Succeeds) {
	const WindowProps props{"Large Window", 2560, 1440, false, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_NO_THROW({
		const CocoaWindow window(props);
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
TEST_F(CocoaWindowTest, MetalResources_AfterConstruction_AllValid) {
	const WindowProps props{"Resources Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_NE(window.GetMetalWindow(), nullptr);
	EXPECT_NE(window.GetNativeWindow(), nullptr);
	EXPECT_NE(window.GetMetalView(), nullptr);
}

/**
 * @brief Test that GetMetalWindow and GetNativeWindow return the same pointer
 */
TEST_F(CocoaWindowTest, NativeWindowPointers_Consistent) {
	const WindowProps props{"Pointer Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const CocoaWindow window(props);

	EXPECT_EQ(window.GetMetalWindow(), window.GetNativeWindow());
}

// ============================================================================
// Lifecycle Tests
// ============================================================================

/**
 * @brief Test creating and destroying multiple windows
 */
TEST_F(CocoaWindowTest, MultipleWindows_SequentialCreation_Succeeds) {
	const WindowProps props1{"Window 1", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const WindowProps props2{"Window 2", 1024, 768, false, GraphicsApi::Metal, WindowApi::Cocoa};

	EXPECT_NO_THROW({
		const CocoaWindow window1(props1);
		EXPECT_EQ(window1.GetWidth(), 800);
	});

	EXPECT_NO_THROW({
		const CocoaWindow window2(props2);
		EXPECT_EQ(window2.GetWidth(), 1024);
	});
}

/**
 * @brief Test full lifecycle with events and updates
 */
TEST_F(CocoaWindowTest, FullLifecycle_WithEventsAndUpdates_Succeeds) {
	const WindowProps props{"Lifecycle Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};
	CocoaWindow window(props);

	// Set event callback
	bool eventCallbackSet = false;
	window.SetEventCallback([&eventCallbackSet](I_Event&) {
		eventCallbackSet = true;
	});

	// Update window
	EXPECT_NO_THROW(window.OnUpdate());

	// Change properties
	window.SetWidth(1024);
	window.SetHeight(768);
	window.SetVSync(true);

	EXPECT_EQ(window.GetWidth(), 1024);
	EXPECT_EQ(window.GetHeight(), 768);
	EXPECT_TRUE(window.IsVSync());

	// Update again
	EXPECT_NO_THROW(window.OnUpdate());
}

