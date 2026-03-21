//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#include "Window/Platforms/Mac/CocoaWindow.hpp"

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

/**
 * @brief Processes NSEvent and converts it to CE events
 * @details Intercepts and processes mouse and keyboard events from Cocoa
 */
// static void ProcessCocoaEvent(const NS::Event* event, const EventWindowData* data) {
// 	// ReSharper disable All
// 	if (!event or !data or !data->EventCallback)
// 		return;
// 	// ReSharper restore All
//
// 	switch (event->type()) {
// 		// Keyboard events
// 		case NS::EventTypeKeyDown: {
// 			const auto keyCode = KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode());
// 			const bool isRepeat = event->isARepeat();
// 			Events::KeyPressedEvent keyEvent{keyCode, isRepeat ? 1 : 0};
// 			data->EventCallback(keyEvent);
//
// 			// Also generate KeyTypedEvent for character input
// 			if (const auto* chars = event->characters()) {
// 				if (const char* str = chars->utf8String(); str and str[0]) {
// 					Events::KeyTypedEvent typedEvent{static_cast<KeyCode::KeyboardCharsCode>(static_cast<unsigned int>(str[0]))};
// 					data->EventCallback(typedEvent);
// 				}
// 			}
// 			break;
// 		}
//
// 		case NS::EventTypeKeyUp: {
// 			const auto keyCode = KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode());
// 			Events::KeyReleasedEvent keyEvent{keyCode};
// 			data->EventCallback(keyEvent);
// 			break;
// 		}
//
// 		// Mouse button events
// 		case NS::EventTypeLeftMouseDown:
// 		case NS::EventTypeRightMouseDown:
// 		case NS::EventTypeOtherMouseDown: {
// 			const auto button = KeyCode::MouseButtonKeyCodeFromCocoa(static_cast<int>(event->buttonNumber()));
// 			Events::MouseButtonPressedEvent mouseEvent{button};
// 			data->EventCallback(mouseEvent);
// 			break;
// 		}
//
// 		case NS::EventTypeLeftMouseUp:
// 		case NS::EventTypeRightMouseUp:
// 		case NS::EventTypeOtherMouseUp: {
// 			const auto button = KeyCode::MouseButtonKeyCodeFromCocoa(static_cast<int>(event->buttonNumber()));
// 			Events::MouseButtonReleasedEvent mouseEvent{button};
// 			data->EventCallback(mouseEvent);
// 			break;
// 		}
//
// 		// Mouse movement events
// 		case NS::EventTypeMouseMoved:
// 		case NS::EventTypeLeftMouseDragged:
// 		case NS::EventTypeRightMouseDragged:
// 		case NS::EventTypeOtherMouseDragged: {
// 			const auto [x, y] = event->locationInWindow();
// 			// Flip Y coordinate (Cocoa uses bottom-left origin, we use top-left)
// 			const auto flippedY = static_cast<float>(data->height - y);
// 			Events::MouseMovedEvent mouseEvent{static_cast<float>(x), flippedY};
// 			data->EventCallback(mouseEvent);
// 			break;
// 		}
//
// 		// Scroll wheel events
// 		case NS::EventTypeScrollWheel: {
// 			// Use precise scrolling deltas if available, otherwise use regular deltas
// 			float deltaX, deltaY;
// 			if (event->hasPreciseScrollingDeltas()) {
// 				deltaX = static_cast<float>(event->scrollingDeltaX());
// 				deltaY = static_cast<float>(event->scrollingDeltaY());
// 			} else {
// 				deltaX = static_cast<float>(event->deltaX());
// 				deltaY = static_cast<float>(event->deltaY());
// 			}
// 			Events::MouseScrolledEvent scrollEvent{deltaX, deltaY};
// 			data->EventCallback(scrollEvent);
// 			break;
// 		}
//
// 		default:
// 			// Other event types are not handled
// 			break;
// 	}
// }
//
// /**
//  * @brief Callback function for Cocoa window events
//  * @details This function is called by the Cocoa bridge when window events occur
//  */
// static void CocoaWindowEventCallback(void* userData, const int eventType, const unsigned int width, const unsigned int height) {
// 	auto* data = static_cast<EventWindowData*>(userData);
// 	if (!data or !data->EventCallback)
// 		return;
//
// 	switch (eventType) {
// 		case 0: // Resize
// 			data->width = width;
// 			data->height = height;
// 			{
// 				Events::WindowResizeEvent event{width, height};
// 				data->EventCallback(event);
// 			}
// 			break;
//
// 		case 1: // Close
// 			{
// 				Events::WindowCloseEvent event;
// 				data->EventCallback(event);
// 			}
// 			break;
//
// 		case 2: // Focus gained
// 		case 3: // Focus lost
// 			// Could emit WindowLostFocusEvent if implemented
// 			break;
//
// 		case 4: // Minimize
// 			data->width = 0;
// 			data->height = 0;
// 			{
// 				Events::WindowResizeEvent event{0, 0};
// 				data->EventCallback(event);
// 			}
// 			break;
//
// 		case 5: // Restore
// 			data->width = width;
// 			data->height = height;
// 			{
// 				Events::WindowResizeEvent event{width, height};
// 				data->EventCallback(event);
// 			}
// 			break;
//
// 		default:
// 			break;
// 	}
// }

CocoaWindow::CocoaWindow(const TypeWindow::WindowProps& windowProps):
	_data(windowProps), _window(nullptr) {
	_Init();
}

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

void CocoaWindow::OnUpdate() const {
	// Process all pending Cocoa events using the bridge
	// Apple::Bridge::ProcessCocoaEvents([](void* userData, void* eventPtr) {
	// 	if (!userData || !eventPtr)
	// 		return;
	//
	// 	const auto* data = static_cast<EventWindowData*>(userData);
	// 	const auto* event = static_cast<NS::Event*>(eventPtr);
	// 	ProcessCocoaEvent(event, data);
	// }, const_cast<EventWindowData*>(&_data));
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

void CocoaWindow::_SetWindowCallbacks() {
	if (!_window)
		return;


}

void CocoaWindow::_SetInternalCallbacks() {
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

void CocoaWindow::SetVSync([[maybe_unused]] const bool enabled) {
	// if (_layer) {
	// 	_data.VSync = enabled;
	// 	_layer->setDisplaySyncEnabled(enabled);
	// }
	// else {
	// 	CE_CORE_WARN("CocoaWindow::SetVSync: Could not set VSync because Metal layer is not initialized.");
	// }
}

void CocoaWindow::_Init() {
	_InitWindow();

	SetVSync(_data.VSync);
	_SetWindowCallbacks();
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

	NS::View* rawView;
	try {
		rawView = NS::View::alloc()->init(frame);
		if (not rawView) {
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
	_view = NS::RetainPtr(rawView);

	_window->setTitle(NS::String::string(_data.title.c_str(), NS::UTF8StringEncoding));
	// Make the window visible
	_window->makeKeyAndOrderFront(nullptr);
	_window->setContentView(_view.get());
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
	// _layer->setDrawableSize({
	// 	static_cast<float>(_data.width) * scale,
	// 	static_cast<float>(_data.height) * scale
	// });
}

}
