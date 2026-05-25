//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalCpp/AppKit/AppKit.hpp"
#include "MetalCpp/Foundation/Foundation.hpp"
#include "MetalCpp/MetalKit/MetalKit.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/EventHandlers/ViewEventHandler.hpp"
#include "Types/EventHandlers/WindowDelegateEventHandler.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <stdexcept>
#include <utility>


namespace CE::Core::Window {

static int _st_CocoaWindowCount = 0;

CocoaWindow::CocoaWindow(TypeWindow::WindowProps windowProps):
	_data(std::move(windowProps)), _window(nullptr) {
	_Init();
}

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

void CocoaWindow::OnUpdate() const {}

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
	const auto [width, height] = GetViewSize();
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

	_viewEventHandler = std::make_unique<Apple::Types::ViewEventHandler>();

	_viewEventHandler->OnKeyPressed([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::KeyPressedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(e->keyCode()), 0};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnKeyReleased([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::KeyReleasedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(e->keyCode())};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnKeyTyped([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		if (const auto* characters = e->characters()) {
			if (const auto* utf8String = characters->utf8String(); utf8String and utf8String[0] != '\0') {
				Events::KeyTypedEvent event{KeyCode::KeyboardCharsCodeFromChar(static_cast<char>(utf8String[0]))};
				_callbacks.EventCallback(event);
			}
		}
	});

	_viewEventHandler->OnMouseMoved([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e and _view))
			return;

		const auto [x, y] = _view->convertPointFromView(e->locationInWindow(), nullptr);

		Events::MouseMovedEvent event{static_cast<float>(x), static_cast<float>(y)};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnMouseScrolled([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto xOffset = e->scrollingDeltaX();
		const auto yOffset = e->scrollingDeltaY();
		Events::MouseScrolledEvent event{static_cast<float>(xOffset), static_cast<float>(yOffset)};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnMouseButtonPressed([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::MouseButtonPressedEvent event{KeyCode::MouseButtonKeyCodeFromCocoa(e->buttonNumber())};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnMouseButtonReleased([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::MouseButtonReleasedEvent event{KeyCode::MouseButtonKeyCodeFromCocoa(e->buttonNumber())};
		_callbacks.EventCallback(event);
	});

	_viewEventHandler->OnMouseDragged([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto buttonCode = e->buttonNumber();
		const auto [x, y] = e->locationInWindow();
		Events::MouseDraggedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode), static_cast<float>(x), static_cast<float>(y)};
		_callbacks.EventCallback(event);
	});

	_view->SetEventHandler(_viewEventHandler.get());
}

void CocoaWindow::_SetWindowEventCallbacks() {
	if (not _windowDelegate)
		return;

	_windowDelegateEventHandler = std::make_unique<Apple::Types::WindowDelegateEventHandler>();

	_windowDelegateEventHandler->OnWindowDidResize([this](const NS::Notification* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto* window = reinterpret_cast<NS::Window*>(e->object());
		const auto [origin, size] = window->frame();
		Events::WindowResizeEvent event{static_cast<unsigned int>(size.width), static_cast<unsigned int>(size.height)};
		_callbacks.EventCallback(event);
		_callbacks._internalCallbacks.ResizeEventCallback(event);
	});

	_windowDelegateEventHandler->OnWindowWillClose([this](const NS::Notification* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::WindowCloseEvent event;
		_callbacks.EventCallback(event);
	});

	_windowDelegate->SetEventHandler(_windowDelegateEventHandler.get());
}

void CocoaWindow::_SetInternalCallbacks() {
	auto& [ResizeEventCallback] = _callbacks._internalCallbacks;

	// The OS already resized the window — just sync internal state and update the layer,
	// do NOT call setFrame again or the window will jump to (0, 0).
	ResizeEventCallback = [this](const Events::WindowResizeEvent& event) {
		_data.width = event.GetWidth();
		_data.height = event.GetHeight();
		_UpdateLayerSize();
	};
}

void CocoaWindow::SetWidth(const unsigned int width) {
	if (not _window)
		return;

	_data.width = width;

	const auto [currentOrigin, currentSize] = _window->frame();
	const CGRect frame = {
		currentOrigin,
		{static_cast<CGFloat>(_data.width), static_cast<CGFloat>(_data.height)}
	};
	_window->setFrame(frame, true, true);

	_UpdateLayerSize();
}

void CocoaWindow::SetHeight(const unsigned int height) {
	if (not _window)
		return;

	_data.height = height;

	const auto [currentOrigin, currentSize] = _window->frame();
	const CGRect frame = {
		currentOrigin,
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

	const auto [currentOrigin, currentSize] = _window->frame();
	const CGRect frame = {
		currentOrigin,
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
