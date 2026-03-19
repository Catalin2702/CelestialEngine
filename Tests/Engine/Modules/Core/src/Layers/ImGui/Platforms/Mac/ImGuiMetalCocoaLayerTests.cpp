//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalCocoaLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-18
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGui/Platforms/Mac/ImGuiMetalCocoaLayer.hpp>
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

constexpr WindowProps windowProps{"Test", 800, 600, false, GraphicsApi::Metal, WindowApi::Cocoa};


/**
 * @brief Test fixture for ImGuiMetalCocoaLayer tests
 * @details These tests require an Application instance because ImGuiMetalCocoaLayer
 *			needs a valid Metal context and window to initialize properly.
 *			Metal is only available on macOS.
 */
class ImGuiMetalCocoaLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}

	void TearDown() override {
		Log::Shutdown();
	}
};

/**
 * @brief Test ImGuiMetalCocoaLayer construction
 */
TEST_F(ImGuiMetalCocoaLayerTest, Construction) {
	const auto layerConstruction = new ImGuiMetalCocoaLayer();
	EXPECT_NE(layerConstruction, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layerConstruction->GetDebugName(), "ImGuiMetalCocoaLayer");
#endif

	delete layerConstruction;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnAttach
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnAttach) {
	std::unique_ptr<Application> _appOnAttach;
	try {
		_appOnAttach = std::make_unique<Application>();
		_appOnAttach->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnAttach = new ImGuiMetalCocoaLayer();
	EXPECT_NO_THROW({
		_appOnAttach->PushLayer(layerOnAttach);
		_appOnAttach->SetRenderLayer(layerOnAttach);
	});

	// Clean up - pop the layer before the test ends
	_appOnAttach->PopLayer(layerOnAttach);
	delete layerOnAttach;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnDetach) {
	std::unique_ptr<Application> _appOnDetach;
	try {
		_appOnDetach = std::make_unique<Application>();
		_appOnDetach->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnDetach = new ImGuiMetalCocoaLayer();
	_appOnDetach->PushLayer(layerOnDetach);
	_appOnDetach->SetRenderLayer(layerOnDetach);

	EXPECT_NO_THROW({
		_appOnDetach->PopLayer(layerOnDetach);
	});

	delete layerOnDetach;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnUpdate
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnUpdate) {
	std::unique_ptr<Application> _appOnUpdate;
	try {
		_appOnUpdate = std::make_unique<Application>();
		_appOnUpdate->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnUpdate = new ImGuiMetalCocoaLayer();
	_appOnUpdate->PushLayer(layerOnUpdate);
	_appOnUpdate->SetRenderLayer(layerOnUpdate);

	EXPECT_NO_THROW({
		_appOnUpdate->Update();
	});

	_appOnUpdate->PopLayer(layerOnUpdate);
	delete layerOnUpdate;
}

/**
 * @brief Test ImGuiMetalCocoaLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalCocoaLayerTest, MultipleOnUpdate) {
	std::unique_ptr<Application> _appMultipleOnUpdate;
	try {
		_appMultipleOnUpdate = std::make_unique<Application>();
		_appMultipleOnUpdate->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleOnUpdate = new ImGuiMetalCocoaLayer();
	_appMultipleOnUpdate->PushLayer(layerMultipleOnUpdate);
	_appMultipleOnUpdate->SetRenderLayer(layerMultipleOnUpdate);

	EXPECT_NO_THROW({
		_appMultipleOnUpdate->Update();
		_appMultipleOnUpdate->Update();
		_appMultipleOnUpdate->Update();
	});

	_appMultipleOnUpdate->PopLayer(layerMultipleOnUpdate);
	delete layerMultipleOnUpdate;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventKeyPressed) {
	std::unique_ptr<Application> _appOnEventKeyPressed;
	try {
		_appOnEventKeyPressed = std::make_unique<Application>();
		_appOnEventKeyPressed->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyPressed = new ImGuiMetalCocoaLayer();
	_appOnEventKeyPressed->PushLayer(layerOnEventKeyPressed);
	_appOnEventKeyPressed->SetRenderLayer(layerOnEventKeyPressed);

	KeyPressedEvent event{KeyboardKeyCode::A, 0};
	_appOnEventKeyPressed->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_appOnEventKeyPressed->PopLayer(layerOnEventKeyPressed);
	delete layerOnEventKeyPressed;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventKeyReleased) {
	std::unique_ptr<Application> _appOnEventKeyReleased;
	try {
		_appOnEventKeyReleased = std::make_unique<Application>();
		_appOnEventKeyReleased->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyReleased = new ImGuiMetalCocoaLayer();
	_appOnEventKeyReleased->PushLayer(layerOnEventKeyReleased);
	_appOnEventKeyReleased->SetRenderLayer(layerOnEventKeyReleased);

	KeyReleasedEvent event{KeyboardKeyCode::A};
	_appOnEventKeyReleased->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventKeyReleased->PopLayer(layerOnEventKeyReleased);
	delete layerOnEventKeyReleased;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventKeyTyped) {
	std::unique_ptr<Application> _appOnEventKeyTyped;
	try {
		_appOnEventKeyTyped = std::make_unique<Application>();
		_appOnEventKeyTyped->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyTyped = new ImGuiMetalCocoaLayer();
	_appOnEventKeyTyped->PushLayer(layerOnEventKeyTyped);
	_appOnEventKeyTyped->SetRenderLayer(layerOnEventKeyTyped);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_appOnEventKeyTyped->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventKeyTyped->PopLayer(layerOnEventKeyTyped);
	delete layerOnEventKeyTyped;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventMouseButtonPressed) {
	std::unique_ptr<Application> _appOnEventMouseButtonPressed;
	try {
		_appOnEventMouseButtonPressed = std::make_unique<Application>();
		_appOnEventMouseButtonPressed->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseButtonPressed = new ImGuiMetalCocoaLayer();
	_appOnEventMouseButtonPressed->PushLayer(layerOnEventMouseButtonPressed);
	_appOnEventMouseButtonPressed->SetRenderLayer(layerOnEventMouseButtonPressed);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_appOnEventMouseButtonPressed->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseButtonPressed->PopLayer(layerOnEventMouseButtonPressed);
	delete layerOnEventMouseButtonPressed;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventMouseButtonReleased) {
	std::unique_ptr<Application> _appOnEventMouseButtonReleased;
	try {
		_appOnEventMouseButtonReleased = std::make_unique<Application>();
		_appOnEventMouseButtonReleased->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseButtonReleased = new ImGuiMetalCocoaLayer();
	_appOnEventMouseButtonReleased->PushLayer(layerOnEventMouseButtonReleased);
	_appOnEventMouseButtonReleased->SetRenderLayer(layerOnEventMouseButtonReleased);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_appOnEventMouseButtonReleased->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseButtonReleased->PopLayer(layerOnEventMouseButtonReleased);
	delete layerOnEventMouseButtonReleased;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventMouseMoved) {
	std::unique_ptr<Application> _appOnEventMouseMoved;
	try {
		_appOnEventMouseMoved = std::make_unique<Application>();
		_appOnEventMouseMoved->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseMoved = new ImGuiMetalCocoaLayer();
	_appOnEventMouseMoved->PushLayer(layerOnEventMouseMoved);
	_appOnEventMouseMoved->SetRenderLayer(layerOnEventMouseMoved);

	MouseMovedEvent event{100.0f, 200.0f};
	_appOnEventMouseMoved->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseMoved->PopLayer(layerOnEventMouseMoved);
	delete layerOnEventMouseMoved;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventMouseScrolled) {
	std::unique_ptr<Application> _appOnEventMouseScrolled;
	try {
		_appOnEventMouseScrolled = std::make_unique<Application>();
		_appOnEventMouseScrolled->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseScrolled = new ImGuiMetalCocoaLayer();
	_appOnEventMouseScrolled->PushLayer(layerOnEventMouseScrolled);
	_appOnEventMouseScrolled->SetRenderLayer(layerOnEventMouseScrolled);


	MouseScrolledEvent event{0.0f, 1.0f};
	_appOnEventMouseScrolled->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseScrolled->PopLayer(layerOnEventMouseScrolled);
	delete layerOnEventMouseScrolled;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnEventWindowResize) {
	std::unique_ptr<Application> _appOnEventWindowResize;
	try {
		_appOnEventWindowResize = std::make_unique<Application>();
		_appOnEventWindowResize->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventWindowResize = new ImGuiMetalCocoaLayer();
	_appOnEventWindowResize->PushLayer(layerOnEventWindowResize);
	_appOnEventWindowResize->SetRenderLayer(layerOnEventWindowResize);

	WindowResizeEvent event{1024, 768};
	_appOnEventWindowResize->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventWindowResize->PopLayer(layerOnEventWindowResize);
	delete layerOnEventWindowResize;
}

/**
 * @brief Test ImGuiMetalCocoaLayer handling multiple events
 */
TEST_F(ImGuiMetalCocoaLayerTest, MultipleEvents) {
	std::unique_ptr<Application> _appMultipleEvents;
	try {
		_appMultipleEvents = std::make_unique<Application>();
		_appMultipleEvents->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleEvents = new ImGuiMetalCocoaLayer();
	_appMultipleEvents->PushLayer(layerMultipleEvents);
	_appMultipleEvents->SetRenderLayer(layerMultipleEvents);

	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};
	WindowResizeEvent resizeEvent{1024, 768};

	EXPECT_NO_THROW({
		_appMultipleEvents->OnEvent(keyEvent);
		_appMultipleEvents->OnEvent(mouseEvent);
		_appMultipleEvents->OnEvent(resizeEvent);
	});

	_appMultipleEvents->PopLayer(layerMultipleEvents);
	delete layerMultipleEvents;
}

/**
 * @brief Test ImGuiMetalCocoaLayer full lifecycle
 */
TEST_F(ImGuiMetalCocoaLayerTest, FullLifecycle) {
	std::unique_ptr<Application> _appFullLifecycle;
	try {
		_appFullLifecycle = std::make_unique<Application>();
		_appFullLifecycle->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerFullLifecycle = new ImGuiMetalCocoaLayer();

	// Attach
	EXPECT_NO_THROW({
		_appFullLifecycle->PushLayer(layerFullLifecycle);
		_appFullLifecycle->SetRenderLayer(layerFullLifecycle);
	});

	// Update multiple times
	EXPECT_NO_THROW({
		_appFullLifecycle->Update();
		_appFullLifecycle->Update();
	});

	// Handle events
	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};

	EXPECT_NO_THROW({
		_appFullLifecycle->OnEvent(keyEvent);
		_appFullLifecycle->OnEvent(mouseEvent);
	});

	// Update again
	EXPECT_NO_THROW({
		_appFullLifecycle->Update();
	});

	// Detach
	EXPECT_NO_THROW({
		_appFullLifecycle->PopLayer(layerFullLifecycle);
	});

	delete layerFullLifecycle;
}

/**
 * @brief Test ImGuiMetalCocoaLayer Begin and End methods
 */
TEST_F(ImGuiMetalCocoaLayerTest, BeginEnd) {
	std::unique_ptr<Application> _appBeginEnd;
	try {
		_appBeginEnd = std::make_unique<Application>();
		_appBeginEnd->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerBeginEnd = new ImGuiMetalCocoaLayer();
	_appBeginEnd->PushLayer(layerBeginEnd);
	_appBeginEnd->SetRenderLayer(layerBeginEnd);

	EXPECT_NO_THROW({
		layerBeginEnd->Begin();
		layerBeginEnd->End();
	});

	_appBeginEnd->PopLayer(layerBeginEnd);
	delete layerBeginEnd;
}

/**
 * @brief Test ImGuiMetalCocoaLayer multiple Begin/End cycles
 */
TEST_F(ImGuiMetalCocoaLayerTest, MultipleBeginEnd) {
	std::unique_ptr<Application> _appMultipleBeginEnd;
	try {
		_appMultipleBeginEnd = std::make_unique<Application>();
		_appMultipleBeginEnd->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleBeginEnd = new ImGuiMetalCocoaLayer();
	_appMultipleBeginEnd->PushLayer(layerMultipleBeginEnd);
	_appMultipleBeginEnd->SetRenderLayer(layerMultipleBeginEnd);

	EXPECT_NO_THROW({
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
		layerMultipleBeginEnd->Begin();
		layerMultipleBeginEnd->End();
	});

	_appMultipleBeginEnd->PopLayer(layerMultipleBeginEnd);
	delete layerMultipleBeginEnd;
}

/**
 * @brief Test ImGuiMetalCocoaLayer OnRender
 */
TEST_F(ImGuiMetalCocoaLayerTest, OnRender) {
	std::unique_ptr<Application> _appOnRender;
	try {
		_appOnRender = std::make_unique<Application>();
		_appOnRender->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnRender = new ImGuiMetalCocoaLayer();
	_appOnRender->PushLayer(layerOnRender);
	_appOnRender->SetRenderLayer(layerOnRender);

	EXPECT_NO_THROW({
		layerOnRender->Begin();
		layerOnRender->OnRender();
		layerOnRender->End();
	});

	_appOnRender->PopLayer(layerOnRender);
	delete layerOnRender;
}

/**
 * @brief Test ImGuiMetalCocoaLayer complete render cycle
 */
TEST_F(ImGuiMetalCocoaLayerTest, CompleteRenderCycle) {
	std::unique_ptr<Application> _appCompleteRenderCycle;
	try {
		_appCompleteRenderCycle = std::make_unique<Application>();
		_appCompleteRenderCycle->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerCompleteRenderCycle = new ImGuiMetalCocoaLayer();
	_appCompleteRenderCycle->PushLayer(layerCompleteRenderCycle);
	_appCompleteRenderCycle->SetRenderLayer(layerCompleteRenderCycle);

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

	_appCompleteRenderCycle->PopLayer(layerCompleteRenderCycle);
	delete layerCompleteRenderCycle;
}

/**
 * @brief Test ImGuiMetalCocoaLayer with events during render
 */
TEST_F(ImGuiMetalCocoaLayerTest, EventsDuringRender) {
	std::unique_ptr<Application> _appEventsDuringRender;
	try {
		_appEventsDuringRender = std::make_unique<Application>();
		_appEventsDuringRender->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerEventsDuringRender = new ImGuiMetalCocoaLayer();
	_appEventsDuringRender->PushLayer(layerEventsDuringRender);
	_appEventsDuringRender->SetRenderLayer(layerEventsDuringRender);

	// Prepare events
	auto keyEvent = KeyPressedEvent{KeyboardKeyCode::Space, 0};
	auto mouseEvent = MouseMovedEvent{150.0f, 250.0f};

	EXPECT_NO_THROW({
		// Start rendering
		layerEventsDuringRender->Begin();

		// Handle events
		_appEventsDuringRender->OnEvent(keyEvent);
		_appEventsDuringRender->OnEvent(mouseEvent);

		// Continue rendering
		layerEventsDuringRender->OnRender();
		layerEventsDuringRender->End();
	});

	_appEventsDuringRender->PopLayer(layerEventsDuringRender);
	delete layerEventsDuringRender;
}
