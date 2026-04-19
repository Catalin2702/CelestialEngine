//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#include "Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "AppKit/Utility/AppKit.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <stdexcept>
#include <utility>


namespace CE::Window {

static int _st_CocoaWindowCount = 0;

CocoaWindow::CocoaWindow(TypeWindow::WindowProps windowProps):
	_data(std::move(windowProps)), _window(nullptr) {
	_Init();
}

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

void CocoaWindow::OnUpdate() const {
	// const auto app = NS::Application::sharedApplication();
	// const auto mode = NS::String::string("kCFRunLoopDefaultMode", NS::UTF8StringEncoding);
	//
	// NS::Event* event;
	// while ((event = app->nextEventMatchingMask(NS::EventMaskAny,NS::Date::distantPast(), mode, true))) {
	// 	app->sendEvent(event);
	// }
}

float CocoaWindow::GetWindowWidth() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetWidth: Could not get width because window is not initialized.");
		return 0;
	}

	const auto [origin, size] = _window->frame();
	return static_cast<float>(size.width);
}

float CocoaWindow::GetWindowHeight() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetHeight: Could not get height because window is not initialized.");
		return 0;
	}

	const auto [origin, size] = _window->frame();
	return static_cast<float>(size.height);
}

std::pair<float, float> CocoaWindow::GetWindowSize() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetSize: Could not get size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _window->frame();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

float CocoaWindow::GetViewWidth() const {
	if (not _view) {
		CE_CORE_WARN("CocoaWindow::GetWidth: Could not get width because window is not initialized.");
		return 0;
	}

	const auto [origin, size] = _view->frame();
	return static_cast<float>(size.width);
}

float CocoaWindow::GetViewHeight() const {
	if (not _view) {
		CE_CORE_WARN("CocoaWindow::GetHeight: Could not get height because window is not initialized.");
		return 0;
	}

	const auto [origin, size] = _view->frame();
	return static_cast<float>(size.height);
}

std::pair<float, float> CocoaWindow::GetViewSize() const {
	if (not _view) {
		CE_CORE_WARN("CocoaWindow::GetSize: Could not get size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _view->frame();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

float CocoaWindow::GetMonitorWidth() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetMonitorWidth: Could not get monitor width because window is not initialized.");
		return 0;
	}

	return 0.f;
}

float CocoaWindow::GetMonitorHeight() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetMonitorHeight: Could not get monitor height because window is not initialized.");
		return 0;
	}

	return 0.f;
}

std::pair<float, float> CocoaWindow::GetMonitorSize() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetMonitorSize: Could not get monitor size because window is not initialized.");
		return {0, 0};
	}

	return {0.f, 0.f};
}

bool CocoaWindow::IsVSync() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::IsVSync: Could not get VSync state because window is not initialized.");
		return false;
	}

	return _data.VSync;
}

std::pair<float, float> CocoaWindow::GetContentScale() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetContentScale: Could not get content scale because window is not initialized.");
		return {1.0f, 1.0f};
	}

	const auto scale = _window->backingScaleFactor();
	return {scale, scale};
}

std::pair<float, float> CocoaWindow::GetContentSize() const {
	const auto [fst, snd] = GetContentScale();
	const auto [width, height] = GetWindowSize();
	return {width * fst, height * snd};
}

void CocoaWindow::SetEventCallback(const EventCallbackFn& callback) {
	_callbacks.EventCallback = callback;
}

void CocoaWindow::SetContentScaleCallback([[maybe_unused]] const ContentSizeCallbackFn& callback) {
	_callbacks.ContentSizeCallback = callback;
}

void CocoaWindow::SetVSyncCallback(const VSyncCallbackFn& callback) {
	_callbacks.VSyncCallback = callback;
}

void CocoaWindow::_SetIOEventCallbacks() {
	if (not _view)
		return;

	RenderViewCallbacks callbacks{};

	callbacks.KeyPressedEventCallback = [](void* userData, const int keyCode, const bool isRepeat) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::KeyPressedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(keyCode), isRepeat ? 1 : 0};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.KeyReleasedEventCallback = [](void* userData, const int keyCode) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::KeyReleasedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(keyCode)};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.KeyTypedEventCallback = [](void* userData, const char character) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::KeyTypedEvent event{KeyCode::KeyboardCharsCodeFromChar(character)};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.MouseMovedEventCallback = [](void* userData, const float x, const float y) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseMovedEvent event{x, y};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.MouseScrolledEventCallback = [](void* userData, const float xOffset, const float yOffset) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseScrolledEvent event{xOffset, yOffset};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.MouseButtonPressedEventCallback = [](void* userData, const int buttonCode) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseButtonPressedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode)};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.MouseButtonReleasedEventCallback = [](void* userData, const int buttonCode) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseButtonReleasedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode)};
			windowCallbacks->EventCallback(event);
		}
	};

	callbacks.MouseDraggedEventCallback = [](void* userData, const int buttonCode, const float x, const float y) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseDraggedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode), x, y};
			windowCallbacks->EventCallback(event);
		}
	};

	_view->setCallbacks(callbacks, &_callbacks);
}

void CocoaWindow::_SetWindowEventCallbacks() {
	if (not _windowDelegate)
		return;

	WindowDelegateCallbacks callbacks{};

	callbacks.WindowDidResizeEventCallback = [](void* userData, const unsigned int width, const unsigned int height) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::WindowResizeEvent event{width, height};
			windowCallbacks->EventCallback(event);
			windowCallbacks->_internalCallbacks.ResizeEventCallback(event);
		}
	};

	callbacks.WindowWillCloseEventCallback = [](void* userData) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(userData); windowCallbacks and windowCallbacks->EventCallback) {
			Events::WindowCloseEvent event;
			windowCallbacks->EventCallback(event);
		}
	};

	_windowDelegate->setCallbacks(callbacks, &_callbacks);
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

	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(_data.width), static_cast<CGFloat>(_data.height)}
	};
	_window->setFrame(frame, true, true);

	_UpdateLayerSize();
}

void CocoaWindow::SetHeight(const unsigned int height) {
	if (not _window)
		return;

	_data.height = height;

	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(_data.width), static_cast<CGFloat>(_data.height)}
	};
	_window->setFrame(frame, true, true);

	_UpdateLayerSize();
}

void CocoaWindow::SetSize(const unsigned int width, const unsigned int height) {
	if (not _window)
		return;

	_data.width = width;
	_data.height = height;

	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(_data.width), static_cast<CGFloat>(_data.height)}
	};
	_window->setFrame(frame, true, true);

	_UpdateLayerSize();
}

void CocoaWindow::SetVSync(const bool enabled) {
	if (_callbacks.VSyncCallback) {
		_callbacks.VSyncCallback(enabled);
	}
}

void CocoaWindow::GetReady() {
	SetVSync(_data.VSync);
	_UpdateLayerSize();
}

void CocoaWindow::_Init() {
	_InitWindow();

	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();
	_SetInternalCallbacks();

	_st_CocoaWindowCount++;
}

void CocoaWindow::_InitWindow() {
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
	_window = NS::TransferPtr(rawWindow);

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
	_view = NS::TransferPtr(rawCocoaView);

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
	_windowDelegate = NS::TransferPtr(rawDelegate);

	_window->setTitle(NS::String::string(_data.title.c_str(), NS::UTF8StringEncoding));
	// Make the window visible
	_window->makeKeyAndOrderFront(nullptr);
	_window->setContentView(reinterpret_cast<NS::View*>(_view.get()));

	_window->setDelegate(_windowDelegate.get());

	// Set frame autosave name to remember window position between launches
	_window->setFrameAutosaveName(_window->title());
}

void CocoaWindow::_Shutdown() {
	if (_window) {
		_window->close();
		_window = nullptr;
	}

	_st_CocoaWindowCount--;
}

void CocoaWindow::_UpdateLayerSize() const {
	if (not _window)
		return;

	if (_callbacks.ContentSizeCallback) {
		_callbacks.ContentSizeCallback(GetContentSize());
	}
}

}
