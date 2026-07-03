//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#include "Apple/MetalCpp/AppKit/WindowDelegate.hpp"
#include "Tools/Log/Log.hpp"


static void CallListeners(const std::vector<const NotificationListener*>& vec, NS::Notification* notification) {
	for (const auto listener: vec) {
		if (listener)
			(*listener)(notification);
	}
}

static void ExtendListeners(std::vector<const NotificationListener*>& vec, const std::vector<const NotificationListener*>& source) {
	vec.reserve(vec.size() + source.size());
	vec.insert(vec.end(), source.begin(), source.end());
}

static void RemoveItem(std::vector<const NotificationListener*>& vec, const NotificationListener* item) {
	std::erase_if(vec, [&](const auto l) {
		return l == nullptr or l == item;
	});
}

static void RemoveIndex(std::vector<const NotificationListener*>& vec, const size_t index, const char* functionCalled) {
	if (index >= vec.size()) {
		const auto message = std::string(functionCalled) + ": Index " + std::to_string(index) + " Vector size: " + std::to_string(vec.size());
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}
	vec.erase(vec.begin() + static_cast<long>(index));
}

namespace NS {

void WindowDelegate::windowDidResize(Notification* notification) {
	CallListeners(windowDidResizeListeners, notification);
}

void WindowDelegate::windowDidMove(Notification* notification) {
	CallListeners(windowDidMoveListeners, notification);
}

void WindowDelegate::windowDidBecomeKey(Notification* notification) {
	CallListeners(windowDidBecomeKeyListeners, notification);
}

void WindowDelegate::windowDidResignKey(Notification* notification) {
	CallListeners(windowDidResignKeyListeners, notification);
}

void WindowDelegate::windowDidMiniaturize(Notification* notification) {
	CallListeners(windowDidMiniaturizeListeners, notification);
}

void WindowDelegate::windowDidDeminiaturize(Notification* notification) {
	CallListeners(windowDidDeminiaturizeListeners, notification);
}

void WindowDelegate::windowDidEnterFullScreen(Notification* notification) {
	CallListeners(windowDidEnterFullScreenListeners, notification);
}

void WindowDelegate::windowDidExitFullScreen(Notification* notification) {
	CallListeners(windowDidExitFullScreenListeners, notification);
}

void WindowDelegate::windowWillClose(Notification* notification) {
	CallListeners(windowWillCloseListeners, notification);
}

void WindowDelegate::AddWindowDidResizeListener(const NotificationListener* listener) {
	windowDidResizeListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidMoveListener(const NotificationListener* listener) {
	windowDidMoveListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidBecomeKeyListener(const NotificationListener* listener) {
	windowDidBecomeKeyListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidResignKeyListener(const NotificationListener* listener) {
	windowDidResignKeyListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidMiniaturizeListener(const NotificationListener* listener) {
	windowDidMiniaturizeListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidDeminiaturizeListener(const NotificationListener* listener) {
	windowDidDeminiaturizeListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidEnterFullScreenListener(const NotificationListener* listener) {
	windowDidEnterFullScreenListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidExitFullScreenListener(const NotificationListener* listener) {
	windowDidExitFullScreenListeners.push_back(listener);
}

void WindowDelegate::AddWindowWillCloseListener(const NotificationListener* listener) {
	windowWillCloseListeners.push_back(listener);
}

void WindowDelegate::AddWindowDidResizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidResizeListeners, listeners);
}

void WindowDelegate::AddWindowDidMoveListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidMoveListeners, listeners);
}

void WindowDelegate::AddWindowDidBecomeKeyListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidBecomeKeyListeners, listeners);
}

void WindowDelegate::AddWindowDidResignKeyListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidResignKeyListeners, listeners);
}

void WindowDelegate::AddWindowDidMiniaturizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidMiniaturizeListeners, listeners);
}

void WindowDelegate::AddWindowDidDeminiaturizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidDeminiaturizeListeners, listeners);
}

void WindowDelegate::AddWindowDidEnterFullScreenListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidEnterFullScreenListeners, listeners);
}

void WindowDelegate::AddWindowDidExitFullScreenListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidExitFullScreenListeners, listeners);
}

void WindowDelegate::AddWindowWillCloseListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowWillCloseListeners, listeners);
}

void WindowDelegate::RemoveWindowDidResizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidResizeListeners, listener);
}

void WindowDelegate::RemoveWindowDidMoveListener(const NotificationListener* listener) {
	RemoveItem(windowDidMoveListeners, listener);
}

void WindowDelegate::RemoveWindowDidBecomeKeyListener(const NotificationListener* listener) {
	RemoveItem(windowDidBecomeKeyListeners, listener);
}

void WindowDelegate::RemoveWindowDidResignKeyListener(const NotificationListener* listener) {
	RemoveItem(windowDidResignKeyListeners, listener);
}

void WindowDelegate::RemoveWindowDidMiniaturizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidMiniaturizeListeners, listener);
}

void WindowDelegate::RemoveWindowDidDeminiaturizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidDeminiaturizeListeners, listener);
}

void WindowDelegate::RemoveWindowDidEnterFullScreenListener(const NotificationListener* listener) {
	RemoveItem(windowDidEnterFullScreenListeners, listener);
}

void WindowDelegate::RemoveWindowDidExitFullScreenListener(const NotificationListener* listener) {
	RemoveItem(windowDidExitFullScreenListeners, listener);
}

void WindowDelegate::RemoveWindowWillCloseListener(const NotificationListener* listener) {
	RemoveItem(windowWillCloseListeners, listener);
}

void WindowDelegate::RemoveWindowDidResizeListener(const size_t index) {
	RemoveIndex(windowDidResizeListeners, index, "WindowDelegate::RemoveWindowDidResizeListener");
}

void WindowDelegate::RemoveWindowDidMoveListener(const size_t index) {
	RemoveIndex(windowDidMoveListeners, index, "WindowDelegate::RemoveWindowDidMoveListener");
}

void WindowDelegate::RemoveWindowDidBecomeKeyListener(const size_t index) {
	RemoveIndex(windowDidBecomeKeyListeners, index, "WindowDelegate::RemoveWindowDidBecomeKeyListener");
}

void WindowDelegate::RemoveWindowDidResignKeyListener(const size_t index) {
	RemoveIndex(windowDidResignKeyListeners, index, "WindowDelegate::RemoveWindowDidResignKeyListener");
}

void WindowDelegate::RemoveWindowDidMiniaturizeListener(const size_t index) {
	RemoveIndex(windowDidMiniaturizeListeners, index, "WindowDelegate::RemoveWindowDidMiniaturizeListener");
}

void WindowDelegate::RemoveWindowDidDeminiaturizeListener(const size_t index) {
	RemoveIndex(windowDidDeminiaturizeListeners, index, "WindowDelegate::RemoveWindowDidDeminiaturizeListener");
}

void WindowDelegate::RemoveWindowDidEnterFullScreenListener(const size_t index) {
	RemoveIndex(windowDidEnterFullScreenListeners, index, "WindowDelegate::RemoveWindowDidEnterFullScreenListener");
}

void WindowDelegate::RemoveWindowDidExitFullScreenListener(const size_t index) {
	RemoveIndex(windowDidExitFullScreenListeners, index, "WindowDelegate::RemoveWindowDidExitFullScreenListener");
}

void WindowDelegate::RemoveWindowWillCloseListener(const size_t index) {
	RemoveIndex(windowWillCloseListeners, index, "WindowDelegate::RemoveWindowWillCloseListener");
}

}
