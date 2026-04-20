//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include <Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>
#include <Types/Window/WindowProps.hpp>

#include <memory>
#include <gtest/gtest.h>


using namespace CE::Core::Application;
using namespace CE::Events;
using namespace CE::Layers;
using namespace CE::Tools::Log;
using namespace CE::KeyCode;
using namespace CE::Types::Render;
using namespace CE::Types::Window;


const WindowProps windowProps{"Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};

/**
 * @brief Test fixture for ImGuiMetalLayer tests
 * @details These tests require an Application instance because ImGuiMetalLayer
 *			needs a valid Metal context and window to initialize properly.
 *			Metal is only available on macOS.
 */
class ImGuiMetalLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();

		try {
			_app = std::make_unique<CocoaApplication>();
			_app->Init(windowProps);
		}
		catch (...) {
			GTEST_SKIP() << "Window not available (no display)";
		}
	}

	void TearDown() override {
		_app.reset();
		Log::Shutdown();
	}
	
public:
	std::unique_ptr<CocoaApplication> _app;
};

/**
 * @brief Test ImGuiMetalLayer construction
 */
TEST_F(ImGuiMetalLayerTest, Construction) {
	const auto imguiLayer = new ImGuiMetalLayer();
	EXPECT_NE(imguiLayer, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(imguiLayer->GetDebugName(), "ImGuiMetalLayer");
#endif

	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnAttach) {

	const auto imguiLayer = new ImGuiMetalLayer();
	EXPECT_NO_THROW({
		_app->SetImGuiLayer(imguiLayer);
	});

	// Clean up - pop the layer before the test ends
	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnDetach) {

	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		_app->RemoveImGuiLayer();
	});

	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnUpdate
 */
TEST_F(ImGuiMetalLayerTest, OnUpdate) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		_app->Tick(0.016f);
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalLayerTest, MultipleOnUpdate) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		_app->Tick(0.016f);
		_app->Tick(0.016f);
		_app->Tick(0.016f);
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyPressed) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	KeyPressedEvent event{KeyboardKeyCode::A, 0};
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyReleased) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	KeyReleasedEvent event{KeyboardKeyCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyTyped) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonPressed) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonReleased) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseMoved) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	MouseMovedEvent event{100.0f, 200.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseScrolled) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	MouseScrolledEvent event{0.0f, 1.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalLayerTest, OnEventWindowResize) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	WindowResizeEvent event{1024, 768};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer handling multiple events
 */
TEST_F(ImGuiMetalLayerTest, MultipleEvents) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};
	WindowResizeEvent resizeEvent{1024, 768};

	EXPECT_NO_THROW({
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);
		_app->OnEvent(resizeEvent);
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer full lifecycle
 */
TEST_F(ImGuiMetalLayerTest, FullLifecycle) {
	const auto imguiLayer = new ImGuiMetalLayer();

	// Attach
	EXPECT_NO_THROW({
		_app->SetImGuiLayer(imguiLayer);
	});

	// Update multiple times
	EXPECT_NO_THROW({
		_app->Tick(0.016f);
		_app->Tick(0.016f);
	});

	// Handle events
	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};

	EXPECT_NO_THROW({
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);
	});

	// Update again
	EXPECT_NO_THROW({
		_app->Tick(0.016f);
	});

	// Detach
	EXPECT_NO_THROW({
		_app->RemoveImGuiLayer();
	});

	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer Begin and End methods
 */
TEST_F(ImGuiMetalLayerTest, BeginEnd) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		imguiLayer->Begin();
		imguiLayer->End();
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer multiple Begin/End cycles
 */
TEST_F(ImGuiMetalLayerTest, MultipleBeginEnd) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		imguiLayer->Begin();
		imguiLayer->End();
		imguiLayer->Begin();
		imguiLayer->End();
		imguiLayer->Begin();
		imguiLayer->End();
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer OnRender
 */
TEST_F(ImGuiMetalLayerTest, OnRender) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		imguiLayer->Begin();
		imguiLayer->OnRender();
		imguiLayer->End();
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer complete render cycle
 */
TEST_F(ImGuiMetalLayerTest, CompleteRenderCycle) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	EXPECT_NO_THROW({
		// First frame
		imguiLayer->Begin();
		imguiLayer->OnRender();
		imguiLayer->End();

		// Second frame
		imguiLayer->Begin();
		imguiLayer->OnRender();
		imguiLayer->End();

		// Third frame
		imguiLayer->Begin();
		imguiLayer->OnRender();
		imguiLayer->End();
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}

/**
 * @brief Test ImGuiMetalLayer with events during render
 */
TEST_F(ImGuiMetalLayerTest, EventsDuringRender) {
	const auto imguiLayer = new ImGuiMetalLayer();
	_app->SetImGuiLayer(imguiLayer);

	// Prepare events
	auto keyEvent = KeyPressedEvent{KeyboardKeyCode::Space, 0};
	auto mouseEvent = MouseMovedEvent{150.0f, 250.0f};

	EXPECT_NO_THROW({
		// Start rendering
		imguiLayer->Begin();

		// Handle events
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);

		// Continue rendering
		imguiLayer->OnRender();
		imguiLayer->End();
	});

	_app->RemoveImGuiLayer();
	delete imguiLayer;
}
