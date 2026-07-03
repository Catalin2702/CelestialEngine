//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"
#include "Tools/Log/Log.hpp"


static void CallListeners(const std::vector<const std::function<void(NS::Notification*)>*>& vec, NS::Notification* notification) {
	for (const auto listener: vec) {
		if (listener)
			(*listener)(notification);
	}
}

static void ExtendListeners(std::vector<const std::function<void(NS::Notification*)>*>& vec, const std::vector<const std::function<void(NS::Notification*)>*>& source) {
	vec.reserve(vec.size() + source.size());
	vec.insert(vec.end(), source.begin(), source.end());
}

static void RemoveItem(std::vector<const std::function<void(NS::Notification*)>*>& vec, const std::function<void(NS::Notification*)>* item) {
	std::erase_if(vec, [&](const auto l) {
		return l == nullptr or l == item;
	});
}

static void RemoveIndex(std::vector<const std::function<void(NS::Notification*)>*>& vec, const size_t index, const char* functionCalled) {
	if (index >= vec.size()) {
		const auto message = std::string(functionCalled) + ": Index " + std::to_string(index) + " Vector size: " + std::to_string(vec.size());
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}
	vec.erase(vec.begin() + static_cast<long>(index));
}

namespace NS {

void WindowEventDispatcher::DispatchWindowDidResize(Notification* notification) {
	CallListeners(windowDidResizeListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidMove(Notification* notification) {
	CallListeners(windowDidMoveListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidBecomeKey(Notification* notification) {
	CallListeners(windowDidBecomeKeyListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidResignKey(Notification* notification) {
	CallListeners(windowDidResignKeyListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidMiniaturize(Notification* notification) {
	CallListeners(windowDidMiniaturizeListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidDeminiaturize(Notification* notification) {
	CallListeners(windowDidDeminiaturizeListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidEnterFullScreen(Notification* notification) {
	CallListeners(windowDidEnterFullScreenListeners, notification);
}

void WindowEventDispatcher::DispatchWindowDidExitFullScreen(Notification* notification) {
	CallListeners(windowDidExitFullScreenListeners, notification);
}

void WindowEventDispatcher::DispatchWindowWillClose(Notification* notification) {
	CallListeners(windowWillCloseListeners, notification);
}

void WindowEventDispatcher::AddWindowDidResizeListener(const NotificationListener* listener) {
	windowDidResizeListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidMoveListener(const NotificationListener* listener) {
	windowDidMoveListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidBecomeKeyListener(const NotificationListener* listener) {
	windowDidBecomeKeyListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidResignKeyListener(const NotificationListener* listener) {
	windowDidResignKeyListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidMiniaturizeListener(const NotificationListener* listener) {
	windowDidMiniaturizeListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidDeminiaturizeListener(const NotificationListener* listener) {
	windowDidDeminiaturizeListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidEnterFullScreenListener(const NotificationListener* listener) {
	windowDidEnterFullScreenListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidExitFullScreenListener(const NotificationListener* listener) {
	windowDidExitFullScreenListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowWillCloseListener(const NotificationListener* listener) {
	windowWillCloseListeners.push_back(listener);
}

void WindowEventDispatcher::AddWindowDidResizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidResizeListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidMoveListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidMoveListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidBecomeKeyListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidBecomeKeyListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidResignKeyListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidResignKeyListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidMiniaturizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidMiniaturizeListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidDeminiaturizeListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidDeminiaturizeListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidEnterFullScreenListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidEnterFullScreenListeners, listeners);
}

void WindowEventDispatcher::AddWindowDidExitFullScreenListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowDidExitFullScreenListeners, listeners);
}

void WindowEventDispatcher::AddWindowWillCloseListeners(const std::vector<const NotificationListener*>& listeners) {
	ExtendListeners(windowWillCloseListeners, listeners);
}

void WindowEventDispatcher::RemoveWindowDidResizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidResizeListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidMoveListener(const NotificationListener* listener) {
	RemoveItem(windowDidMoveListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidBecomeKeyListener(const NotificationListener* listener) {
	RemoveItem(windowDidBecomeKeyListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidResignKeyListener(const NotificationListener* listener) {
	RemoveItem(windowDidResignKeyListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidMiniaturizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidMiniaturizeListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidDeminiaturizeListener(const NotificationListener* listener) {
	RemoveItem(windowDidDeminiaturizeListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidEnterFullScreenListener(const NotificationListener* listener) {
	RemoveItem(windowDidEnterFullScreenListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidExitFullScreenListener(const NotificationListener* listener) {
	RemoveItem(windowDidExitFullScreenListeners, listener);
}

void WindowEventDispatcher::RemoveWindowWillCloseListener(const NotificationListener* listener) {
	RemoveItem(windowWillCloseListeners, listener);
}

void WindowEventDispatcher::RemoveWindowDidResizeListener(const size_t index) {
	RemoveIndex(windowDidResizeListeners, index, "WindowDelegate::RemoveWindowDidResizeListener");
}

void WindowEventDispatcher::RemoveWindowDidMoveListener(const size_t index) {
	RemoveIndex(windowDidMoveListeners, index, "WindowDelegate::RemoveWindowDidMoveListener");
}

void WindowEventDispatcher::RemoveWindowDidBecomeKeyListener(const size_t index) {
	RemoveIndex(windowDidBecomeKeyListeners, index, "WindowDelegate::RemoveWindowDidBecomeKeyListener");
}

void WindowEventDispatcher::RemoveWindowDidResignKeyListener(const size_t index) {
	RemoveIndex(windowDidResignKeyListeners, index, "WindowDelegate::RemoveWindowDidResignKeyListener");
}

void WindowEventDispatcher::RemoveWindowDidMiniaturizeListener(const size_t index) {
	RemoveIndex(windowDidMiniaturizeListeners, index, "WindowDelegate::RemoveWindowDidMiniaturizeListener");
}

void WindowEventDispatcher::RemoveWindowDidDeminiaturizeListener(const size_t index) {
	RemoveIndex(windowDidDeminiaturizeListeners, index, "WindowDelegate::RemoveWindowDidDeminiaturizeListener");
}

void WindowEventDispatcher::RemoveWindowDidEnterFullScreenListener(const size_t index) {
	RemoveIndex(windowDidEnterFullScreenListeners, index, "WindowDelegate::RemoveWindowDidEnterFullScreenListener");
}

void WindowEventDispatcher::RemoveWindowDidExitFullScreenListener(const size_t index) {
	RemoveIndex(windowDidExitFullScreenListeners, index, "WindowDelegate::RemoveWindowDidExitFullScreenListener");
}

void WindowEventDispatcher::RemoveWindowWillCloseListener(const size_t index) {
	RemoveIndex(windowWillCloseListeners, index, "WindowDelegate::RemoveWindowWillCloseListener");
}

}
