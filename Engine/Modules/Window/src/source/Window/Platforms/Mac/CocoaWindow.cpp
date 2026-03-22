//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#include "Window/Platforms/Mac/CocoaWindow.hpp"

#include "AppKit/View/RenderView.hpp"
#include "AppKit/Window/WindowDelegate.hpp"

#include "Bridge/AppKit/Window/WindowBridge.h"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <stdexcept>


namespace CE::Window {

static bool _st_CocoaInitialized = false;
static int _st_CocoaWindowCount = 0;

CocoaWindow::CocoaWindow(const TypeWindow::WindowProps& windowProps):
	_data(windowProps), _window(nullptr) {
	_Init();
}

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

std::pair<float, float> CocoaWindow::GetContentScale() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetContentScale: Could not get content scale because window is not initialized.");
		return {1.0f, 1.0f};
	}
	const auto scale = _window->backingScaleFactor();
	return {scale, scale};
}

void CocoaWindow::SetEventCallback(const EventCallbackFn& callback) {
	_callbacks.EventCallback = callback;
}

void CocoaWindow::SetContentScaleCallback([[maybe_unused]] const ContentScaleCallbackFn& callback) {
	_callbacks.ContentScaleCallback = callback;
}

void CocoaWindow::_SetIOEventCallbacks() {
	if (not _view)
		return;

	RenderViewCallbacks callbacks{};

	callbacks.KeyPressedEventCallback = [](void* userData, const int keyCode, const bool isRepeat) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::KeyPressedEvent event{static_cast<KeyCode::KeyboardKeyCode>(keyCode), isRepeat ? 1 : 0};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.KeyReleasedEventCallback = [](void* userData, const int keyCode) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::KeyReleasedEvent event{static_cast<KeyCode::KeyboardKeyCode>(keyCode)};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.MouseMovedEventCallback = [](void* userData, const float x, const float y) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::MouseMovedEvent event{x, y};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.MouseScrolledEventCallback = [](void* userData, const float xOffset, const float yOffset) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::MouseScrolledEvent event{xOffset, yOffset};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.MouseButtonPressedEventCallback = [](void* userData, const int buttonCode) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::MouseButtonPressedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode)};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.MouseButtonReleasedEventCallback = [](void* userData, const int buttonCode) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::MouseButtonReleasedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode)};
			_callbacks->EventCallback(event);
		}
	};

	callbacks.MouseDraggedEventCallback = [](void* userData, const int buttonCode, const float x, const float y) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::MouseDraggedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode), x, y};
			_callbacks->EventCallback(event);
		}
	};

	_view->setCallbacks(callbacks, &_callbacks);
}

void CocoaWindow::_SetWindowEventCallbacks() {
	if (not _windowDelegate)
		return;

	WindowDelegateCallbacks callbacks{};

	callbacks.WindowDidResizeEventCallback = [](void* userData, const unsigned int width, const unsigned int height) {
		if (const auto _callbacks = static_cast<WindowCallbacks*>(userData)) {
			Events::WindowResizeEvent event{width, height};
			_callbacks->EventCallback(event);
			_callbacks->_internalCallbacks.ResizeEventCallback(event);
		}
	};
}

void CocoaWindow::_SetInternalCallbacks() {
	auto& [ResizeEventCallback] = _callbacks._internalCallbacks;

	ResizeEventCallback = [this](const Events::WindowResizeEvent& event) {
		SetSize(event.GetWidth(), event.GetHeight());
	};
}

void CocoaWindow::SetWidth(const unsigned int width) {
	if (not _window)
		return;

	_data.width = width;
	_UpdateLayerSize();
}

void CocoaWindow::SetHeight(const unsigned int height) {
	if (not _window)
		return;

	_data.height = height;
	_UpdateLayerSize();
}

void CocoaWindow::SetSize(unsigned int width, unsigned int height) {
	if (not _window)
		return;

	_data.width = width;
	_data.height = height;
	_UpdateLayerSize();
}

void CocoaWindow::SetVSync(const bool enabled) {
	_callbacks.VSyncCallback(enabled);
}

void CocoaWindow::_Init() {
	_InitWindow();

	SetVSync(_data.VSync);
	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();
	_SetInternalCallbacks();

	_st_CocoaWindowCount++;
}

void CocoaWindow::_InitWindow() {
	const auto app = NS::Application::sharedApplication();
	if (not _st_CocoaInitialized) {
		app->setActivationPolicy(NS::ActivationPolicyRegular);
		_st_CocoaInitialized = true;
	}

	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(_data.width), static_cast<CGFloat>(_data.height)}
	};

	NS::Window* rawWindow;
	try {
		rawWindow = NS::Window::alloc()->init(
			frame,
			NS::WindowStyleMaskTitled | NS::WindowStyleMaskClosable |
			NS::WindowStyleMaskResizable | NS::WindowStyleMaskMiniaturizable,
			NS::BackingStoreBuffered,
			false
		);
		if (not rawWindow) {
			CE_CORE_ERROR("CocoaWindow::_InitWindow: Could not create Cocoa window!");
			throw std::runtime_error("CocoaWindow::_InitWindow: Could not create Cocoa window!");
		}
	}
	catch (const std::exception& e) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Exception while creating Cocoa window: {0}", e.what());
		throw;
	}
	catch (...) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Unknown exception while creating Cocoa window");
		throw;
	}
	_window = NS::RetainPtr(rawWindow);

	NS::RenderView* rawCocoaView;
	try {
		rawCocoaView = NS::RenderView::alloc()->init(frame);
		if (not rawCocoaView) {
			CE_CORE_ERROR("CocoaWindow::_InitWindow: Could not create Cocoa view!");
			throw std::runtime_error("CocoaWindow::_InitWindow: Could not create Cocoa view!");
		}
	}
	catch (const std::exception& e) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Exception while creating Cocoa view: {0}", e.what());
		throw;
	}
	catch (...) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Unknown exception while creating Cocoa view");
		throw;
	}
	_view = NS::RetainPtr(rawCocoaView);

	NS::WindowDelegate* rawDelegate;
	try {
		rawDelegate = NS::WindowDelegate::alloc()->init();
		if (not rawDelegate) {
			CE_CORE_ERROR("CocoaWindow::_InitWindow: Could not create Cocoa window delegate!");
			throw std::runtime_error("CocoaWindow::_InitWindow: Could not create Cocoa window delegate!");
		}
	}
	catch (const std::exception& e) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Exception while creating Cocoa window delegate: {0}", e.what());
		throw;
	}
	catch (...) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Unknown exception while creating Cocoa window delegate");
		throw;
	}
	_windowDelegate = NS::RetainPtr(rawDelegate);

	_window->setTitle(NS::String::string(_data.title.c_str(), NS::UTF8StringEncoding));
	// Make the window visible
	_window->makeKeyAndOrderFront(nullptr);
	_window->setContentView(reinterpret_cast<NS::View*>(_view.get()));
	// Set frame autosave name to remember window position between launches
	Apple::Bridge::SetWindowFrameAutosaveName(_window.get(), _window->title()->utf8String());

	// Activate the application
	app->activateIgnoringOtherApps(true);
}

void CocoaWindow::_Shutdown() {
	if (_window) {
		_window->close();
		_window = nullptr;
	}

	_st_CocoaWindowCount--;

	if (_st_CocoaWindowCount <= 0 and _st_CocoaInitialized) {
		_st_CocoaInitialized = false;
	}
}

void CocoaWindow::_UpdateLayerSize() const {
	if (not _window)
		return;

	const auto scale = GetContentScale();
	_callbacks.ContentScaleCallback(scale);
}

}
