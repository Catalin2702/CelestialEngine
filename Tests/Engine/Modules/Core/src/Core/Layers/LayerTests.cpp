//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#include <Core/Hub/Events/I_EventHubDispatcher.hpp>
#include <Core/Layers/I_Layer.hpp>

#include <gtest/gtest.h>

#include <string>

using CE::Core::I_EventHubDispatcher;
using CE::Core::I_Layer;

namespace {

/**
 * @brief Minimal hub standing in for a real one, so a layer can be handed a dispatcher without an application
 * @details The subscription hooks only need something to hand over and store; the channels stay empty because no
 *			test here dispatches through them. Deriving from the templated base fills in GetWindowApi.
 */
class StubEventHubDispatcher final: public CE::Core::I_EventHubDispatcherBase<CE::Types::WindowApi::GLFW> {
public:
	[[nodiscard]] CE::Core::ApplicationEventHub& GetApplicationEventHub() override { return _application; }
	[[nodiscard]] const CE::Core::ApplicationEventHub& GetApplicationEventHub() const override { return _application; }

	[[nodiscard]] CE::Core::KeyboardEventHub& GetKeyboardEventHub() override { return _keyboard; }
	[[nodiscard]] const CE::Core::KeyboardEventHub& GetKeyboardEventHub() const override { return _keyboard; }

	[[nodiscard]] CE::Core::MouseEventHub& GetMouseEventHub() override { return _mouse; }
	[[nodiscard]] const CE::Core::MouseEventHub& GetMouseEventHub() const override { return _mouse; }

	[[nodiscard]] CE::Core::WindowEventHub& GetWindowEventHub() override { return _window; }
	[[nodiscard]] const CE::Core::WindowEventHub& GetWindowEventHub() const override { return _window; }

	[[nodiscard]] CE::Core::RenderEventHub& GetRenderEventHub() override { return _render; }
	[[nodiscard]] const CE::Core::RenderEventHub& GetRenderEventHub() const override { return _render; }

private:
	CE::Core::ApplicationEventHub _application;
	CE::Core::KeyboardEventHub _keyboard;
	CE::Core::MouseEventHub _mouse;
	CE::Core::WindowEventHub _window;
	CE::Core::RenderEventHub _render;
};

/**
 * @brief Mock layer recording every lifecycle call it receives
 * @details Layers no longer receive events through an OnEvent method: they subscribe to the event hub instead, so
 *			the layer contract is the lifecycle (attach/detach), the per-frame calls (update/render) and the hub
 *			subscription hooks. The begin/end frame bracket is not part of it - only an ImGui layer has one, because
 *			only an immediate-mode UI needs its widgets declared between a NewFrame and a Render.
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

	void SubscribeToEventHub(I_EventHubDispatcher& eventHubDispatcher) override {
		subscribed = true;
		lastEventHub = &eventHubDispatcher;
	}

	void UnsubscribeFromEventHub() override { subscribed = false; }

public:
	bool attached = false;
	bool subscribed = false;
	int attachCount = 0;
	int detachCount = 0;
	int updateCount = 0;
	mutable int renderCount = 0;
	const I_EventHubDispatcher* lastEventHub = nullptr;
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

// ============================================================================
// Event Hub Hook Tests
// ============================================================================

/**
 * @brief Test that the hub subscription hooks are honoured by an overriding layer
 */
TEST_F(LayerTest, EventHubHooks_AreCalledOnOverridingLayer) {
	MockLayer layer;
	StubEventHubDispatcher eventHub;

	EXPECT_FALSE(layer.subscribed);

	layer.SubscribeToEventHub(eventHub);
	EXPECT_TRUE(layer.subscribed);

	layer.UnsubscribeFromEventHub();
	EXPECT_FALSE(layer.subscribed);
}

/**
 * @brief Test that the layer is handed the very hub it was asked to subscribe to
 * @details The point of the parameter: a layer subscribes to the dispatcher it is given, not to one it goes looking
 *			for through the application singleton.
 */
TEST_F(LayerTest, SubscribeToEventHub_ReceivesTheGivenDispatcher) {
	MockLayer layer;
	StubEventHubDispatcher eventHub;

	layer.SubscribeToEventHub(eventHub);

	EXPECT_EQ(layer.lastEventHub, &eventHub);
}

/**
 * @brief Test that the hub subscription hooks are optional
 * @details A layer that ignores the event hub must not be forced to implement the hooks.
 */
TEST_F(LayerTest, EventHubHooks_AreOptional) {
	MinimalLayer layer;
	StubEventHubDispatcher eventHub;

	EXPECT_NO_THROW(layer.SubscribeToEventHub(eventHub));
	EXPECT_NO_THROW(layer.UnsubscribeFromEventHub());
}

/**
 * @brief Test that a layer is usable through a base-class pointer
 */
TEST_F(LayerTest, LayerLifecycle_ThroughBasePointer) {
	MockLayer layer{"TestLayer"};
	I_Layer* base = &layer;

	base->OnAttach();
	base->OnUpdate();
	base->OnRender();
	base->OnDetach();

	EXPECT_EQ(layer.attachCount, 1);
	EXPECT_EQ(layer.updateCount, 1);
	EXPECT_EQ(layer.renderCount, 1);
	EXPECT_EQ(layer.detachCount, 1);
	EXPECT_FALSE(layer.attached);
}
