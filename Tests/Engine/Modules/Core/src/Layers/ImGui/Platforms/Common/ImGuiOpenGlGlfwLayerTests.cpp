//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Layers/ImGui/Platforms/Common
// File: ImGuiOpenGlGlfwLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-03
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGui/Platforms/Common/ImGuiOpenGlGlfwLayer.hpp>
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

constexpr WindowProps windowProps{"Test", 800, 600, false, GraphicsApi::OpenGL, WindowApi::GLFW};

/**
 * @brief Test fixture for ImGuiOpenGlGlfwLayer tests
 * @details These tests require an Application instance because ImGuiOpenGlGlfwLayer
 *			needs a valid OpenGL context and window to initialize properly
 */
class ImGuiOpenGlGlfwLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();

		try {
			_app = std::make_unique<Application>();
			_app->InitWindow(windowProps);
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
//
/**
 * @brief Test ImGuiOpenGlGlfwLayer construction
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, Construction) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	EXPECT_NE(layer, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layer->GetDebugName(), "ImGuiOpenGlGlfwLayer");
#endif

	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnAttach
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnAttach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();

	EXPECT_NO_THROW({
		_app->PushLayer(layer);
		_app->SetRenderLayer(layer);
	});

	// Clean up - pop the layer before the test ends
	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnDetach after OnAttach
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnDetach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();

	// First attach the layer
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	// Then detach it
	EXPECT_NO_THROW({
		_app->PopLayer(layer);
	});

	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnUpdate
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer multiple OnUpdate calls
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, MultipleOnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
		_app->Update();
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventKeyPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	KeyPressedEvent event{KeyboardKeyCode::A, 0}; // 'A' key
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventKeyReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	KeyReleasedEvent event{KeyboardKeyCode::A}; // 'A' key
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventKeyTyped) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	KeyTypedEvent event{KeyboardCharsCode::A};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventMouseButtonPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	MouseButtonPressedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventMouseButtonReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	MouseButtonReleasedEvent event{MouseButtonCode::Left};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventMouseMoved) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	MouseMovedEvent event{100.0f, 200.0f};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventMouseScrolled) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	MouseScrolledEvent event{0.0f, 1.0f}; // Scroll up
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer OnEvent with WindowResize
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, OnEventWindowResize) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

	WindowResizeEvent event{1024, 768};
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlGlfwLayer handling multiple events
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, MultipleEvents) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();
	_app->PushLayer(layer);
	_app->SetRenderLayer(layer);

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
 * @brief Test ImGuiOpenGlGlfwLayer full lifecycle
 */
TEST_F(ImGuiOpenGlGlfwLayerTest, FullLifecycle) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlGlfwLayer();

	// Attach
	EXPECT_NO_THROW({
		_app->PushLayer(layer);
		_app->SetRenderLayer(layer);
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

