//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-18
// Updated by: Catalin Chirosca
// Updated: 2026-03-20
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp>
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
 * @brief Test fixture for ImGuiMetalLayer tests
 * @details These tests require an Application instance because ImGuiMetalLayer
 *			needs a valid Metal context and window to initialize properly.
 *			Metal is only available on macOS.
 */
class ImGuiMetalLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}

	void TearDown() override {
		Log::Shutdown();
	}
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
	std::unique_ptr<Application> _appOnAttach;
	try {
		_appOnAttach = std::make_unique<Application>();
		_appOnAttach->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnAttach = new ImGuiMetalLayer();
	EXPECT_NO_THROW({
		_appOnAttach->PushLayer(layerOnAttach);
		_appOnAttach->SetRenderLayer(layerOnAttach);
	});

	// Clean up - pop the layer before the test ends
	_appOnAttach->PopLayer(layerOnAttach);
	delete layerOnAttach;
}

/**
 * @brief Test ImGuiMetalLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnDetach) {
	std::unique_ptr<Application> _appOnDetach;
	try {
		_appOnDetach = std::make_unique<Application>();
		_appOnDetach->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnDetach = new ImGuiMetalLayer();
	_appOnDetach->PushLayer(layerOnDetach);
	_appOnDetach->SetRenderLayer(layerOnDetach);

	EXPECT_NO_THROW({
		_appOnDetach->PopLayer(layerOnDetach);
	});

	delete layerOnDetach;
}

/**
 * @brief Test ImGuiMetalLayer OnUpdate
 */
TEST_F(ImGuiMetalLayerTest, OnUpdate) {
	std::unique_ptr<Application> _appOnUpdate;
	try {
		_appOnUpdate = std::make_unique<Application>();
		_appOnUpdate->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnUpdate = new ImGuiMetalLayer();
	_appOnUpdate->PushLayer(layerOnUpdate);
	_appOnUpdate->SetRenderLayer(layerOnUpdate);

	EXPECT_NO_THROW({
		_appOnUpdate->Update();
	});

	_appOnUpdate->PopLayer(layerOnUpdate);
	delete layerOnUpdate;
}

/**
 * @brief Test ImGuiMetalLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalLayerTest, MultipleOnUpdate) {
	std::unique_ptr<Application> _appMultipleOnUpdate;
	try {
		_appMultipleOnUpdate = std::make_unique<Application>();
		_appMultipleOnUpdate->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleOnUpdate = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyPressed) {
	std::unique_ptr<Application> _appOnEventKeyPressed;
	try {
		_appOnEventKeyPressed = std::make_unique<Application>();
		_appOnEventKeyPressed->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyPressed = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyReleased) {
	std::unique_ptr<Application> _appOnEventKeyReleased;
	try {
		_appOnEventKeyReleased = std::make_unique<Application>();
		_appOnEventKeyReleased->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyReleased = new ImGuiMetalLayer();
	_appOnEventKeyReleased->PushLayer(layerOnEventKeyReleased);
	_appOnEventKeyReleased->SetRenderLayer(layerOnEventKeyReleased);

	KeyReleasedEvent event{KeyboardKeyCode::A};
	_appOnEventKeyReleased->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventKeyReleased->PopLayer(layerOnEventKeyReleased);
	delete layerOnEventKeyReleased;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyTyped) {
	std::unique_ptr<Application> _appOnEventKeyTyped;
	try {
		_appOnEventKeyTyped = std::make_unique<Application>();
		_appOnEventKeyTyped->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventKeyTyped = new ImGuiMetalLayer();
	_appOnEventKeyTyped->PushLayer(layerOnEventKeyTyped);
	_appOnEventKeyTyped->SetRenderLayer(layerOnEventKeyTyped);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_appOnEventKeyTyped->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventKeyTyped->PopLayer(layerOnEventKeyTyped);
	delete layerOnEventKeyTyped;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonPressed) {
	std::unique_ptr<Application> _appOnEventMouseButtonPressed;
	try {
		_appOnEventMouseButtonPressed = std::make_unique<Application>();
		_appOnEventMouseButtonPressed->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseButtonPressed = new ImGuiMetalLayer();
	_appOnEventMouseButtonPressed->PushLayer(layerOnEventMouseButtonPressed);
	_appOnEventMouseButtonPressed->SetRenderLayer(layerOnEventMouseButtonPressed);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_appOnEventMouseButtonPressed->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseButtonPressed->PopLayer(layerOnEventMouseButtonPressed);
	delete layerOnEventMouseButtonPressed;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonReleased) {
	std::unique_ptr<Application> _appOnEventMouseButtonReleased;
	try {
		_appOnEventMouseButtonReleased = std::make_unique<Application>();
		_appOnEventMouseButtonReleased->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseButtonReleased = new ImGuiMetalLayer();
	_appOnEventMouseButtonReleased->PushLayer(layerOnEventMouseButtonReleased);
	_appOnEventMouseButtonReleased->SetRenderLayer(layerOnEventMouseButtonReleased);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_appOnEventMouseButtonReleased->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseButtonReleased->PopLayer(layerOnEventMouseButtonReleased);
	delete layerOnEventMouseButtonReleased;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseMoved) {
	std::unique_ptr<Application> _appOnEventMouseMoved;
	try {
		_appOnEventMouseMoved = std::make_unique<Application>();
		_appOnEventMouseMoved->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseMoved = new ImGuiMetalLayer();
	_appOnEventMouseMoved->PushLayer(layerOnEventMouseMoved);
	_appOnEventMouseMoved->SetRenderLayer(layerOnEventMouseMoved);

	MouseMovedEvent event{100.0f, 200.0f};
	_appOnEventMouseMoved->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseMoved->PopLayer(layerOnEventMouseMoved);
	delete layerOnEventMouseMoved;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseScrolled) {
	std::unique_ptr<Application> _appOnEventMouseScrolled;
	try {
		_appOnEventMouseScrolled = std::make_unique<Application>();
		_appOnEventMouseScrolled->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventMouseScrolled = new ImGuiMetalLayer();
	_appOnEventMouseScrolled->PushLayer(layerOnEventMouseScrolled);
	_appOnEventMouseScrolled->SetRenderLayer(layerOnEventMouseScrolled);


	MouseScrolledEvent event{0.0f, 1.0f};
	_appOnEventMouseScrolled->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventMouseScrolled->PopLayer(layerOnEventMouseScrolled);
	delete layerOnEventMouseScrolled;
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalLayerTest, OnEventWindowResize) {
	std::unique_ptr<Application> _appOnEventWindowResize;
	try {
		_appOnEventWindowResize = std::make_unique<Application>();
		_appOnEventWindowResize->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnEventWindowResize = new ImGuiMetalLayer();
	_appOnEventWindowResize->PushLayer(layerOnEventWindowResize);
	_appOnEventWindowResize->SetRenderLayer(layerOnEventWindowResize);

	WindowResizeEvent event{1024, 768};
	_appOnEventWindowResize->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_appOnEventWindowResize->PopLayer(layerOnEventWindowResize);
	delete layerOnEventWindowResize;
}

/**
 * @brief Test ImGuiMetalLayer handling multiple events
 */
TEST_F(ImGuiMetalLayerTest, MultipleEvents) {
	std::unique_ptr<Application> _appMultipleEvents;
	try {
		_appMultipleEvents = std::make_unique<Application>();
		_appMultipleEvents->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleEvents = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer full lifecycle
 */
TEST_F(ImGuiMetalLayerTest, FullLifecycle) {
	std::unique_ptr<Application> _appFullLifecycle;
	try {
		_appFullLifecycle = std::make_unique<Application>();
		_appFullLifecycle->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerFullLifecycle = new ImGuiMetalLayer();

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
 * @brief Test ImGuiMetalLayer Begin and End methods
 */
TEST_F(ImGuiMetalLayerTest, BeginEnd) {
	std::unique_ptr<Application> _appBeginEnd;
	try {
		_appBeginEnd = std::make_unique<Application>();
		_appBeginEnd->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerBeginEnd = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer multiple Begin/End cycles
 */
TEST_F(ImGuiMetalLayerTest, MultipleBeginEnd) {
	std::unique_ptr<Application> _appMultipleBeginEnd;
	try {
		_appMultipleBeginEnd = std::make_unique<Application>();
		_appMultipleBeginEnd->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerMultipleBeginEnd = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer OnRender
 */
TEST_F(ImGuiMetalLayerTest, OnRender) {
	std::unique_ptr<Application> _appOnRender;
	try {
		_appOnRender = std::make_unique<Application>();
		_appOnRender->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerOnRender = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer complete render cycle
 */
TEST_F(ImGuiMetalLayerTest, CompleteRenderCycle) {
	std::unique_ptr<Application> _appCompleteRenderCycle;
	try {
		_appCompleteRenderCycle = std::make_unique<Application>();
		_appCompleteRenderCycle->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerCompleteRenderCycle = new ImGuiMetalLayer();
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
 * @brief Test ImGuiMetalLayer with events during render
 */
TEST_F(ImGuiMetalLayerTest, EventsDuringRender) {
	std::unique_ptr<Application> _appEventsDuringRender;
	try {
		_appEventsDuringRender = std::make_unique<Application>();
		_appEventsDuringRender->InitWindow(windowProps);
	}
	catch (...) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layerEventsDuringRender = new ImGuiMetalLayer();
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
