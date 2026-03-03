//
// Module: Tests/Engine/Modules/Core/Layers
// File: ImGuiOpenGlLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGuiOpenGlLayer.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>

#include <memory>
#include <gtest/gtest.h>

using namespace CE::Core;
using namespace CE::Events;
using namespace CE::Layers;
using namespace CE::Tools::Log;
using namespace CE::Types::Window;

/**
 * @brief Test fixture for ImGuiOpenGlLayer tests
 * @details These tests require an Application instance because ImGuiOpenGlLayer
 *			needs a valid OpenGL context and window to initialize properly
 */
class ImGuiOpenGlLayerTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();

		try {
			_app = new Application(WindowProps{"Test-ImGuiOpenGlLayer", 800, 600, false, GraphicsApi::OpenGL});
		}
		catch (...) {
			_windowAvailable = false;
			return;
		}

		_windowAvailable = true;
	}

	void TearDown() override {
		Log::Shutdown();
		delete _app;
	}

	bool _windowAvailable = false;
	Application* _app = nullptr;
};
//
/**
 * @brief Test ImGuiOpenGlLayer construction
 */
TEST_F(ImGuiOpenGlLayerTest, Construction) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	EXPECT_NE(layer, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layer->GetDebugName(), "ImGuiOpenGlLayer");
#endif

	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnAttach
 */
TEST_F(ImGuiOpenGlLayerTest, OnAttach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();

	EXPECT_NO_THROW({
		_app->PushLayer(layer);
	});

	// Clean up - pop the layer before the test ends
	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnDetach after OnAttach
 */
TEST_F(ImGuiOpenGlLayerTest, OnDetach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();

	// First attach the layer
	_app->PushLayer(layer);

	// Then detach it
	EXPECT_NO_THROW({
		_app->PopLayer(layer);
	});

	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnUpdate
 */
TEST_F(ImGuiOpenGlLayerTest, OnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer multiple OnUpdate calls
 */
TEST_F(ImGuiOpenGlLayerTest, MultipleOnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
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
 * @brief Test ImGuiOpenGlLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventKeyPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	KeyPressedEvent event(65, 0); // 'A' key
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventKeyReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	KeyReleasedEvent event(65); // 'A' key
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventKeyTyped) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	KeyTypedEvent event(65); // 'A' key
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventMouseButtonPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	MouseButtonPressedEvent event(0); // Left mouse button
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventMouseButtonReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	MouseButtonReleasedEvent event(0); // Left mouse button
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventMouseMoved) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	MouseMovedEvent event(100.0f, 200.0f);
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventMouseScrolled) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	MouseScrolledEvent event(0.0f, 1.0f); // Scroll up
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer OnEvent with WindowResize
 */
TEST_F(ImGuiOpenGlLayerTest, OnEventWindowResize) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	WindowResizeEvent event(1024, 768);
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer handling multiple events
 */
TEST_F(ImGuiOpenGlLayerTest, MultipleEvents) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();
	_app->PushLayer(layer);

	KeyPressedEvent keyEvent(65, 0);
	MouseMovedEvent mouseEvent(100.0f, 200.0f);
	WindowResizeEvent resizeEvent(1024, 768);

	EXPECT_NO_THROW({
		_app->OnEvent(keyEvent);
		_app->OnEvent(mouseEvent);
		_app->OnEvent(resizeEvent);
	});

	_app->PopLayer(layer);
	delete layer;
}

/**
 * @brief Test ImGuiOpenGlLayer full lifecycle
 */
TEST_F(ImGuiOpenGlLayerTest, FullLifecycle) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto layer = new ImGuiOpenGlLayer();

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
	KeyPressedEvent keyEvent(65, 0);
	MouseMovedEvent mouseEvent(100.0f, 200.0f);

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

