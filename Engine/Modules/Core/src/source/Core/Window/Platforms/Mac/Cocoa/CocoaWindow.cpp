//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Utility.hpp"

#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <stdexcept>
#include <utility>


namespace CE::Core::Window {

void CocoaWindowEventHandler::DispatchCocoaWindowCreated() {
	windowStateEvents.cocoaWindowCreatedMulticastDispatcher.Dispatch();
}

void CocoaWindowEventHandler::DispatchCocoaWindowInitialized() {
	windowStateEvents.cocoaWindowInitializedMulticastDispatcher.Dispatch();
}

void CocoaWindowEventHandler::DispatchCocoaWindowWillShutdown() {
	windowStateEvents.cocoaWindowWillShutdownMulticastDispatcher.Dispatch();
}

CocoaWindow::CocoaWindow() {
	cocoaWindowEventDispatcher = std::make_unique<CocoaWindowEventHandler>();
	cocoaWindowEventDispatcher->windowStateEvents.cocoaWindowCreatedMulticastDispatcher.Dispatch();
}

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

void CocoaWindow::SetContentView(const MTK::View* view) const {
	if (not _window) {
		CE_CORE_WARN("CocoaWindow::SetContentView: Cannot set content view because window is not initialized.");
		return;
	}

	if (not view) {
		CE_CORE_ERROR("CocoaWindow::SetContentView: Cannot set a null content view!");
		throw std::runtime_error("CocoaWindow::SetContentView: Cannot set a null content view!");
	}

	_window->setContentView(view);
	// The view must be the first responder to receive key events, and the window must accept mouse-moved events for the
	// view's dispatcher to get continuous cursor updates (there is no tracking area otherwise).
	(void)_window->makeFirstResponder(view);
	_window->setAcceptsMouseMovedEvents(true);
}

void CocoaWindow::Init() {
	_InitWindow();
	cocoaWindowEventDispatcher->windowStateEvents.cocoaWindowInitializedMulticastDispatcher.Dispatch();
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

	_window->setDelegate(cocoaWindowEventDispatcher.get());
	_window->setMinSize(CGSizeMake(640, 360));
	_window->setOpaque(false);
	_window->setTitle(NS::String::string(windowProps.title.c_str(), NS::UTF8StringEncoding));

	// Restore the saved window position BEFORE creating the Metal view
	_window->setFrameAutosaveName(_window->title());

	_window->makeKeyAndOrderFront(nullptr);
}

void CocoaWindow::_Shutdown() {
	cocoaWindowEventDispatcher->windowStateEvents.cocoaWindowWillShutdownMulticastDispatcher.Dispatch();
	if (_window) {
		_window->close();
		_window = nullptr;
	}
}

}
