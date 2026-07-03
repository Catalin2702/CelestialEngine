//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-06-20
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/Utility.hpp"

#include <AppKit/AppKit.hpp>

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

void CocoaWindow::Init() {
	_InitWindow();

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

	windowEventDispatcher = std::make_unique<NS::WindowEventDispatcher>();

	_window->setDelegate(windowEventDispatcher.get());
	_window->setMinSize(CGSizeMake(640, 360));
	_window->setOpaque(false);
	_window->setTitle(NS::String::string(windowProps.title.c_str(), NS::UTF8StringEncoding));

	// Restore the saved window position BEFORE creating the Metal view
	_window->setFrameAutosaveName(_window->title());

	_window->makeKeyAndOrderFront(nullptr);
}

void CocoaWindow::_Shutdown() {
	if (_window) {
		_window->close();
		_window = nullptr;
	}
}

}
