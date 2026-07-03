//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include <functional>
#include <vector>

namespace NS {

class WindowEventDispatcher: public I_WindowEventDispatcher {
	using NotificationListener = std::function<void(Notification*)>;
public:
	WindowEventDispatcher() = default;

	~WindowEventDispatcher() override = default;

public:
	void DispatchWindowDidResize(Notification* notification) override;
	void DispatchWindowDidMove(Notification* notification) override;
	void DispatchWindowDidBecomeKey(Notification* notification) override;
	void DispatchWindowDidResignKey(Notification* notification) override;
	void DispatchWindowDidMiniaturize(Notification* notification) override;
	void DispatchWindowDidDeminiaturize(Notification* notification) override;
	void DispatchWindowDidEnterFullScreen(Notification* notification) override;
	void DispatchWindowDidExitFullScreen(Notification* notification) override;
	void DispatchWindowWillClose(Notification* notification) override;

public:
	void AddWindowDidResizeListener(const NotificationListener* listener);
	void AddWindowDidMoveListener(const NotificationListener* listener);
	void AddWindowDidBecomeKeyListener(const NotificationListener* listener);
	void AddWindowDidResignKeyListener(const NotificationListener* listener);
	void AddWindowDidMiniaturizeListener(const NotificationListener* listener);
	void AddWindowDidDeminiaturizeListener(const NotificationListener* listener);
	void AddWindowDidEnterFullScreenListener(const NotificationListener* listener);
	void AddWindowDidExitFullScreenListener(const NotificationListener* listener);
	void AddWindowWillCloseListener(const NotificationListener* listener);

	void AddWindowDidResizeListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidMoveListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidBecomeKeyListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidResignKeyListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidMiniaturizeListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidDeminiaturizeListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidEnterFullScreenListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowDidExitFullScreenListeners(const std::vector<const NotificationListener*>& listeners);
	void AddWindowWillCloseListeners(const std::vector<const NotificationListener*>& listeners);

public:
	void RemoveWindowDidResizeListener(const NotificationListener* listener);
	void RemoveWindowDidMoveListener(const NotificationListener* listener);
	void RemoveWindowDidBecomeKeyListener(const NotificationListener* listener);
	void RemoveWindowDidResignKeyListener(const NotificationListener* listener);
	void RemoveWindowDidMiniaturizeListener(const NotificationListener* listener);
	void RemoveWindowDidDeminiaturizeListener(const NotificationListener* listener);
	void RemoveWindowDidEnterFullScreenListener(const NotificationListener* listener);
	void RemoveWindowDidExitFullScreenListener(const NotificationListener* listener);
	void RemoveWindowWillCloseListener(const NotificationListener* listener);

	void RemoveWindowDidResizeListener(size_t index);
	void RemoveWindowDidMoveListener(size_t index);
	void RemoveWindowDidBecomeKeyListener(size_t index);
	void RemoveWindowDidResignKeyListener(size_t index);
	void RemoveWindowDidMiniaturizeListener(size_t index);
	void RemoveWindowDidDeminiaturizeListener(size_t index);
	void RemoveWindowDidEnterFullScreenListener(size_t index);
	void RemoveWindowDidExitFullScreenListener(size_t index);
	void RemoveWindowWillCloseListener(size_t index);

private:
	std::vector<const NotificationListener*> _windowDidResizeListeners;
	std::vector<const NotificationListener*> _windowDidMoveListeners;
	std::vector<const NotificationListener*> _windowDidBecomeKeyListeners;
	std::vector<const NotificationListener*> _windowDidResignKeyListeners;
	std::vector<const NotificationListener*> _windowDidMiniaturizeListeners;
	std::vector<const NotificationListener*> _windowDidDeminiaturizeListeners;
	std::vector<const NotificationListener*> _windowDidEnterFullScreenListeners;
	std::vector<const NotificationListener*> _windowDidExitFullScreenListeners;
	std::vector<const NotificationListener*> _windowWillCloseListeners;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
