//
// Module: CelestialEngine/Tests/Engine/Modules/Native/Apple/MetalCpp/MetalKit
// File: MtkViewDelegateTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

using CE::Native::MtkViewDelegate;

namespace {

/**
 * @brief Listener recording the draw / resize callbacks handed to the view delegate
 * @details The delegate only forwards the view pointer, so an opaque non-null address stands in for a real
 *			MTK::View, which would require a Metal device.
 */
class ViewCallbackListener {
public:
	void OnDraw(MTK::View* view) {
		++drawCalls;
		lastView = view;
	}

	void OnResize(MTK::View* view, const CGSize size) {
		++resizeCalls;
		lastView = view;
		lastSize = size;
	}

public:
	int drawCalls = 0;
	int resizeCalls = 0;
	MTK::View* lastView = nullptr;
	CGSize lastSize{};
};

/**
 * @brief Opaque, never-dereferenced stand-in for a MetalKit view
 */
MTK::View* MakeFakeView() { return reinterpret_cast<MTK::View*>(0xCAFE); }

/**
 * @brief Test fixture for MtkViewDelegate tests
 * @details The MetalKit view drives the frame loop through this delegate: drawInMTKView produces a frame, and
 *			drawableSizeWillChange reports the new drawable size the render context must reconcile with.
 */
class MtkViewDelegateTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

/**
 * @brief Test that the draw callback reaches the bound delegate with the originating view
 */
TEST_F(MtkViewDelegateTest, DrawInMTKView_ForwardsViewToDelegate) {
	ViewCallbackListener listener;
	MtkViewDelegate delegate;
	delegate.SetDrawInMtkViewDelegate(
		EventDelegate<MTK::View*>::FromMethod<ViewCallbackListener, &ViewCallbackListener::OnDraw>(&listener)
	);

	auto* view = MakeFakeView();
	delegate.drawInMTKView(view);
	delegate.drawInMTKView(view);

	EXPECT_EQ(listener.drawCalls, 2);
	EXPECT_EQ(listener.lastView, view);
}

/**
 * @brief Test that the drawable-resize callback forwards both the view and the new size
 */
TEST_F(MtkViewDelegateTest, DrawableSizeWillChange_ForwardsViewAndSize) {
	ViewCallbackListener listener;
	MtkViewDelegate delegate;
	delegate.SetDrawableSizeWillChangeDelegate(
		EventDelegate<MTK::View*, CGSize>::FromMethod<ViewCallbackListener, &ViewCallbackListener::OnResize>(&listener)
	);

	auto* view = MakeFakeView();
	delegate.drawableSizeWillChange(view, CGSize{.width = 1280.0, .height = 720.0});

	EXPECT_EQ(listener.resizeCalls, 1);
	EXPECT_EQ(listener.lastView, view);
	EXPECT_DOUBLE_EQ(listener.lastSize.width, 1280.0);
	EXPECT_DOUBLE_EQ(listener.lastSize.height, 720.0);
}

/**
 * @brief Test that the resize callback is optional
 * @details Unlike the draw callback, no subscriber is required: the delegate must stay silent instead of throwing.
 */
TEST_F(MtkViewDelegateTest, DrawableSizeWillChange_WithoutDelegate_IsNoOp) {
	MtkViewDelegate delegate;

	EXPECT_NO_THROW(delegate.drawableSizeWillChange(MakeFakeView(), CGSize{.width = 800.0, .height = 600.0}));
}

/**
 * @brief Test that the draw and resize channels stay independent
 */
TEST_F(MtkViewDelegateTest, DrawAndResizeChannels_AreIndependent) {
	ViewCallbackListener listener;
	MtkViewDelegate delegate;
	delegate.SetDrawInMtkViewDelegate(
		EventDelegate<MTK::View*>::FromMethod<ViewCallbackListener, &ViewCallbackListener::OnDraw>(&listener));
	delegate.SetDrawableSizeWillChangeDelegate(
		EventDelegate<MTK::View*, CGSize>::FromMethod<ViewCallbackListener, &ViewCallbackListener::OnResize>(&listener));

	delegate.drawInMTKView(MakeFakeView());

	EXPECT_EQ(listener.drawCalls, 1);
	EXPECT_EQ(listener.resizeCalls, 0);

	delegate.drawableSizeWillChange(MakeFakeView(), CGSize{.width = 640.0, .height = 480.0});

	EXPECT_EQ(listener.drawCalls, 1);
	EXPECT_EQ(listener.resizeCalls, 1);
}
