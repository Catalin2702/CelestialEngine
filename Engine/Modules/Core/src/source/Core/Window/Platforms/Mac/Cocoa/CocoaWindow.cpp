//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-06-16
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Apple/MetalCpp/AppKit/AppKit.hpp"
#include "Apple/MetalCpp/Foundation/Foundation.hpp"
#include "Apple/MetalCpp/MetalKit/MetalKit.hpp"
#include "Apple/Types/EventHandlers/ViewControllerEventHandler.hpp"
#include "Apple/Types/EventHandlers/ViewEventHandler.hpp"
#include "Apple/Types/EventHandlers/WindowDelegateEventHandler.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Utility/Utility.hpp"

#include <stdexcept>
#include <utility>


namespace CE::Core::Window {

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

std::pair<float, float> CocoaWindow::GetWindowSize() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetSize: Could not get size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _window->frame();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

std::pair<float, float> CocoaWindow::GetFrameSize() const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::GetFrameSize: Could not get frame size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _window->contentRectForFrameRect();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

bool CocoaWindow::IsVSync() const {
	return false;
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
	if (not _viewController and _viewController->view())
		return;

	_handlers.ViewEventHandler.OnKeyPressed([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::KeyPressedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(e->keyCode()), 0};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnKeyReleased([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::KeyReleasedEvent event{KeyCode::KeyboardKeyCodeFromCocoa(e->keyCode())};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnKeyTyped([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		if (const auto* characters = e->characters()) {
			if (const auto* utf8String = characters->utf8String(); utf8String and utf8String[0] != '\0') {
				Events::KeyTypedEvent event{KeyCode::KeyboardCharsCodeFromChar(utf8String[0])};
				_callbacks.EventCallback(event);
			}
		}
	});

	_handlers.ViewEventHandler.OnMouseMoved([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e and _viewController->view()))
			return;

		const auto [x, y] = _viewController->view()->convertPointFromView(e->locationInWindow(), nullptr);

		Events::MouseMovedEvent event{static_cast<float>(x), static_cast<float>(y)};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnMouseScrolled([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto xOffset = e->scrollingDeltaX();
		const auto yOffset = e->scrollingDeltaY();
		Events::MouseScrolledEvent event{static_cast<float>(xOffset), static_cast<float>(yOffset)};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnMouseButtonPressed([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::MouseButtonPressedEvent event{KeyCode::MouseButtonKeyCodeFromCocoa(e->buttonNumber())};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnMouseButtonReleased([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::MouseButtonReleasedEvent event{KeyCode::MouseButtonKeyCodeFromCocoa(e->buttonNumber())};
		_callbacks.EventCallback(event);
	});

	_handlers.ViewEventHandler.OnMouseDragged([this](const NS::Event* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto buttonCode = e->buttonNumber();
		const auto [x, y] = e->locationInWindow();
		Events::MouseDraggedEvent event{static_cast<KeyCode::MouseButtonCode>(buttonCode), static_cast<float>(x), static_cast<float>(y)};
		_callbacks.EventCallback(event);
	});

	_viewController->view()->SetEventHandler(&_handlers.ViewEventHandler);
}

void CocoaWindow::_SetWindowEventCallbacks() {
	if (not _windowDelegate)
		return;

	_handlers.WindowDelegateEventHandler.OnWindowDidResize([this](const NS::Notification* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		const auto* window = reinterpret_cast<NS::Window*>(e->object());
		const auto [origin, size] = window->frame();
		Events::WindowResizeEvent event{static_cast<unsigned int>(size.width), static_cast<unsigned int>(size.height)};
		_callbacks.EventCallback(event);
	});

	_handlers.WindowDelegateEventHandler.OnWindowWillClose([this](const NS::Notification* e) {
		if (not (_callbacks.EventCallback and e))
			return;

		Events::WindowCloseEvent event;
		_callbacks.EventCallback(event);
	});

	_handlers.WindowDelegateEventHandler.OnWindowDidChangeScreen([this](const NS::Notification* e) {
		if (not callbacks.WindowDidChangeCallback)
			return;

		const auto window = reinterpret_cast<NS::Window*>(e->object());
		const auto screen = window->screen();
		if (not screen) {
			CE_CORE_WARN("CocoaWindow::_SetWindowEventCallbacks: Window did change screen event received but could not get new screen from notification.");
			return;
		}
		callbacks.WindowDidChangeCallback(screen);
	});
	_handlers.WindowDelegateEventHandler.OnWindowDidChangeScreenProfile([this](const NS::Notification* e) {
		if (not callbacks.WindowDidChangeScreenProfileCallback)
			return;

		const auto window = reinterpret_cast<NS::Window*>(e->object());
		const auto screen = window->screen();
		if (not screen) {
			CE_CORE_WARN("CocoaWindow::_SetWindowEventCallbacks: Window change screen profile event received but could not get new screen from notification.");
			return;
		}
		callbacks.WindowDidChangeScreenProfileCallback(screen);
	});
	_handlers.WindowDelegateEventHandler.OnWindowDidChangeBackingProperties([this](const NS::Notification* e) {
		if (not callbacks.WindowDidChangeBackingPropertiesCallback)
			return;

		const auto window = reinterpret_cast<NS::Window*>(e->object());
		const auto screen = window->screen();
		if (not screen) {
			CE_CORE_WARN("CocoaWindow::_SetWindowEventCallbacks: Window change backing properties event received but could not get new screen from notification.");
			return;
		}
		callbacks.WindowDidChangeBackingPropertiesCallback(screen);
	});

	_windowDelegate->SetEventHandler(&_handlers.WindowDelegateEventHandler);
}

void CocoaWindow::_SetViewControllerEventCallbacks() {
	if (not _viewController)
		return;

	_handlers.ViewControllerEventHandler.OnViewDidLoad([this] {
		_SetIOEventCallbacks();
	});

	_viewController->SetEventHandler(&_handlers.ViewControllerEventHandler);
}

void CocoaWindow::SetSize(const unsigned int width, const unsigned int height) {
	if (not _window)
		return;

	const auto [currentOrigin, currentSize] = _window->frame();
	const CGRect frame = {
		currentOrigin,
		{static_cast<CGFloat>(width), static_cast<CGFloat>(height)}
	};
	_window->setFrame(frame, true, true);
}

void CocoaWindow::SetVSync(const bool enabled) {
	if (_callbacks.VSyncCallback) {
		_callbacks.VSyncCallback(enabled);
	}
}

void CocoaWindow::GetReady(const bool VSync) {
	SetVSync(VSync);
}

void CocoaWindow::Init(const MTL::Device* device) {
	_InitWindow();
	_InitViewController(device);

	_SetWindowEventCallbacks();
}

void CocoaWindow::_InitWindow() {
	const auto& windowProps = Utility::Config::Config::StGetWindowProps();
	const CGRect frame = {
		{static_cast<CGFloat>(0), static_cast<CGFloat>(0)},
		{static_cast<CGFloat>(windowProps.width), static_cast<CGFloat>(windowProps.height)}
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

	_window->setDelegate(_windowDelegate.get());
	_window->setMinSize(CGSizeMake(640, 360));
	_window->setOpaque(false);
	_window->setTitle(NS::String::string(windowProps.title.c_str(), NS::UTF8StringEncoding));
}

void CocoaWindow::_InitViewController(const MTL::Device* device) {
	NS::RenderViewController* rawViewController;
	try {
		rawViewController = NS::RenderViewController::alloc()->init(_window->contentRectForFrameRect(), device);
		if (not rawViewController) {
			CE_CORE_ERROR("CocoaWindow::InitViewController: Could not create RenderViewController.");
			throw std::runtime_error("CocoaWindow::InitViewController: Could not create RenderViewController.");
		}
	}
	catch (const std::exception& e) {
		CE_CORE_ERROR("CocoaWindow::InitViewController: Exception occurred while creating RenderViewController: {}", e.what());
		throw;
	}
	catch (...) {
		CE_CORE_ERROR("CocoaWindow::InitViewController: Unknown exception occurred while creating RenderViewController.");
		throw;
	}
	_viewController = NS::TransferPtr(rawViewController);
	_SetViewControllerEventCallbacks();

	// Restore the saved window position BEFORE creating the Metal view
	_window->setFrameAutosaveName(_window->title());

	_window->setContentViewController(_viewController.get());
	// Moves the window to the front of the screen list and makes it the key window.
	_window->makeKeyAndOrderFront(nullptr);
}

void CocoaWindow::_Shutdown() {
	if (_window) {
		_window->close();
		_window = nullptr;
	}
}

}
