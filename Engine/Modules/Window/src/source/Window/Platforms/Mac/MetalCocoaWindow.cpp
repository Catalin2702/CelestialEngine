//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac
// File: MetalCocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#include "Window/Platforms/Mac/MetalCocoaWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/Cocoa/CocoaBridge.h"
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
static void ProcessCocoaEvent(const NS::Event* event, const EventWindowData* data) {
	// ReSharper disable All
	if (!event or !data or !data->EventCallback)
		return;

	switch (event->type()) {
		// Keyboard events
		case NS::EventTypeKeyDown: {
			const auto keyCode = KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode());
			const bool isRepeat = event->isARepeat();
			Events::KeyPressedEvent keyEvent{keyCode, isRepeat ? 1 : 0};
			data->EventCallback(keyEvent);

			// Also generate KeyTypedEvent for character input
			if (const auto* chars = event->characters()) {
				if (const char* str = chars->utf8String(); str and str[0]) {
					Events::KeyTypedEvent typedEvent{static_cast<KeyCode::KeyboardCharsCode>(static_cast<unsigned int>(str[0]))};
					data->EventCallback(typedEvent);
				}
			}
			break;
		}

		case NS::EventTypeKeyUp: {
			const auto keyCode = KeyCode::KeyboardKeyCodeFromCocoa(event->keyCode());
			Events::KeyReleasedEvent keyEvent{keyCode};
			data->EventCallback(keyEvent);
			break;
		}

		// Mouse button events
		case NS::EventTypeLeftMouseDown:
		case NS::EventTypeRightMouseDown:
		case NS::EventTypeOtherMouseDown: {
			const auto button = KeyCode::MouseButtonKeyCodeFromCocoa(event->buttonNumber());
			Events::MouseButtonPressedEvent mouseEvent{button};
			data->EventCallback(mouseEvent);
			break;
		}

		case NS::EventTypeLeftMouseUp:
		case NS::EventTypeRightMouseUp:
		case NS::EventTypeOtherMouseUp: {
			const auto button = KeyCode::MouseButtonKeyCodeFromCocoa(event->buttonNumber());
			Events::MouseButtonReleasedEvent mouseEvent{button};
			data->EventCallback(mouseEvent);
			break;
		}

		// Mouse movement events
		case NS::EventTypeMouseMoved:
		case NS::EventTypeLeftMouseDragged:
		case NS::EventTypeRightMouseDragged:
		case NS::EventTypeOtherMouseDragged: {
			const auto [x, y] = event->locationInWindow();
			// Flip Y coordinate (Cocoa uses bottom-left origin, we use top-left)
			const float flippedY = data->height - y;
			Events::MouseMovedEvent mouseEvent{static_cast<float>(x), flippedY};
			data->EventCallback(mouseEvent);
			break;
		}

		// Scroll wheel events
		case NS::EventTypeScrollWheel: {
			// Use precise scrolling deltas if available, otherwise use regular deltas
			float deltaX, deltaY;
			if (event->hasPreciseScrollingDeltas()) {
				deltaX = static_cast<float>(event->scrollingDeltaX());
				deltaY = static_cast<float>(event->scrollingDeltaY());
			} else {
				deltaX = static_cast<float>(event->deltaX());
				deltaY = static_cast<float>(event->deltaY());
			}
			Events::MouseScrolledEvent scrollEvent{deltaX, deltaY};
			data->EventCallback(scrollEvent);
			break;
		}

		default:
			// Other event types are not handled
			break;
	}
}

/**
 * @brief Callback function for Cocoa window events
 * @details This function is called by the Cocoa bridge when window events occur
 */
static void CocoaWindowEventCallback(void* userData, const int eventType, const unsigned int width, const unsigned int height) {
	auto* data = static_cast<EventWindowData*>(userData);
	if (!data or !data->EventCallback)
		return;

	switch (eventType) {
		case 0: // Resize
			data->width = width;
			data->height = height;
			{
				Events::WindowResizeEvent event{width, height};
				data->EventCallback(event);
			}
			break;

		case 1: // Close
			{
				Events::WindowCloseEvent event;
				data->EventCallback(event);
			}
			break;

		case 2: // Focus gained
			// Could emit WindowFocusEvent if implemented
			break;

		case 3: // Focus lost
			// Could emit WindowLostFocusEvent if implemented
			break;

		case 4: // Minimize
			data->width = 0;
			data->height = 0;
			{
				Events::WindowResizeEvent event{0, 0};
				data->EventCallback(event);
			}
			break;

		case 5: // Restore
			data->width = width;
			data->height = height;
			{
				Events::WindowResizeEvent event{width, height};
				data->EventCallback(event);
			}
			break;

		default:
			break;
	}
}

MetalCocoaWindow::MetalCocoaWindow(const TypeWindow::WindowProps& windowProps):
	_data(windowProps), _commandQueue(nullptr), _device(nullptr), _layer(nullptr), _window(nullptr), _windowDelegate(nullptr) {
	_Init();
}

MetalCocoaWindow::~MetalCocoaWindow() {
	_Shutdown();
}

void MetalCocoaWindow::OnUpdate() const {
	const auto pool = NS::AutoreleasePool::alloc()->init();

	// Process all pending Cocoa events using the bridge
	Apple::Bridge::ProcessCocoaEvents([](void* userData, void* eventPtr) {
		if (!userData || !eventPtr)
			return;

		const auto* data = static_cast<EventWindowData*>(userData);
		const auto* event = static_cast<NS::Event*>(eventPtr);
		ProcessCocoaEvent(event, data);
	}, const_cast<EventWindowData*>(&_data));

	pool->release();
}

void MetalCocoaWindow::SetEventCallback(const EventCallbackFn& callback) {
	_data.EventCallback = callback;
}

void MetalCocoaWindow::SetWindowCallbacks() {
	if (!_window)
		return;

	// Create delegate and set it for the window
	_windowDelegate = Apple::Bridge::CreateCocoaWindowDelegate(CocoaWindowEventCallback, &_data);
	if (_windowDelegate) {
		Apple::Bridge::SetCocoaWindowDelegate(_window, _windowDelegate);
	}
	else {
		CE_CORE_WARN("Failed to create Cocoa window delegate");
	}
}

void MetalCocoaWindow::SetWidth(const unsigned int width) {
	if (not (_window and _layer))
		return;

	_data.width = width;
	_UpdateLayerSize();
}

void MetalCocoaWindow::SetHeight(const unsigned int height) {
	if (not (_window and _layer))
		return;

	_data.height = height;
	_UpdateLayerSize();
}

void MetalCocoaWindow::SetVSync(const bool enabled) {
	if (_layer) {
		_data.VSync = enabled;
		_layer->setDisplaySyncEnabled(enabled);
	}
	else {
		CE_CORE_WARN("Could not set VSync because Metal layer is not initialized.");
	}
}

void MetalCocoaWindow::_Init() {
	_InitDevice();
	_InitWindow();

	SetVSync(_data.VSync);
	SetWindowCallbacks();

	_st_CocoaWindowCount++;
}

void MetalCocoaWindow::_InitDevice() {
	_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	if (not _device) {
		CE_CORE_ERROR("Could not create MetalDevice!");
		throw std::runtime_error("Could not create MetalDevice!");
	}

	_commandQueue = NS::TransferPtr(_device->newCommandQueue());
	if (not _commandQueue) {
		CE_CORE_ERROR("Could not create Metal Command Queue!");
		throw std::runtime_error("Could not create Metal Command Queue!");
	}
}

void MetalCocoaWindow::_InitWindow() {
	if (not _st_CocoaInitialized) {
		const auto app = NS::Application::sharedApplication();
		app->setActivationPolicy(NS::ActivationPolicyRegular);
		_st_CocoaInitialized = true;
	}

	const CGRect frame = {
		{0, 0},
		{static_cast<float>(_data.width), static_cast<float>(_data.height)}
	};

	_window = NS::Window::alloc()->init(
		frame,
		NS::WindowStyleMaskTitled | NS::WindowStyleMaskClosable |
		NS::WindowStyleMaskResizable | NS::WindowStyleMaskMiniaturizable,
		NS::BackingStoreBuffered,
		false
	);
	if (not _window) {
		CE_CORE_ERROR("Could not create Cocoa window!");
		throw std::runtime_error("Could not create Cocoa window!");
	}

	_window->setTitle(NS::String::string(_data.title.c_str(), NS::UTF8StringEncoding));

	// Set frame autosave name to remember window position between launches
	Apple::Bridge::SetWindowFrameAutosaveName(_window, "CelestialEngineMainWindow");

	_layer = NS::RetainPtr(CA::MetalLayer::layer());
	if (not _layer) {
		CE_CORE_ERROR("Could not create CAMetalLayer!");
		throw std::runtime_error("Could not create CAMetalLayer!");
	}

	_layer->setDevice(_device.get());
	_layer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);

	if (const auto contentView = _window->contentView()) {
		contentView->setLayer(_layer.get());
		contentView->setWantsLayer(true);
	}
	else {
		CE_CORE_ERROR("Failed to get content view from Metal window!");
		throw std::runtime_error("Failed to get content view from Metal window!");
	}

	_layer->setContentsScale(_window->backingScaleFactor());
	_layer->setMaximumDrawableCount(3);
	_layer->setAllowsNextDrawableTimeout(false);

	_UpdateLayerSize();

	// Make the window visible
	_window->makeKeyAndOrderFront(nullptr);

	// Activate the application
	const auto app = NS::Application::sharedApplication();
	app->activateIgnoringOtherApps(true);
}

void MetalCocoaWindow::_Shutdown() {
	// Cleanup delegate before closing window
	if (_windowDelegate) {
		Apple::Bridge::DestroyCocoaWindowDelegate(_windowDelegate);
		_windowDelegate = nullptr;
	}

	if (_window) {
		_window->close();
		_window = nullptr;
	}

	_st_CocoaWindowCount--;

	if (_st_CocoaWindowCount <= 0 and _st_CocoaInitialized) {
		_st_CocoaInitialized = false;
	}
}

void MetalCocoaWindow::_UpdateLayerSize() const {
	if (not (_window and _layer))
		return;

	const auto scale = _window->backingScaleFactor();
	_layer->setDrawableSize({
		static_cast<float>(_data.width) * scale,
		static_cast<float>(_data.height) * scale
	});
}

}
