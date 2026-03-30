//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-18
// Updated by: Catalin Chirosca
// Updated: 2026-03-30
//

#include <Core/Application.hpp>
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

using namespace CE::Core;
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
			_app = std::make_unique<Application>();
			_app->InitWindow(windowProps);
			_app->InitRenderer(windowProps);
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
	std::unique_ptr<Application> _app;
};

/**
 * @brief Test ImGuiMetalLayer construction
 */
TEST_F(ImGuiMetalLayerTest, Construction) {
	const auto layerConstruction = new ImGuiMetalLayer();
	EXPECT_NE(layerConstruction, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layerConstruction->GetDebugName(), "ImGuiMetalLayer");
#endif

	delete layerConstruction;
}

/**
 * @brief Test ImGuiMetalLayer OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnAttach) {

	const auto layerOnAttach = new ImGuiMetalLayer();
	EXPECT_NO_THROW({
		_app->PushLayer(layerOnAttach);
		_app->SetRenderLayer(layerOnAttach);
	});

	// Clean up - pop the layer before the test ends
	_app->PopLayer(layerOnAttach);
	delete layerOnAttach;
}

/**
 * @brief Test ImGuiMetalLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnDetach) {

	const auto layerOnDetach = new ImGuiMetalLayer();
	_app->PushLayer(layerOnDetach);
	_app->SetRenderLayer(layerOnDetach);

	EXPECT_NO_THROW({
		_app->PopLayer(layerOnDetach);
	});

	delete layerOnDetach;
}

/**
 * @brief Test ImGuiMetalLayer OnUpdate
 */
TEST_F(ImGuiMetalLayerTest, OnUpdate) {
	const auto layerOnUpdate = new ImGuiMetalLayer();
	_app->PushLayer(layerOnUpdate);
	_app->SetRenderLayer(layerOnUpdate);

	EXPECT_NO_THROW({
		_app->Update();
	});

	_app->PopLayer(layerOnUpdate);
	delete layerOnUpdate;
}

/**
 * @brief Test ImGuiMetalLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalLayerTest, MultipleOnUpdate) {
	const auto layerMultipleOnUpdate = new ImGuiMetalLayer();
	_app->PushLayer(layerMultipleOnUpdate);
	_app->SetRenderLayer(layerMultipleOnUpdate);

	EXPECT_NO_THROW({
		_app->Update();
		_app->Update();
		_app->Update();
	});

	_app->PopLayer(layerMultipleOnUpdate);
	delete layerMultipleOnUpdate;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyPressed) {
	const auto layerOnEventKeyPressed = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventKeyPressed);
	_app->SetRenderLayer(layerOnEventKeyPressed);

	KeyPressedEvent event{KeyboardKeyCode::A, 0};
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventKeyPressed);
	delete layerOnEventKeyPressed;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyReleased) {
	const auto layerOnEventKeyReleased = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventKeyReleased);
	_app->SetRenderLayer(layerOnEventKeyReleased);

	KeyReleasedEvent event{KeyboardKeyCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventKeyReleased);
	delete layerOnEventKeyReleased;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyTyped) {
	const auto layerOnEventKeyTyped = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventKeyTyped);
	_app->SetRenderLayer(layerOnEventKeyTyped);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventKeyTyped);
	delete layerOnEventKeyTyped;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonPressed) {
	const auto layerOnEventMouseButtonPressed = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventMouseButtonPressed);
	_app->SetRenderLayer(layerOnEventMouseButtonPressed);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventMouseButtonPressed);
	delete layerOnEventMouseButtonPressed;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonReleased) {
	const auto layerOnEventMouseButtonReleased = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventMouseButtonReleased);
	_app->SetRenderLayer(layerOnEventMouseButtonReleased);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventMouseButtonReleased);
	delete layerOnEventMouseButtonReleased;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseMoved) {
	const auto layerOnEventMouseMoved = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventMouseMoved);
	_app->SetRenderLayer(layerOnEventMouseMoved);

	MouseMovedEvent event{100.0f, 200.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventMouseMoved);
	delete layerOnEventMouseMoved;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseScrolled) {
	const auto layerOnEventMouseScrolled = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventMouseScrolled);
	_app->SetRenderLayer(layerOnEventMouseScrolled);

	MouseScrolledEvent event{0.0f, 1.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventMouseScrolled);
	delete layerOnEventMouseScrolled;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalLayerTest, OnEventWindowResize) {
	const auto layerOnEventWindowResize = new ImGuiMetalLayer();
	_app->PushLayer(layerOnEventWindowResize);
	_app->SetRenderLayer(layerOnEventWindowResize);

	WindowResizeEvent event{1024, 768};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layerOnEventWindowResize);
	delete layerOnEventWindowResize;
}

/**
 * @brief Test ImGuiMetalLayer handling multiple events
 */
TEST_F(ImGuiMetalLayerTest, MultipleEvents) {
	const auto layerMultipleEvents = new ImGuiMetalLayer();
	_app->PushLayer(layerMultipleEvents);
	_app->SetRenderLayer(layerMultipleEvents);

	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};
	WindowResizeEvent resizeEvent{1024, 768};

	EXPECT_NO_THROW({
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);
		_app->OnEvent(resizeEvent);
	});

	_app->PopLayer(layerMultipleEvents);
	delete layerMultipleEvents;
}

/**
 * @brief Test ImGuiMetalLayer full lifecycle
 */
TEST_F(ImGuiMetalLayerTest, FullLifecycle) {
	const auto layerFullLifecycle = new ImGuiMetalLayer();

	// Attach
	EXPECT_NO_THROW({
		_app->PushLayer(layerFullLifecycle);
		_app->SetRenderLayer(layerFullLifecycle);
	});

	// Update multiple times
	EXPECT_NO_THROW({
		_app->Update();
		_app->Update();
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
		_app->Update();
	});

	// Detach
	EXPECT_NO_THROW({
		_app->PopLayer(layerFullLifecycle);
	});

	delete layerFullLifecycle;
}

/**
 * @brief Test ImGuiMetalLayer Begin and End methods
 */
TEST_F(ImGuiMetalLayerTest, BeginEnd) {
	const auto layerBeginEnd = new ImGuiMetalLayer();
	_app->PushLayer(layerBeginEnd);
	_app->SetRenderLayer(layerBeginEnd);

	EXPECT_NO_THROW({
		layerBeginEnd->Begin();
		layerBeginEnd->End();
	});

	_app->PopLayer(layerBeginEnd);
	delete layerBeginEnd;
}

/**
 * @brief Test ImGuiMetalLayer multiple Begin/End cycles
 */
TEST_F(ImGuiMetalLayerTest, MultipleBeginEnd) {
	const auto layerMultipleBeginEnd = new ImGuiMetalLayer();
	_app->PushLayer(layerMultipleBeginEnd);
	_app->SetRenderLayer(layerMultipleBeginEnd);

	EXPECT_NO_THROW({
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
	});

	_app->PopLayer(layerMultipleBeginEnd);
	delete layerMultipleBeginEnd;
}

/**
 * @brief Test ImGuiMetalLayer OnRender
 */
TEST_F(ImGuiMetalLayerTest, OnRender) {
	const auto layerOnRender = new ImGuiMetalLayer();
	_app->PushLayer(layerOnRender);
	_app->SetRenderLayer(layerOnRender);

	EXPECT_NO_THROW({
		layerOnRender->Begin();
		layerOnRender->OnRender();
		layerOnRender->End();
	});

	_app->PopLayer(layerOnRender);
	delete layerOnRender;
}

/**
 * @brief Test ImGuiMetalLayer complete render cycle
 */
TEST_F(ImGuiMetalLayerTest, CompleteRenderCycle) {
	const auto layerCompleteRenderCycle = new ImGuiMetalLayer();
	_app->PushLayer(layerCompleteRenderCycle);
	_app->SetRenderLayer(layerCompleteRenderCycle);

	EXPECT_NO_THROW({
		// First frame
		layerCompleteRenderCycle->Begin();
		layerCompleteRenderCycle->OnRender();
		layerCompleteRenderCycle->End();

		// Second frame
		layerCompleteRenderCycle->Begin();
		layerCompleteRenderCycle->OnRender();
		layerCompleteRenderCycle->End();

		// Third frame
		layerCompleteRenderCycle->Begin();
		layerCompleteRenderCycle->OnRender();
		layerCompleteRenderCycle->End();
	});

	_app->PopLayer(layerCompleteRenderCycle);
	delete layerCompleteRenderCycle;
}

/**
 * @brief Test ImGuiMetalLayer with events during render
 */
TEST_F(ImGuiMetalLayerTest, EventsDuringRender) {
	const auto layerEventsDuringRender = new ImGuiMetalLayer();
	_app->PushLayer(layerEventsDuringRender);
	_app->SetRenderLayer(layerEventsDuringRender);

	// Prepare events
	auto keyEvent = KeyPressedEvent{KeyboardKeyCode::Space, 0};
	auto mouseEvent = MouseMovedEvent{150.0f, 250.0f};

	EXPECT_NO_THROW({
		// Start rendering
		layerEventsDuringRender->Begin();

		// Handle events
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);

		// Continue rendering
		layerEventsDuringRender->OnRender();
		layerEventsDuringRender->End();
	});

	_app->PopLayer(layerEventsDuringRender);
	delete layerEventsDuringRender;
}
