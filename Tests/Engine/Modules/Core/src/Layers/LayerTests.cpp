//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/I_Layer.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>

#include <gtest/gtest.h>

using namespace CE::Layers;
using namespace CE::Events;
using namespace CE::KeyCode;


/**
 * @brief Mock Layer implementation for testing
 */
class MockLayer: public I_Layer {
public:
	explicit MockLayer(const std::string& name = "MockLayer"): I_Layer(name) {}

	void OnAttach() override {
		_attached = true;
	}

	void OnDetach() override {
		_attached = false;
	}

	void OnUpdate() const override {
		_updateCount++;
	}

	bool OnEvent([[maybe_unused]] I_Event& event) override {
		_eventCount++;
		return _shouldHandleEvent;
	}

	// Test helper methods
	[[nodiscard]] bool IsAttached() const { return _attached; }
	[[nodiscard]] int GetUpdateCount() const { return _updateCount; }
	[[nodiscard]] int GetEventCount() const { return _eventCount; }
	void SetShouldHandleEvent(const bool value) { _shouldHandleEvent = value; }

private:
	bool _attached = false;
	mutable int _updateCount = 0;
	mutable int _eventCount = 0;
	bool _shouldHandleEvent = false;
};

/**
 * @brief Test fixture for I_Layer tests
 */
class LayerTest: public ::testing::Test {};

/**
 * @brief Test Layer construction
 */
TEST_F(LayerTest, Construction) {
	const MockLayer layer{"TestLayer"};

	EXPECT_FALSE(layer.IsAttached());
	EXPECT_EQ(layer.GetUpdateCount(), 0);
	EXPECT_EQ(layer.GetEventCount(), 0);

#ifdef CE_DEBUG
	EXPECT_EQ(layer.GetDebugName(), "TestLayer");
#endif
}

/**
 * @brief Test Layer construction with default name
 */
TEST_F(LayerTest, ConstructionDefaultName) {
	const MockLayer layer;

	EXPECT_FALSE(layer.IsAttached());

#ifdef CE_DEBUG
	EXPECT_EQ(layer.GetDebugName(), "MockLayer");
#endif
}

/**
 * @brief Test Layer OnAttach
 */
TEST_F(LayerTest, OnAttach) {
	MockLayer layer{"TestLayer"};

	EXPECT_FALSE(layer.IsAttached());

	layer.OnAttach();

	EXPECT_TRUE(layer.IsAttached());
}

/**
 * @brief Test Layer OnDetach
 */
TEST_F(LayerTest, OnDetach) {
	MockLayer layer{"TestLayer"};

	layer.OnAttach();
	EXPECT_TRUE(layer.IsAttached());

	layer.OnDetach();
	EXPECT_FALSE(layer.IsAttached());
}

/**
 * @brief Test Layer OnUpdate
 */
TEST_F(LayerTest, OnUpdate) {
	const MockLayer layer{"TestLayer"};

	EXPECT_EQ(layer.GetUpdateCount(), 0);

	layer.OnUpdate();
	EXPECT_EQ(layer.GetUpdateCount(), 1);

	layer.OnUpdate();
	EXPECT_EQ(layer.GetUpdateCount(), 2);

	layer.OnUpdate();
	EXPECT_EQ(layer.GetUpdateCount(), 3);
}

/**
 * @brief Test Layer OnEvent without handling
 */
TEST_F(LayerTest, OnEventNotHandled) {
	MockLayer layer{"TestLayer"};
	layer.SetShouldHandleEvent(false);

	KeyPressedEvent event(KeyboardKeyCode::A, 0); // 'A' key

	EXPECT_EQ(layer.GetEventCount(), 0);
	bool handled = layer.OnEvent(event);

	EXPECT_FALSE(handled);
	EXPECT_EQ(layer.GetEventCount(), 1);
}

/**
 * @brief Test Layer OnEvent with handling
 */
TEST_F(LayerTest, OnEventHandled) {
	MockLayer layer{"TestLayer"};
	layer.SetShouldHandleEvent(true);

	KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_EQ(layer.GetEventCount(), 0);
	bool handled = layer.OnEvent(event);

	EXPECT_TRUE(handled);
	EXPECT_EQ(layer.GetEventCount(), 1);
}

/**
 * @brief Test Layer receiving multiple events
 */
TEST_F(LayerTest, MultipleEvents) {
	MockLayer layer{"TestLayer"};
	layer.SetShouldHandleEvent(false);

	KeyPressedEvent keyEvent(KeyboardKeyCode::A, 0);
	MouseMovedEvent mouseEvent(100.0f, 200.0f);
	WindowResizeEvent resizeEvent(800, 600);

	EXPECT_EQ(layer.GetEventCount(), 0);

	layer.OnEvent(keyEvent);
	EXPECT_EQ(layer.GetEventCount(), 1);

	layer.OnEvent(mouseEvent);
	EXPECT_EQ(layer.GetEventCount(), 2);

	layer.OnEvent(resizeEvent);
	EXPECT_EQ(layer.GetEventCount(), 3);
}

/**
 * @brief Test Layer lifecycle
 */
TEST_F(LayerTest, LayerLifecycle) {
	MockLayer layer{"TestLayer"};

	// Initial state
	EXPECT_FALSE(layer.IsAttached());
	EXPECT_EQ(layer.GetUpdateCount(), 0);
	EXPECT_EQ(layer.GetEventCount(), 0);

	// Attach
	layer.OnAttach();
	EXPECT_TRUE(layer.IsAttached());

	// Update
	layer.OnUpdate();
	layer.OnUpdate();
	EXPECT_EQ(layer.GetUpdateCount(), 2);

	// Event
	KeyPressedEvent event(KeyboardKeyCode::A, 0);
	layer.OnEvent(event);
	EXPECT_EQ(layer.GetEventCount(), 1);

	// Detach
	layer.OnDetach();
	EXPECT_FALSE(layer.IsAttached());
}

#ifdef CE_DEBUG
/**
 * @brief Test Layer debug name
 */
TEST_F(LayerTest, DebugName) {
	const MockLayer layer1("Layer1");
	const MockLayer layer2("Layer2");
	const MockLayer layer3;

	EXPECT_EQ(layer1.GetDebugName(), "Layer1");
	EXPECT_EQ(layer2.GetDebugName(), "Layer2");
	EXPECT_EQ(layer3.GetDebugName(), "MockLayer");
}
#endif
