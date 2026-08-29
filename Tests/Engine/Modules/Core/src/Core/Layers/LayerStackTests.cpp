//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers
// File: LayerStackTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Core/Layers/I_Layer.hpp>
#include <Core/Layers/LayerStack.hpp>
#include <Types/Var/Vars.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

using CE::Core::I_Layer;
using CE::Core::LayerStack;

namespace {

/**
 * @brief Mock layer for LayerStack tests
 * @details Ownership is shared: the test holds a shared_ptr next to the stack's own, so a popped layer stays alive
 *			and its counters remain readable after the stack dropped it.
 */
class MockStackLayer: public I_Layer {
public:
	explicit MockStackLayer(const std::string& name = "MockStackLayer"): I_Layer(name) {}

	void OnAttach() override {
		attached = true;
		++attachCount;
	}

	void OnDetach() override {
		attached = false;
		++detachCount;
	}

	void OnUpdate() override { ++updateCount; }
	void OnRender() const override {}
	void Begin(f32) override {}
	void End() override {}

public:
	bool attached = false;
	int attachCount = 0;
	int detachCount = 0;
	int updateCount = 0;
};

std::shared_ptr<MockStackLayer> MakeLayer(const std::string& name = "MockStackLayer") {
	return std::make_shared<MockStackLayer>(name);
}

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
	const auto layer = MakeLayer("Layer");

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
	const auto overlay = MakeLayer("Overlay");

	stack.PushOverlay(overlay);

	EXPECT_EQ(stack.Size(), 1);
	EXPECT_TRUE(overlay->attached);
}

/**
 * @brief Test that the stack shares ownership of the layers it holds
 */
TEST_F(LayerStackTest, PushLayer_SharesOwnershipWithTheCaller) {
	LayerStack stack;
	const auto layer = MakeLayer("Layer");

	EXPECT_EQ(layer.use_count(), 1);

	stack.PushLayer(layer);

	EXPECT_EQ(layer.use_count(), 2);
}

/**
 * @brief Test that layers are inserted before overlays
 * @details Overlays (the ImGui layer, for instance) must always render last, so a layer pushed after an overlay
 *			still lands before it in the iteration order.
 */
TEST_F(LayerStackTest, PushLayer_InsertsBeforeOverlays) {
	LayerStack stack;

	stack.PushOverlay(MakeLayer("Overlay"));
	stack.PushLayer(MakeLayer("First"));
	stack.PushLayer(MakeLayer("Second"));

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

	stack.PushLayer(MakeLayer("Layer1"));
	stack.PushLayer(MakeLayer("Layer2"));
	stack.PushOverlay(MakeLayer("Overlay1"));
	stack.PushOverlay(MakeLayer("Overlay2"));

	EXPECT_EQ(stack.Size(), 4);
}

// ============================================================================
// Pop Tests
// ============================================================================

/**
 * @brief Test that PopLayer removes the layer and detaches it
 */
TEST_F(LayerStackTest, PopLayer_RemovesAndDetachesLayer) {
	LayerStack stack;
	const auto layer = MakeLayer("Layer");

	stack.PushLayer(layer);
	stack.PopLayer(layer);

	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(layer->detachCount, 1);
	EXPECT_FALSE(layer->attached);
}

/**
 * @brief Test that popping a layer releases the stack's share of the ownership
 */
TEST_F(LayerStackTest, PopLayer_ReleasesTheStackOwnership) {
	LayerStack stack;
	const auto layer = MakeLayer("Layer");

	stack.PushLayer(layer);
	stack.PopLayer(layer);

	EXPECT_EQ(layer.use_count(), 1);
}

/**
 * @brief Test that PopOverlay removes the overlay and detaches it
 */
TEST_F(LayerStackTest, PopOverlay_RemovesAndDetachesOverlay) {
	LayerStack stack;
	const auto overlay = MakeLayer("Overlay");

	stack.PushOverlay(overlay);
	stack.PopOverlay(overlay);

	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(overlay->detachCount, 1);
}

/**
 * @brief Test that popping an unknown layer leaves the stack untouched
 */
TEST_F(LayerStackTest, PopLayer_UnknownLayer_LeavesStackUntouched) {
	LayerStack stack;
	const auto known = MakeLayer("Known");
	const auto unknown = MakeLayer("Unknown");

	stack.PushLayer(known);
	stack.PopLayer(unknown);

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
	const auto firstLayer = MakeLayer("First");

	stack.PushOverlay(MakeLayer("Overlay"));
	stack.PushLayer(firstLayer);
	stack.PopLayer(firstLayer);

	stack.PushLayer(MakeLayer("Second"));

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"Second", "Overlay"}));
}

// ============================================================================
// Replace Tests
// ============================================================================

/**
 * @brief Test that ReplaceLayer detaches the old layer and attaches the new one
 */
TEST_F(LayerStackTest, ReplaceLayer_DetachesOldAndAttachesNew) {
	LayerStack stack;
	const auto oldLayer = MakeLayer("Old");
	const auto newLayer = MakeLayer("New");

	stack.PushLayer(oldLayer);
	stack.ReplaceLayer(oldLayer, newLayer);

	EXPECT_EQ(stack.Size(), 1);
	EXPECT_EQ(oldLayer->detachCount, 1);
	EXPECT_FALSE(oldLayer->attached);
	EXPECT_EQ(newLayer->attachCount, 1);
	EXPECT_TRUE(newLayer->attached);
}

/**
 * @brief Test that the old layer is detached before the new one is attached
 * @details ImGui layers create their context in OnAttach and destroy it in OnDetach, so overlapping the two would
 *			leave the stale context current.
 */
TEST_F(LayerStackTest, ReplaceLayer_DetachesBeforeAttaching) {
	LayerStack stack;
	const auto oldLayer = MakeLayer("Old");
	const auto newLayer = MakeLayer("New");

	stack.PushOverlay(oldLayer);

	EXPECT_EQ(oldLayer->detachCount, 0);

	stack.ReplaceLayer(oldLayer, newLayer);

	EXPECT_EQ(oldLayer->detachCount, 1);
	EXPECT_EQ(newLayer->attachCount, 1);
}

/**
 * @brief Test that the replacement keeps the position of the entry it took over
 * @details Replacing in place (instead of popping and pushing) is what keeps an overlay an overlay: a PushOverlay
 *			would move it past the layers, a PushLayer would move it before the other overlays.
 */
TEST_F(LayerStackTest, ReplaceLayer_KeepsThePositionOfTheReplacedEntry) {
	LayerStack stack;
	const auto oldOverlay = MakeLayer("OldOverlay");
	const auto newOverlay = MakeLayer("NewOverlay");

	stack.PushOverlay(oldOverlay);
	stack.PushLayer(MakeLayer("Layer"));
	stack.ReplaceLayer(oldOverlay, newOverlay);

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"Layer", "NewOverlay"}));
}

/**
 * @brief Test that replacing keeps the insertion point consistent
 */
TEST_F(LayerStackTest, ReplaceLayer_KeepsInsertionPointConsistent) {
	LayerStack stack;
	const auto oldLayer = MakeLayer("Old");

	stack.PushOverlay(MakeLayer("Overlay"));
	stack.PushLayer(oldLayer);
	stack.ReplaceLayer(oldLayer, MakeLayer("New"));

	stack.PushLayer(MakeLayer("Second"));

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"New", "Second", "Overlay"}));
}

/**
 * @brief Test that replacing releases the stack's share of the old layer's ownership
 */
TEST_F(LayerStackTest, ReplaceLayer_ReleasesTheOldLayerOwnership) {
	LayerStack stack;
	const auto oldLayer = MakeLayer("Old");

	stack.PushLayer(oldLayer);
	stack.ReplaceLayer(oldLayer, MakeLayer("New"));

	EXPECT_EQ(oldLayer.use_count(), 1);
}

/**
 * @brief Test that replacing an unknown layer leaves the stack untouched
 */
TEST_F(LayerStackTest, ReplaceLayer_UnknownLayer_LeavesStackUntouched) {
	LayerStack stack;
	const auto known = MakeLayer("Known");
	const auto unknown = MakeLayer("Unknown");
	const auto replacement = MakeLayer("Replacement");

	stack.PushLayer(known);
	stack.ReplaceLayer(unknown, replacement);

	EXPECT_EQ(stack.Size(), 1);
	EXPECT_TRUE(known->attached);
	EXPECT_EQ(replacement->attachCount, 0);
	EXPECT_EQ(unknown->detachCount, 0);
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
	const auto first = MakeLayer("First");
	const auto second = MakeLayer("Second");

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
 * @brief Test that Clear detaches and releases every layer
 */
TEST_F(LayerStackTest, Clear_DetachesAndReleasesEveryLayer) {
	LayerStack stack;
	const auto layer = MakeLayer("Layer1");
	const auto overlay = MakeLayer("Overlay1");

	stack.PushLayer(layer);
	stack.PushOverlay(overlay);

	stack.Clear();

	EXPECT_EQ(layer->detachCount, 1);
	EXPECT_EQ(overlay->detachCount, 1);
	EXPECT_EQ(layer.use_count(), 1);
	EXPECT_EQ(overlay.use_count(), 1);
	EXPECT_TRUE(stack.Empty());
	EXPECT_EQ(stack.Size(), 0);
}

/**
 * @brief Test that a cleared stack accepts new layers again
 */
TEST_F(LayerStackTest, Clear_ResetsInsertionPoint) {
	LayerStack stack;
	stack.PushLayer(MakeLayer("Layer1"));
	stack.PushOverlay(MakeLayer("Overlay1"));
	stack.Clear();

	stack.PushOverlay(MakeLayer("Overlay2"));
	stack.PushLayer(MakeLayer("Layer2"));

	std::vector<std::string> order;
	for (const auto& layer: stack)
		order.push_back(layer->GetName());

	EXPECT_EQ(order, (std::vector<std::string>{"Layer2", "Overlay2"}));
}

/**
 * @brief Test that destroying the stack detaches and releases the layers it still holds
 */
TEST_F(LayerStackTest, Destructor_DetachesAndReleasesOwnedLayers) {
	const auto first = MakeLayer("Layer1");
	const auto second = MakeLayer("Layer2");

	{
		LayerStack stack;
		stack.PushLayer(first);
		stack.PushLayer(second);

		EXPECT_EQ(first->detachCount, 0);
		EXPECT_EQ(first.use_count(), 2);
	}

	EXPECT_EQ(first->detachCount, 1);
	EXPECT_EQ(second->detachCount, 1);
	EXPECT_EQ(first.use_count(), 1);
	EXPECT_EQ(second.use_count(), 1);
}

/**
 * @brief Test that the stack keeps a layer alive after the caller dropped its own reference
 */
TEST_F(LayerStackTest, PushLayer_KeepsTheLayerAliveAfterTheCallerReleasesIt) {
	LayerStack stack;
	std::weak_ptr<MockStackLayer> observer;

	{
		const auto layer = MakeLayer("Layer");
		observer = layer;
		stack.PushLayer(layer);
	}

	EXPECT_FALSE(observer.expired());

	stack.Clear();

	EXPECT_TRUE(observer.expired());
}
