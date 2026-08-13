//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Core/Layers/I_Layer.hpp>

#include <gtest/gtest.h>

#include <string>

using CE::Core::I_Layer;

namespace {

/**
 * @brief Mock layer recording every lifecycle call it receives
 * @details Layers no longer receive events through an OnEvent method: they subscribe to the event hub instead, so
 *			the layer contract is the lifecycle (attach/detach), the per-frame calls (update/render, begin/end) and
 *			the hub subscription hooks.
 */
class MockLayer: public I_Layer {
public:
	explicit MockLayer(const std::string& name = "MockLayer"): I_Layer(name) {}

	void OnAttach() override {
		attached = true;
		++attachCount;
	}

	void OnDetach() override {
		attached = false;
		++detachCount;
	}

	void OnUpdate() override { ++updateCount; }

	void OnRender() const override { ++renderCount; }

	void Begin(const float deltaTime) override {
		++beginCount;
		lastDeltaTime = deltaTime;
	}

	void End() override { ++endCount; }

	void SubscribeToEventHub() override { subscribed = true; }

	void UnsubscribeFromEventHub() override { subscribed = false; }

public:
	bool attached = false;
	bool subscribed = false;
	int attachCount = 0;
	int detachCount = 0;
	int updateCount = 0;
	mutable int renderCount = 0;
	int beginCount = 0;
	int endCount = 0;
	float lastDeltaTime = 0.0f;
};

/**
 * @brief Layer that does not override the optional hub hooks
 */
class MinimalLayer final: public I_Layer {
public:
	MinimalLayer(): I_Layer("MinimalLayer") {}

	void OnAttach() override {}
	void OnDetach() override {}
	void OnUpdate() override {}
	void OnRender() const override {}
	void Begin(float) override {}
	void End() override {}
};

/**
 * @brief Test fixture for I_Layer tests
 */
class LayerTest: public ::testing::Test {};

}

// ============================================================================
// Naming Tests
// ============================================================================

/**
 * @brief Test that the layer keeps the name it was constructed with
 */
TEST_F(LayerTest, GetName_ReturnsConstructionName) {
	const MockLayer layer{"TestLayer"};

	EXPECT_EQ(layer.GetName(), "TestLayer");
}

/**
 * @brief Test that a layer built without a name falls back to the default
 */
TEST_F(LayerTest, GetName_DefaultsToLayerName) {
	const MockLayer layer;

	EXPECT_EQ(layer.GetName(), "MockLayer");
}

// ============================================================================
// Lifecycle Tests
// ============================================================================

/**
 * @brief Test that a freshly constructed layer is not attached
 */
TEST_F(LayerTest, Construction_LeavesLayerDetached) {
	const MockLayer layer{"TestLayer"};

	EXPECT_FALSE(layer.attached);
	EXPECT_EQ(layer.attachCount, 0);
	EXPECT_EQ(layer.updateCount, 0);
}

/**
 * @brief Test that OnAttach marks the layer attached
 */
TEST_F(LayerTest, OnAttach_MarksLayerAttached) {
	MockLayer layer{"TestLayer"};

	layer.OnAttach();

	EXPECT_TRUE(layer.attached);
	EXPECT_EQ(layer.attachCount, 1);
}

/**
 * @brief Test that OnDetach marks the layer detached
 */
TEST_F(LayerTest, OnDetach_MarksLayerDetached) {
	MockLayer layer{"TestLayer"};

	layer.OnAttach();
	layer.OnDetach();

	EXPECT_FALSE(layer.attached);
	EXPECT_EQ(layer.detachCount, 1);
}

// ============================================================================
// Frame Tests
// ============================================================================

/**
 * @brief Test that OnUpdate is counted on every call
 */
TEST_F(LayerTest, OnUpdate_IsCalledEveryTime) {
	MockLayer layer;

	layer.OnUpdate();
	layer.OnUpdate();
	layer.OnUpdate();

	EXPECT_EQ(layer.updateCount, 3);
}

/**
 * @brief Test that OnRender is callable on a const layer
 * @details The render pass runs over const layers, so OnRender must not require a mutable layer.
 */
TEST_F(LayerTest, OnRender_IsCallableOnConstLayer) {
	const MockLayer layer;

	layer.OnRender();

	EXPECT_EQ(layer.renderCount, 1);
}

/**
 * @brief Test that Begin receives the frame delta time and End closes the frame
 */
TEST_F(LayerTest, BeginEnd_FrameBracketReceivesDeltaTime) {
	MockLayer layer;

	layer.Begin(0.016f);
	layer.End();

	EXPECT_EQ(layer.beginCount, 1);
	EXPECT_EQ(layer.endCount, 1);
	EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.016f);
}

// ============================================================================
// Event Hub Hook Tests
// ============================================================================

/**
 * @brief Test that the hub subscription hooks are honoured by an overriding layer
 */
TEST_F(LayerTest, EventHubHooks_AreCalledOnOverridingLayer) {
	MockLayer layer;

	EXPECT_FALSE(layer.subscribed);

	layer.SubscribeToEventHub();
	EXPECT_TRUE(layer.subscribed);

	layer.UnsubscribeFromEventHub();
	EXPECT_FALSE(layer.subscribed);
}

/**
 * @brief Test that the hub subscription hooks are optional
 * @details A layer that ignores the event hub must not be forced to implement the hooks.
 */
TEST_F(LayerTest, EventHubHooks_AreOptional) {
	MinimalLayer layer;

	EXPECT_NO_THROW(layer.SubscribeToEventHub());
	EXPECT_NO_THROW(layer.UnsubscribeFromEventHub());
}

/**
 * @brief Test that a layer is usable through a base-class pointer
 */
TEST_F(LayerTest, LayerLifecycle_ThroughBasePointer) {
	MockLayer layer{"TestLayer"};
	I_Layer* base = &layer;

	base->OnAttach();
	base->Begin(0.033f);
	base->OnUpdate();
	base->OnRender();
	base->End();
	base->OnDetach();

	EXPECT_EQ(layer.attachCount, 1);
	EXPECT_EQ(layer.beginCount, 1);
	EXPECT_EQ(layer.updateCount, 1);
	EXPECT_EQ(layer.renderCount, 1);
	EXPECT_EQ(layer.endCount, 1);
	EXPECT_EQ(layer.detachCount, 1);
	EXPECT_FALSE(layer.attached);
}
