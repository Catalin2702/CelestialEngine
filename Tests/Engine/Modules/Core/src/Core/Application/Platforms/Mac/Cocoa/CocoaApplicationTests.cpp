//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Application
// File: CocoaApplicationTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-27
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#include <AppKit/Utility/AppKit.hpp>
#include <Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp>
#include <Core/Layers/I_Layer.hpp>
#include <Core/Window/I_Window.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>

using namespace CE::AppKit;
using namespace CE::Core::Application;
using namespace CE::Events;
using namespace CE::Core::Layers;
using namespace CE::Tools::Log;
using namespace CE::Types::Render;
using namespace CE::Types::Window;


/**
 * @brief Mock Layer for testing CocoaApplication (Cocoa + Metal)
 */
class MockCocoaAppLayer: public I_Layer {
public:
	explicit MockCocoaAppLayer(const std::string& name = "MockCocoaAppLayer"): I_Layer(name) {}

	void OnAttach() override {
		_attached = true;
		_attachCount++;
	}

	void OnDetach() override {
		_attached = false;
		_detachCount++;
	}

	void OnEvent(I_Event& event) override {
		_eventCount++;
		if (_shouldHandleEvent) {
			event.Consume();
		}
	}

	void OnUpdate() override {
		_updateCount++;
	}

	// Test helper methods
	[[nodiscard]] bool IsAttached() const { return _attached; }
	[[nodiscard]] int GetAttachCount() const { return _attachCount; }
	[[nodiscard]] int GetDetachCount() const { return _detachCount; }
	[[nodiscard]] int GetUpdateCount() const { return _updateCount; }
	[[nodiscard]] int GetEventCount() const { return _eventCount; }
	void SetShouldHandleEvent(const bool value) { _shouldHandleEvent = value; }

private:
	bool _attached = false;
	bool _shouldHandleEvent = false;
	mutable int _attachCount = 0;
	mutable int _detachCount = 0;
	mutable int _updateCount = 0;
	int _eventCount = 0;
};

/**
 * @brief Test fixture for CocoaApplication tests (Cocoa + Metal)
 */
class CocoaApplicationTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}

	void TearDown() override {
		Log::Shutdown();
	}

	static std::unique_ptr<CocoaApplication> CreateApplication() {
		return std::make_unique<CocoaApplication>();
	}
};

const WindowProps defaultCocoaProps{"CocoaAppTest", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};

/**
 * @brief Test CocoaApplication construction with default parameters
 */
TEST_F(CocoaApplicationTest, ConstructionDefault) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);

	ASSERT_NE(app, nullptr);
	EXPECT_EQ(&I_Application::StGet(), app.get());
	EXPECT_NE(&app->GetWindow(), nullptr);

	EXPECT_FALSE(app->HasLayers());
	EXPECT_GE(app->LayersSize(), 0);
}

/**
 * @brief Test CocoaApplication construction with WindowProps
 */
TEST_F(CocoaApplicationTest, ConstructionWithWindowProps) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);

	ASSERT_NE(app, nullptr);
	EXPECT_EQ(&I_Application::StGet(), app.get());
	EXPECT_NE(&app->GetWindow(), nullptr);

	// Verify window properties
	const auto& window = app->GetWindow();
	EXPECT_EQ(window.GetWindowWidth(), 800);
	EXPECT_EQ(window.GetWindowHeight(), 600 + NAVBAR_HEIGHT);
}

/**
 * @brief Test CocoaApplication construction with individual parameters
 */
TEST_F(CocoaApplicationTest, ConstructionWithIndividualParams) {
	const WindowProps props{"MyCocoaApp", 1024, 768, false, GraphicsApi::Metal, WindowApi::Cocoa};
	const auto app = CreateApplication();
	app->Init(props);

	ASSERT_NE(app, nullptr);
	EXPECT_EQ(&I_Application::StGet(), app.get());
	EXPECT_NE(&app->GetWindow(), nullptr);

	// Verify window properties
	const auto& window = app->GetWindow();
	EXPECT_EQ(window.GetWindowWidth(), 1024);
	EXPECT_EQ(window.GetWindowHeight(), 768 + NAVBAR_HEIGHT);
}

/**
 * @brief Test singleton pattern - only one CocoaApplication instance allowed
 */
TEST_F(CocoaApplicationTest, SingletonPattern) {
	const auto app = CreateApplication();
	EXPECT_EQ(&I_Application::StGet(), app.get());
}

/**
 * @brief Test I_Application::StGet() returns the correct CocoaApplication instance
 */
TEST_F(CocoaApplicationTest, GetInstance) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);

	I_Application& instance = I_Application::StGet();

	EXPECT_EQ(&instance, app.get());
	EXPECT_EQ(&instance.GetWindow(), &app->GetWindow());
}

/**
 * @brief Test GetWindow returns a valid window with correct dimensions
 */
TEST_F(CocoaApplicationTest, GetWindow) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);
	const auto& window = app->GetWindow();

	EXPECT_GT(window.GetWindowWidth(), 0);
	EXPECT_GT(window.GetWindowHeight(), 0);
}

/**
 * @brief Test GetRenderContext returns a valid Metal rendering context
 */
TEST_F(CocoaApplicationTest, GetRenderContext) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);

	EXPECT_NO_THROW({ [[maybe_unused]] auto& ctx = app->GetRenderContext(); });
}

/**
 * @brief Test PushLayer adds layer and calls OnAttach
 */
TEST_F(CocoaApplicationTest, PushLayer) {
	const auto app = CreateApplication();
	auto* layer = new MockCocoaAppLayer{"TestLayer"};

	EXPECT_FALSE(layer->IsAttached());
	EXPECT_FALSE(app->HasLayers());
	EXPECT_EQ(app->LayersSize(), 0);

	app->PushLayer(layer);

	EXPECT_TRUE(layer->IsAttached());
	EXPECT_EQ(layer->GetAttachCount(), 1);
	EXPECT_TRUE(app->HasLayers());
	EXPECT_EQ(app->LayersSize(), 1);
}

/**
 * @brief Test PushOverlay adds overlay and calls OnAttach
 */
TEST_F(CocoaApplicationTest, PushOverlay) {
	const auto app = CreateApplication();
	auto* overlay = new MockCocoaAppLayer("TestOverlay");

	EXPECT_FALSE(overlay->IsAttached());

	app->PushOverlay(overlay);

	EXPECT_TRUE(overlay->IsAttached());
	EXPECT_EQ(overlay->GetAttachCount(), 1);
	EXPECT_TRUE(app->HasLayers());
	EXPECT_EQ(app->LayersSize(), 1);
}

/**
 * @brief Test PopLayer removes layer and calls OnDetach
 */
TEST_F(CocoaApplicationTest, PopLayer) {
	const auto app = CreateApplication();
	auto* layer = new MockCocoaAppLayer{"TestLayer"};

	app->PushLayer(layer);
	EXPECT_TRUE(layer->IsAttached());
	EXPECT_TRUE(app->HasLayers());

	app->PopLayer(layer);

	EXPECT_FALSE(layer->IsAttached());
	EXPECT_EQ(layer->GetDetachCount(), 1);
	EXPECT_FALSE(app->HasLayers());
	EXPECT_EQ(app->LayersSize(), 0);

	// PopLayer doesn't delete, so we must do it manually
	delete layer;
}

/**
 * @brief Test PopOverlay removes overlay and calls OnDetach
 */
TEST_F(CocoaApplicationTest, PopOverlay) {
	const auto app = CreateApplication();
	auto* overlay = new MockCocoaAppLayer("TestOverlay");

	app->PushOverlay(overlay);
	EXPECT_TRUE(overlay->IsAttached());
	EXPECT_TRUE(app->HasLayers());

	app->PopOverlay(overlay);

	EXPECT_FALSE(overlay->IsAttached());
	EXPECT_EQ(overlay->GetDetachCount(), 1);
	EXPECT_FALSE(app->HasLayers());
	EXPECT_EQ(app->LayersSize(), 0);

	// PopOverlay doesn't delete, so we must do it manually
	delete overlay;
}

/**
 * @brief Test HasLayers returns correct state
 */
TEST_F(CocoaApplicationTest, HasLayers) {
	const auto app = CreateApplication();
	EXPECT_FALSE(app->HasLayers());

	auto* layer1 = new MockCocoaAppLayer("Layer1");
	app->PushLayer(layer1);
	EXPECT_TRUE(app->HasLayers());

	auto* layer2 = new MockCocoaAppLayer("Layer2");
	app->PushLayer(layer2);
	EXPECT_TRUE(app->HasLayers());

	app->PopLayer(layer1);
	EXPECT_TRUE(app->HasLayers());

	app->PopLayer(layer2);
	EXPECT_FALSE(app->HasLayers());

	delete layer1;
	delete layer2;
}

/**
 * @brief Test multiple layers and overlays
 */
TEST_F(CocoaApplicationTest, MultipleLayersAndOverlays) {
	const auto app = CreateApplication();

	const auto layer1 = new MockCocoaAppLayer("Layer1");
	const auto layer2 = new MockCocoaAppLayer("Layer2");
	const auto overlay1 = new MockCocoaAppLayer("Overlay1");
	const auto overlay2 = new MockCocoaAppLayer("Overlay2");

	app->PushLayer(layer1);
	app->PushLayer(layer2);
	app->PushOverlay(overlay1);
	app->PushOverlay(overlay2);

	EXPECT_TRUE(layer1->IsAttached());
	EXPECT_TRUE(layer2->IsAttached());
	EXPECT_TRUE(overlay1->IsAttached());
	EXPECT_TRUE(overlay2->IsAttached());
	EXPECT_TRUE(app->HasLayers());

	// Clean up in reverse order
	app->PopOverlay(overlay2);
	app->PopOverlay(overlay1);
	app->PopLayer(layer2);
	app->PopLayer(layer1);

	EXPECT_FALSE(app->HasLayers());

	delete layer1;
	delete layer2;
	delete overlay1;
	delete overlay2;
}

/**
 * @brief Test Tick calls OnUpdate on all layers (requires ImGui Metal layer)
 * @details CocoaApplication::Tick always dereferences _imguiLayer,
 *          so InitImGuiLayer must be called before Tick.
 */
TEST_F(CocoaApplicationTest, UpdateCallsLayerOnUpdate) {
	const auto app = CreateApplication();
	app->Init(defaultCocoaProps);
	app->InitImGuiLayer(GraphicsApi::Metal);

	const auto layer1 = new MockCocoaAppLayer("Layer1");
	const auto layer2 = new MockCocoaAppLayer("Layer2");
	const auto overlay = new MockCocoaAppLayer("Overlay");

	app->PushLayer(layer1);
	app->PushLayer(layer2);
	app->PushOverlay(overlay);

	EXPECT_EQ(layer1->GetUpdateCount(), 0);
	EXPECT_EQ(layer2->GetUpdateCount(), 0);
	EXPECT_EQ(overlay->GetUpdateCount(), 0);

	app->Tick(0.016f);

	EXPECT_EQ(layer1->GetUpdateCount(), 1);
	EXPECT_EQ(layer2->GetUpdateCount(), 1);
	EXPECT_EQ(overlay->GetUpdateCount(), 1);

	app->Tick(0.016f);
	app->Tick(0.016f);

	EXPECT_EQ(layer1->GetUpdateCount(), 3);
	EXPECT_EQ(layer2->GetUpdateCount(), 3);
	EXPECT_EQ(overlay->GetUpdateCount(), 3);

	// Note: Don't delete layers manually - they're still in the stack
	// and will be deleted by LayerStack destructor when app is destroyed
}

/**
 * @brief Test OnEvent propagates to layers in reverse order
 */
TEST_F(CocoaApplicationTest, OnEventPropagation) {
	const auto app = CreateApplication();

	const auto layer1 = new MockCocoaAppLayer("Layer1");
	const auto layer2 = new MockCocoaAppLayer("Layer2");
	const auto overlay = new MockCocoaAppLayer("Overlay");

	app->PushLayer(layer1);
	app->PushLayer(layer2);
	app->PushOverlay(overlay);

	// Create a mock event (use WindowResizeEvent for testing)
	WindowResizeEvent event{800, 600};

	EXPECT_EQ(layer1->GetEventCount(), 0);
	EXPECT_EQ(layer2->GetEventCount(), 0);
	EXPECT_EQ(overlay->GetEventCount(), 0);

	app->OnEvent(event);

	// Events should propagate in reverse order: overlay, layer2, layer1
	// All should receive the event since none handle it
	EXPECT_EQ(overlay->GetEventCount(), 1);
	EXPECT_EQ(layer2->GetEventCount(), 1);
	EXPECT_EQ(layer1->GetEventCount(), 1);

	// Note: Don't delete layers manually - they're managed by LayerStack
}

/**
 * @brief Test OnEvent stops propagation when handled by a layer
 */
TEST_F(CocoaApplicationTest, OnEventStopsPropagation) {
	const auto app = CreateApplication();

	const auto layer1 = new MockCocoaAppLayer("Layer1");
	const auto layer2 = new MockCocoaAppLayer("Layer2");
	const auto overlay = new MockCocoaAppLayer("Overlay");

	app->PushLayer(layer1);
	app->PushLayer(layer2);
	app->PushOverlay(overlay);

	// Make layer2 handle the event
	layer2->SetShouldHandleEvent(true);

	WindowResizeEvent event{800, 600};

	app->OnEvent(event);

	// Overlay and layer2 should receive event, but layer1 should not
	// (because layer2 handled it)
	EXPECT_EQ(overlay->GetEventCount(), 1);
	EXPECT_EQ(layer2->GetEventCount(), 1);
	EXPECT_EQ(layer1->GetEventCount(), 0);

	EXPECT_TRUE(event.IsHandled());

	// Note: Don't delete layers manually - they're managed by LayerStack
}

/**
 * @brief Test WindowClose event causes Quit
 */
TEST_F(CocoaApplicationTest, OnWindowCloseEvent) {
	const auto app = CreateApplication();

	WindowCloseEvent closeEvent;
	EXPECT_FALSE(closeEvent.IsHandled());

	// Simulate window close event - Quit() will call StopDisplayLink() and stop NSApplication
	app->OnEvent(closeEvent);

	// Event should be handled by the application
	EXPECT_TRUE(closeEvent.IsHandled());
}

/**
 * @brief Mock Layer that sets an external flag when detached
 */
class MockCocoaAppLayerWithFlag: public I_Layer {
public:
	explicit MockCocoaAppLayerWithFlag(const std::string& name, bool* detachedFlag)
		: I_Layer(name), _detachedFlag(detachedFlag) {}

	void OnAttach() override {}
	void OnDetach() override {
		if (_detachedFlag) {
			*_detachedFlag = true;
		}
	}
	void OnEvent([[maybe_unused]] I_Event& event) override {}
	void OnUpdate() override {}

private:
	bool* _detachedFlag;
};

/**
 * @brief Test destructor cleanup calls OnDetach on all layers
 */
TEST_F(CocoaApplicationTest, DestructorCleanup) {
	bool layer1Detached = false;
	bool layer2Detached = false;

	{
		const auto app = CreateApplication();

		// Create layers that set flags when detached
		auto* layer1 = new MockCocoaAppLayerWithFlag("Layer1", &layer1Detached);
		auto* layer2 = new MockCocoaAppLayerWithFlag("Layer2", &layer2Detached);

		app->PushLayer(layer1);
		app->PushLayer(layer2);

		EXPECT_FALSE(layer1Detached);
		EXPECT_FALSE(layer2Detached);
	}
	// Application destroyed here - should call OnDetach on all layers

	// Verify OnDetach was called on both layers before they were deleted
	EXPECT_TRUE(layer1Detached);
	EXPECT_TRUE(layer2Detached);
}
