//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Mac
// File: ImGuiMetalGlfwLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGui/Platforms/Mac/ImGuiMetalGlfwLayer.hpp>
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
using namespace CE::Types::Window;


/**
 * @brief Test fixture for ImGuiMetalGlfwLayer tests
 * @details These tests require an Application instance because ImGuiMetalGlfwLayer
 *			needs a valid Metal context and window to initialize properly.
 *			Metal is only available on macOS.
 */
class ImGuiMetalGlfwLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();

		try {
			_app = std::make_unique<Application>(WindowProps{"Test-ImGuiMetalGlfwLayer", 800, 600, false, GraphicsApi::Metal, WindowApi::GLFW});
		}
		catch (...) {
			_windowAvailable = false;
			return;
		}

		_windowAvailable = true;
	}

	void TearDown() override {
		_app.reset();
		Log::Shutdown();
	}

	bool _windowAvailable = false;
	std::unique_ptr<Application> _app = nullptr;
};

/**
 * @brief Test ImGuiMetalGlfwLayer construction
 */
TEST_F(ImGuiMetalGlfwLayerTest, Construction) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	EXPECT_NE(layer, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layer->GetDebugName(), "ImGuiMetalGlfwLayer");
#endif

	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnAttach
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnAttach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	EXPECT_NO_THROW({
		_app->PushLayer(layer);
	});

	// Clean up - pop the layer before the test ends
	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnDetach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->PopLayer(layer);
	});

	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnUpdate
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalGlfwLayerTest, MultipleOnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
		_app->Update();
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventKeyPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	KeyPressedEvent event{KeyboardKeyCode::A, 0};
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventKeyReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	KeyReleasedEvent event{KeyboardKeyCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventKeyTyped) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventMouseButtonPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventMouseButtonReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventMouseMoved) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	MouseMovedEvent event{100.0f, 200.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventMouseScrolled) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	MouseScrolledEvent event{0.0f, 1.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalGlfwLayerTest, OnEventWindowResize) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	WindowResizeEvent event{1024, 768};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer handling multiple events
 */
TEST_F(ImGuiMetalGlfwLayerTest, MultipleEvents) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();
	_app->PushLayer(layer);

	KeyPressedEvent keyEvent{KeyboardKeyCode::A, 0};
	MouseMovedEvent mouseEvent{100.0f, 200.0f};
	WindowResizeEvent resizeEvent{1024, 768};

	EXPECT_NO_THROW({
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);
		_app->OnEvent(resizeEvent);
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiMetalGlfwLayer full lifecycle
 */
TEST_F(ImGuiMetalGlfwLayerTest, FullLifecycle) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiMetalGlfwLayer();

	// Attach
	EXPECT_NO_THROW({
		_app->PushLayer(layer);
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
		_app->PopLayer(layer);
	});

	delete layer;
}
