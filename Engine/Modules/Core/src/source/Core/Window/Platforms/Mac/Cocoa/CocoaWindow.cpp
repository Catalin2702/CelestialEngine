//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Tools/Log/Log.hpp"
#include "Utility/Utility.hpp"

#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <objc/message.h>
#include <objc/runtime.h>

#include <stdexcept>
#include <utility>


namespace CE::Core {

namespace {

// metal-cpp exposes no NSUserDefaults wrapper, so the fullscreen flag is persisted through the Obj-C runtime directly. This
// mirrors what setFrameAutosaveName already does for the windowed frame, but for the native-fullscreen state (which AppKit only
// restores through the separate state-restoration machinery the engine does not opt into).

NS::String* FullScreenDefaultsKey(const NS::Window* window) {
	return window->title()->stringByAppendingString(NS::String::string(".fullscreen", NS::UTF8StringEncoding));
}

bool IsWindowFullScreen(const NS::Window* window) {
	return (window->styleMask() & NS::WindowStyleMaskFullScreen) != 0;
}

void SaveFullScreenFlag(NS::String* key, const bool value) {
	const auto defaults = reinterpret_cast<id (*)(Class, SEL)>(objc_msgSend)(objc_getClass("NSUserDefaults"), sel_registerName("standardUserDefaults"));
	reinterpret_cast<void (*)(id, SEL, BOOL, id)>(objc_msgSend)(defaults, sel_registerName("setBool:forKey:"), value, reinterpret_cast<id>(key));
	// The engine stops the run loop and returns from main instead of going through NSApplication's normal termination, so force
	// the write to disk now rather than relying on the periodic flush.
	reinterpret_cast<void (*)(id, SEL)>(objc_msgSend)(defaults, sel_registerName("synchronize"));
}

bool LoadFullScreenFlag(NS::String* key) {
	const auto defaults = reinterpret_cast<id (*)(Class, SEL)>(objc_msgSend)(objc_getClass("NSUserDefaults"), sel_registerName("standardUserDefaults"));
	return reinterpret_cast<BOOL (*)(id, SEL, id)>(objc_msgSend)(defaults, sel_registerName("boolForKey:"), reinterpret_cast<id>(key));
}

}

void CocoaWindowEventHandler::DispatchCocoaWindowCreated() const {
	cocoaWindowStateEvents.cocoaWindowCreatedDispatcher.Dispatch();
}

void CocoaWindowEventHandler::DispatchCocoaWindowInitialized() const {
	cocoaWindowStateEvents.cocoaWindowInitializedDispatcher.Dispatch();
}

void CocoaWindowEventHandler::DispatchCocoaWindowWillShutdown() const {
	cocoaWindowStateEvents.cocoaWindowWillShutdownDispatcher.Dispatch();
}

CocoaWindow::CocoaWindow() {
	cocoaWindowEventDispatcher.cocoaWindowStateEvents.cocoaWindowCreatedDispatcher.Dispatch();
}

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

std::pair<float, float> CocoaWindow::GetWindowSize() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::GetSize: Could not get size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _window->frame();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

std::pair<float, float> CocoaWindow::GetFrameSize() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::GetFrameSize: Could not get frame size because window is not initialized.");
		return {0, 0};
	}

	const auto [origin, size] = _window->contentRectForFrameRect();
	return {static_cast<float>(size.width), static_cast<float>(size.height)};
}

void CocoaWindow::SetWindowSize(const unsigned int width, const unsigned int height) {
	if (not _window) [[unlikely]]
		return;

	const auto [currentOrigin, currentSize] = _window->frame();
	const CGRect frame = {
		.origin = currentOrigin,
		.size = {.width = static_cast<CGFloat>(width), .height = static_cast<CGFloat>(height)}
	};
	_window->setFrame(frame, true, true);
}

void CocoaWindow::SetContentView(const MTK::View* view) const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::SetContentView: Cannot set content view because window is not initialized.");
		return;
	}

	if (not view) [[unlikely]] {
		CE_CORE_ERROR("CocoaWindow::SetContentView: Cannot set a null content view!");
		throw std::runtime_error("CocoaWindow::SetContentView: Cannot set a null content view!");
	}

	_window->setContentView(view);
	// The view must be the first responder to receive key events, and the window must accept mouse-moved events for the
	// view's dispatcher to get continuous cursor updates (there is no tracking area otherwise).
	(void)_window->makeFirstResponder(view);
	_window->setAcceptsMouseMovedEvents(true);
}

void CocoaWindow::Show() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::Show: Cannot show window because it is not initialized.");
		return;
	}

	_window->makeKeyAndOrderFront(nullptr);

	// Restore the native fullscreen state saved on the previous shutdown (the windowed frame itself is restored by the frame
	// autosave). Done after the window is on screen so toggleFullScreen animates into its own Space.
	if (LoadFullScreenFlag(FullScreenDefaultsKey(_window.get())) and not IsWindowFullScreen(_window.get()))
		_window->toggleFullScreen(nullptr);
}

void CocoaWindow::Init() {
	_InitWindow();
	cocoaWindowEventDispatcher.cocoaWindowStateEvents.cocoaWindowInitializedDispatcher.Dispatch();
}

void CocoaWindow::Miniaturize() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::Miniaturize: Cannot miniaturize because window is not initialized");
		return;
	}

	_window->miniaturize(nullptr);
}

void CocoaWindow::Deminiaturize() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::Deminiaturize: Cannot deminiaturize because window is not initialized");
		return;
	}

	_window->deminiaturize(nullptr);
}

void CocoaWindow::ToggleFullScreen() const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::ToggleFullScreen: Cannot toggle fullscreen because window is not initialized");
		return;
	}

	_window->toggleFullScreen(nullptr);
}

void CocoaWindow::_InitWindow() {
	const auto& windowProps = Utility::Config::StGetWindowProps();
	const CGRect frame = {
		.origin = {.x = static_cast<CGFloat>(0), .y = static_cast<CGFloat>(0)},
		.size = {.width = static_cast<CGFloat>(windowProps.width), .height = static_cast<CGFloat>(windowProps.height)}
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
		if (not rawWindow) [[unlikely]] {
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

	_window->setDelegate(&cocoaWindowEventDispatcher);
	_window->setMinSize(CGSizeMake(640, 360));
	_window->setOpaque(false);
	_window->setTitle(NS::String::string(windowProps.title.c_str(), NS::UTF8StringEncoding));

	// Restore the saved window position BEFORE creating the Metal view
	_window->setFrameAutosaveName(_window->title());
}

void CocoaWindow::_Shutdown() {
	cocoaWindowEventDispatcher.cocoaWindowStateEvents.cocoaWindowWillShutdownDispatcher.Dispatch();
	if (_window) [[unlikely]] {
		// Persist the native fullscreen state so Show() can restore it on the next launch (mirrors the frame autosave).
		SaveFullScreenFlag(FullScreenDefaultsKey(_window.get()), IsWindowFullScreen(_window.get()));

		_window->close();
		_window = nullptr;
	}
}

}
