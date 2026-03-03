//
// Module: Tests/Engine/Modules/Core/Layers
// File: ImGuiMetalLayerTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Core/Application.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Layers/ImGuiMetalLayer.hpp>
#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>

#include <print>
#include <memory>
#include <chrono>
#include <gtest/gtest.h>

using namespace CE::Core;
using namespace CE::Events;
using namespace CE::Layers;
using namespace CE::Tools::Log;
using namespace CE::Types::Window;


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

		_app = new Application(WindowProps{"Test-ImGuiMetalLayer", 800, 600, false, GraphicsApi::Metal});

		_windowAvailable = true;
	}

	void TearDown() override {
		Log::Shutdown();
		delete _app;
	}

	bool _windowAvailable = false;
	Application* _app = nullptr;
};

/**
 * @brief Test ImGuiMetalLayer construction
 */
TEST_F(ImGuiMetalLayerTest, Construction) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}
	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	EXPECT_NE(layer, nullptr);

#ifdef CE_DEBUG
	EXPECT_EQ(layer->GetDebugName(), "ImGuiMetalLayer");
#endif

	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnAttach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();

	EXPECT_NO_THROW({
		_app->PushLayer(layer);
	});

	// Clean up - pop the layer before the test ends
	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnDetach after OnAttach
 */
TEST_F(ImGuiMetalLayerTest, OnDetach) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();

	// First attach the layer
	_app->PushLayer(layer);

	// Then detach it
	EXPECT_NO_THROW({
		_app->PopLayer(layer);
	});

	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnUpdate
 */
TEST_F(ImGuiMetalLayerTest, OnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer multiple OnUpdate calls
 */
TEST_F(ImGuiMetalLayerTest, MultipleOnUpdate) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	EXPECT_NO_THROW({
		_app->Update();
		_app->Update();
		_app->Update();
	});

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	KeyPressedEvent event(65, 0); // 'A' key
	_app->OnEvent(event);

	// ImGui layers typically don't block event propagation
	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	KeyReleasedEvent event(65); // 'A' key
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with KeyTyped
 */
TEST_F(ImGuiMetalLayerTest, OnEventKeyTyped) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	KeyTypedEvent event(65); // 'A' key
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonPressed
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonPressed) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	MouseButtonPressedEvent event(0); // Left mouse button
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseButtonReleased
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseButtonReleased) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	MouseButtonReleasedEvent event(0); // Left mouse button
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseMoved
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseMoved) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	MouseMovedEvent event(100.0f, 200.0f);
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with MouseScrolled
 */
TEST_F(ImGuiMetalLayerTest, OnEventMouseScrolled) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	MouseScrolledEvent event(0.0f, 1.0f); // Scroll up
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer OnEvent with WindowResize
 */
TEST_F(ImGuiMetalLayerTest, OnEventWindowResize) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
	_app->PushLayer(layer);

	WindowResizeEvent event(1024, 768);
	_app->OnEvent(event);

	EXPECT_FALSE(event.IsHandled());

	_app->PopLayer(layer);
	delete layer;

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer handling multiple events
 */
TEST_F(ImGuiMetalLayerTest, MultipleEvents) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();
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

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}

/**
 * @brief Test ImGuiMetalLayer full lifecycle
 */
TEST_F(ImGuiMetalLayerTest, FullLifecycle) {
	if (!_windowAvailable) {
		GTEST_SKIP() << "Window not available (no display)";
	}

	const auto start = std::chrono::high_resolution_clock::now();

	const auto layer = new ImGuiMetalLayer();

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

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::print("[          ] Time: {} us\n", duration);
}
