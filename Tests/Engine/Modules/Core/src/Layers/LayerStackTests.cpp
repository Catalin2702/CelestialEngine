//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerStackTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-03-11
//

#include <Events/I_Event.hpp>
#include <Layers/I_Layer.hpp>
#include <Layers/LayerStack.hpp>

#include <gtest/gtest.h>

using namespace CE::Layers;


/**
 * @brief Mock Layer for testing LayerStack
 */
class MockStackLayer: public I_Layer {
public:
	explicit MockStackLayer(const std::string& name = "MockStackLayer"): I_Layer(name) {}

	void OnAttach() override {
		_attached = true;
		_attachCount++;
	}

	void OnDetach() override {
		_attached = false;
		_detachCount++;
	}

	void OnEvent([[maybe_unused]] CE::Events::I_Event& event) override {
		_eventCount++;
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

private:
	bool _attached = false;
	mutable int _attachCount = 0;
	mutable int _detachCount = 0;
	mutable int _updateCount = 0;
	mutable int _eventCount = 0;
};

/**
 * @brief Test fixture for LayerStack tests
 */
class LayerStackTest: public ::testing::Test {
protected:
	void SetUp() override {
		// Setup code if needed
	}
	void TearDown() override {
		// Cleanup code if needed
	}
};

/**
 * @brief Test LayerStack construction
 */
TEST_F(LayerStackTest, Construction) {
	LayerStack stack;

	EXPECT_EQ(stack.begin(), stack.end());
}

/**
 * @brief Test pushing a single layer
 */
TEST_F(LayerStackTest, PushLayer) {
	LayerStack stack;
	auto* layer = new MockStackLayer("Layer1");

	EXPECT_FALSE(layer->IsAttached());
	EXPECT_EQ(layer->GetAttachCount(), 0);

	stack.PushLayer(layer);

	EXPECT_TRUE(layer->IsAttached());
	EXPECT_EQ(layer->GetAttachCount(), 1);
	EXPECT_NE(stack.begin(), stack.end());
}

/**
 * @brief Test pushing multiple layers
 */
TEST_F(LayerStackTest, PushMultipleLayers) {
	LayerStack stack;
	auto* layer1 = new MockStackLayer("Layer1");
	auto* layer2 = new MockStackLayer("Layer2");
	auto* layer3 = new MockStackLayer("Layer3");

	stack.PushLayer(layer1);
	stack.PushLayer(layer2);
	stack.PushLayer(layer3);

	EXPECT_TRUE(layer1->IsAttached());
	EXPECT_TRUE(layer2->IsAttached());
	EXPECT_TRUE(layer3->IsAttached());

	// Verify order: layers should be in the order they were pushed
	auto it = stack.begin();
	EXPECT_EQ(*it, layer1);
	++it;
	EXPECT_EQ(*it, layer2);
	++it;
	EXPECT_EQ(*it, layer3);
}

/**
 * @brief Test pushing a single overlay
 */
TEST_F(LayerStackTest, PushOverlay) {
	LayerStack stack;
	auto* overlay = new MockStackLayer("Overlay1");

	EXPECT_FALSE(overlay->IsAttached());

	stack.PushOverlay(overlay);

	EXPECT_TRUE(overlay->IsAttached());
	EXPECT_EQ(overlay->GetAttachCount(), 1);
}

/**
 * @brief Test pushing layers and overlays
 */
TEST_F(LayerStackTest, PushLayersAndOverlays) {
	LayerStack stack;
	auto* layer1 = new MockStackLayer("Layer1");
	auto* layer2 = new MockStackLayer("Layer2");
	auto* overlay1 = new MockStackLayer("Overlay1");
	auto* overlay2 = new MockStackLayer("Overlay2");

	stack.PushLayer(layer1);
	stack.PushOverlay(overlay1);
	stack.PushLayer(layer2);
	stack.PushOverlay(overlay2);

	// Verify order: layers first, then overlays
	auto it = stack.begin();
	EXPECT_EQ(*it, layer1);
	++it;
	EXPECT_EQ(*it, layer2);
	++it;
	EXPECT_EQ(*it, overlay1);
	++it;
	EXPECT_EQ(*it, overlay2);
}

/**
 * @brief Test popping a layer
 */
TEST_F(LayerStackTest, PopLayer) {
	LayerStack stack;
	auto* layer = new MockStackLayer("Layer1");

	stack.PushLayer(layer);
	EXPECT_TRUE(layer->IsAttached());
	EXPECT_EQ(layer->GetDetachCount(), 0);

	stack.PopLayer(layer);

	EXPECT_FALSE(layer->IsAttached());
	EXPECT_EQ(layer->GetDetachCount(), 1);

	delete layer;
}

/**
 * @brief Test popping an overlay
 */
TEST_F(LayerStackTest, PopOverlay) {
	LayerStack stack;
	auto* overlay = new MockStackLayer("Overlay1");

	stack.PushOverlay(overlay);
	EXPECT_TRUE(overlay->IsAttached());

	stack.PopOverlay(overlay);

	EXPECT_FALSE(overlay->IsAttached());
	EXPECT_EQ(overlay->GetDetachCount(), 1);

	delete overlay;
}

/**
 * @brief Test clearing the stack
 */
TEST_F(LayerStackTest, Clear) {
	LayerStack stack;
	auto* layer1 = new MockStackLayer("Layer1");
	auto* layer2 = new MockStackLayer("Layer2");
	auto* overlay1 = new MockStackLayer("Overlay1");

	stack.PushLayer(layer1);
	stack.PushLayer(layer2);
	stack.PushOverlay(overlay1);

	EXPECT_TRUE(layer1->IsAttached());
	EXPECT_TRUE(layer2->IsAttached());
	EXPECT_TRUE(overlay1->IsAttached());

	stack.Clear();

	EXPECT_EQ(stack.begin(), stack.end());
}

/**
 * @brief Test iterating through the stack
 */
TEST_F(LayerStackTest, Iteration) {
	LayerStack stack;
	auto* layer1 = new MockStackLayer("Layer1");
	auto* layer2 = new MockStackLayer("Layer2");
	auto* overlay1 = new MockStackLayer("Overlay1");

	stack.PushLayer(layer1);
	stack.PushLayer(layer2);
	stack.PushOverlay(overlay1);

	int count = 0;
	for (auto* layer : stack) {
		EXPECT_NE(layer, nullptr);
		count++;
	}

	EXPECT_EQ(count, 3);
}

/**
 * @brief Test layer stack lifecycle
 */
TEST_F(LayerStackTest, LayerStackLifecycle) {
	LayerStack stack;
	auto* layer1 = new MockStackLayer("Layer1");
	auto* layer2 = new MockStackLayer("Layer2");

	// Push layers
	stack.PushLayer(layer1);
	stack.PushLayer(layer2);

	EXPECT_EQ(layer1->GetAttachCount(), 1);
	EXPECT_EQ(layer2->GetAttachCount(), 1);
	EXPECT_EQ(layer1->GetDetachCount(), 0);
	EXPECT_EQ(layer2->GetDetachCount(), 0);

	// Pop one layer
	stack.PopLayer(layer1);

	EXPECT_EQ(layer1->GetDetachCount(), 1);
	EXPECT_EQ(layer2->GetDetachCount(), 0);

	delete layer1;

	// Clear should detach remaining layers
	stack.Clear();

	// Note: layer2 was deleted by Clear()
}
