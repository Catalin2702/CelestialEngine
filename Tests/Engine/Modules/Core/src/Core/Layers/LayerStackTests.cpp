//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerStackTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include <Core/Layers/I_Layer.hpp>
#include <Core/Layers/LayerStack.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

using CE::Core::I_Layer;
using CE::Core::LayerStack;

namespace {

/**
 * @brief Mock layer for LayerStack tests
 * @details The stack owns its layers and deletes them on Clear/destruction, so the detach flag is reported through
 *			an external counter that outlives the layer.
 */
class MockStackLayer: public I_Layer {
public:
	explicit MockStackLayer(const std::string& name = "MockStackLayer", int* detachCounter = nullptr):
		I_Layer(name), _detachCounter(detachCounter) {}

	void OnAttach() override {
		attached = true;
		++attachCount;
	}

	void OnDetach() override {
		attached = false;
		++detachCount;
		if (_detachCounter)
			++(*_detachCounter);
	}

	void OnUpdate() override { ++updateCount; }
	void OnRender() const override {}
	void Begin(float) override {}
	void End() override {}

public:
	bool attached = false;
	int attachCount = 0;
	int detachCount = 0;
	int updateCount = 0;

private:
	int* _detachCounter;							///< Optional external counter, survives the layer deletion
};

/**
 * @brief Test fixture for LayerStack tests
 */
class LayerStackTest: public ::testing::Test {};

}

// ============================================================================
// Push Tests
// ============================================================================

/**
 * @brief Test that a fresh stack is empty
 */
TEST_F(LayerStackTest, DefaultConstructed_IsEmpty) {
	const LayerStack stack;

	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(stack.Size(), 0);
}

/**
 * @brief Test that PushLayer stores the layer and attaches it
 */
TEST_F(LayerStackTest, PushLayer_StoresAndAttachesLayer) {
	LayerStack stack;
	auto* layer = new MockStackLayer{"Layer"};

	stack.PushLayer(layer);

	EXPECT_FALSE(stack.Empty());
	EXPECT_EQ(stack.Size(), 1);
	EXPECT_TRUE(layer->attached);
	EXPECT_EQ(layer->attachCount, 1);
}

/**
 * @brief Test that PushOverlay stores the overlay and attaches it
 */
TEST_F(LayerStackTest, PushOverlay_StoresAndAttachesOverlay) {
	LayerStack stack;
	auto* overlay = new MockStackLayer{"Overlay"};

	stack.PushOverlay(overlay);

	EXPECT_EQ(stack.Size(), 1);
	EXPECT_TRUE(overlay->attached);
}

/**
 * @brief Test that layers are inserted before overlays
 * @details Overlays (the ImGui layer, for instance) must always render last, so a layer pushed after an overlay
 *			still lands before it in the iteration order.
 */
TEST_F(LayerStackTest, PushLayer_InsertsBeforeOverlays) {
	LayerStack stack;
	auto* overlay = new MockStackLayer{"Overlay"};
	auto* firstLayer = new MockStackLayer{"First"};
	auto* secondLayer = new MockStackLayer{"Second"};

	stack.PushOverlay(overlay);
	stack.PushLayer(firstLayer);
	stack.PushLayer(secondLayer);

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"First", "Second", "Overlay"}));
}

/**
 * @brief Test that pushing several layers keeps them all
 */
TEST_F(LayerStackTest, PushLayer_MultipleLayers_KeepsAllOfThem) {
	LayerStack stack;

	stack.PushLayer(new MockStackLayer{"Layer1"});
	stack.PushLayer(new MockStackLayer{"Layer2"});
	stack.PushOverlay(new MockStackLayer{"Overlay1"});
	stack.PushOverlay(new MockStackLayer{"Overlay2"});

	EXPECT_EQ(stack.Size(), 4);
}

// ============================================================================
// Pop Tests
// ============================================================================

/**
 * @brief Test that PopLayer removes the layer and detaches it
 * @details The stack owns its layers, so the popped layer is destroyed with it: the detach is observed through the
 *			external counter, which outlives the layer, never through the (already dangling) layer pointer.
 */
TEST_F(LayerStackTest, PopLayer_RemovesAndDetachesLayer) {
	LayerStack stack;
	int detachCount = 0;
	auto* layer = new MockStackLayer{"Layer", &detachCount};

	stack.PushLayer(layer);
	stack.PopLayer(layer);

	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(detachCount, 1);
}

/**
 * @brief Test that PopOverlay removes the overlay and detaches it
 */
TEST_F(LayerStackTest, PopOverlay_RemovesAndDetachesOverlay) {
	LayerStack stack;
	int detachCount = 0;
	auto* overlay = new MockStackLayer{"Overlay", &detachCount};

	stack.PushOverlay(overlay);
	stack.PopOverlay(overlay);

	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(detachCount, 1);
}

/**
 * @brief Test that popping an unknown layer leaves the stack untouched
 */
TEST_F(LayerStackTest, PopLayer_UnknownLayer_LeavesStackUntouched) {
	LayerStack stack;
	auto* known = new MockStackLayer{"Known"};
	const auto unknown = std::make_unique<MockStackLayer>("Unknown");

	stack.PushLayer(known);
	stack.PopLayer(unknown.get());

	EXPECT_EQ(stack.Size(), 1);
	EXPECT_TRUE(known->attached);
	EXPECT_EQ(unknown->detachCount, 0);
}

/**
 * @brief Test that popping a layer keeps the insertion point consistent
 * @details After removing a layer, a newly pushed one must still land before the overlays.
 */
TEST_F(LayerStackTest, PopLayer_KeepsInsertionPointConsistent) {
	LayerStack stack;
	auto* overlay = new MockStackLayer{"Overlay"};
	auto* firstLayer = new MockStackLayer{"First"};

	stack.PushOverlay(overlay);
	stack.PushLayer(firstLayer);
	stack.PopLayer(firstLayer);

	stack.PushLayer(new MockStackLayer{"Second"});

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"Second", "Overlay"}));
}

// ============================================================================
// Iteration Tests
// ============================================================================

/**
 * @brief Test that iterating an empty stack yields nothing
 */
TEST_F(LayerStackTest, Iteration_EmptyStack_YieldsNothing) {
	LayerStack stack;

	EXPECT_EQ(stack.begin(), stack.end());
}

/**
 * @brief Test that iterating the stack reaches every layer once
 */
TEST_F(LayerStackTest, Iteration_ReachesEveryLayerOnce) {
	LayerStack stack;
	auto* first = new MockStackLayer{"First"};
	auto* second = new MockStackLayer{"Second"};

	stack.PushLayer(first);
	stack.PushOverlay(second);

	for (const auto& layer: stack)
		layer->OnUpdate();

	EXPECT_EQ(first->updateCount, 1);
	EXPECT_EQ(second->updateCount, 1);
}

// ============================================================================
// Ownership Tests
// ============================================================================

/**
 * @brief Test that Clear detaches and deletes every layer
 */
TEST_F(LayerStackTest, Clear_DetachesAndDeletesEveryLayer) {
	int detachCount = 0;

	LayerStack stack;
	stack.PushLayer(new MockStackLayer{"Layer1", &detachCount});
	stack.PushOverlay(new MockStackLayer{"Overlay1", &detachCount});

	stack.Clear();

	EXPECT_EQ(detachCount, 2);
	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(stack.Size(), 0);
}

/**
 * @brief Test that a cleared stack accepts new layers again
 */
TEST_F(LayerStackTest, Clear_ResetsInsertionPoint) {
	LayerStack stack;
	stack.PushLayer(new MockStackLayer{"Layer1"});
	stack.PushOverlay(new MockStackLayer{"Overlay1"});
	stack.Clear();

	stack.PushOverlay(new MockStackLayer{"Overlay2"});
	stack.PushLayer(new MockStackLayer{"Layer2"});

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"Layer2", "Overlay2"}));
}

/**
 * @brief Test that destroying the stack detaches and deletes the layers it still owns
 */
TEST_F(LayerStackTest, Destructor_DetachesAndDeletesOwnedLayers) {
	int detachCount = 0;

	{
		LayerStack stack;
		stack.PushLayer(new MockStackLayer{"Layer1", &detachCount});
		stack.PushLayer(new MockStackLayer{"Layer2", &detachCount});

		EXPECT_EQ(detachCount, 0);
	}

	EXPECT_EQ(detachCount, 2);
}
