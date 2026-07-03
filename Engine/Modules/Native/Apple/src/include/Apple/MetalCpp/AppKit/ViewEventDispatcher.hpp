//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include <functional>
#include <vector>

namespace NS {

class ViewEventDispatcher: public I_ViewEventDispatcher {
	using EventListener = std::function<void(Event*)>;
public:
	ViewEventDispatcher() = default;

public:
	void DispatchMouseDown(Event* event) override;
	void DispatchMouseUp(Event* event) override;
	void DispatchMouseDragged(Event* event) override;
	void DispatchRightMouseDown(Event* event) override;
	void DispatchRightMouseUp(Event* event) override;
	void DispatchRightMouseDragged(Event* event) override;
	void DispatchOtherMouseDown(Event* event) override;
	void DispatchOtherMouseUp(Event* event) override;
	void DispatchOtherMouseDragged(Event* event) override;
	void DispatchMouseMoved(Event* event) override;
	void DispatchMouseEntered(Event* event) override;
	void DispatchMouseExited(Event* event) override;
	void DispatchKeyDown(Event* event) override;
	void DispatchKeyUp(Event* event) override;
	void DispatchFlagsChanged(Event* event) override;
	void DispatchScrollWheel(Event* event) override;

public:
	void AddMouseDownListener(const EventListener* listener);
	void AddMouseUpListener(const EventListener* listener);
	void AddMouseDraggedListener(const EventListener* listener);
	void AddRightMouseDownListener(const EventListener* listener);
	void AddRightMouseUpListener(const EventListener* listener);
	void AddRightMouseDraggedListener(const EventListener* listener);
	void AddOtherMouseDownListener(const EventListener* listener);
	void AddOtherMouseUpListener(const EventListener* listener);
	void AddOtherMouseDraggedListener(const EventListener* listener);
	void AddMouseMovedListener(const EventListener* listener);
	void AddMouseEnteredListener(const EventListener* listener);
	void AddMouseExitedListener(const EventListener* listener);
	void AddKeyDownListener(const EventListener* listener);
	void AddKeyUpListener(const EventListener* listener);
	void AddFlagsChangedListener(const EventListener* listener);
	void AddScrollWheelListener(const EventListener* listener);

	void AddMouseDownListeners(const std::vector<const EventListener*>& listeners);
	void AddMouseUpListeners(const std::vector<const EventListener*>& listeners);
	void AddMouseDraggedListeners(const std::vector<const EventListener*>& listeners);
	void AddRightMouseDownListeners(const std::vector<const EventListener*>& listeners);
	void AddRightMouseUpListeners(const std::vector<const EventListener*>& listeners);
	void AddRightMouseDraggedListeners(const std::vector<const EventListener*>& listeners);
	void AddOtherMouseDownListeners(const std::vector<const EventListener*>& listeners);
	void AddOtherMouseUpListeners(const std::vector<const EventListener*>& listeners);
	void AddOtherMouseDraggedListeners(const std::vector<const EventListener*>& listeners);
	void AddMouseMovedListeners(const std::vector<const EventListener*>& listeners);
	void AddMouseEnteredListeners(const std::vector<const EventListener*>& listeners);
	void AddMouseExitedListeners(const std::vector<const EventListener*>& listeners);
	void AddKeyDownListeners(const std::vector<const EventListener*>& listeners);
	void AddKeyUpListeners(const std::vector<const EventListener*>& listeners);
	void AddFlagsChangedListeners(const std::vector<const EventListener*>& listeners);
	void AddScrollWheelListeners(const std::vector<const EventListener*>& listeners);

public:
	void RemoveMouseDownListener(const EventListener* listener);
	void RemoveMouseUpListener(const EventListener* listener);
	void RemoveMouseDraggedListener(const EventListener* listener);
	void RemoveRightMouseDownListener(const EventListener* listener);
	void RemoveRightMouseUpListener(const EventListener* listener);
	void RemoveRightMouseDraggedListener(const EventListener* listener);
	void RemoveOtherMouseDownListener(const EventListener* listener);
	void RemoveOtherMouseUpListener(const EventListener* listener);
	void RemoveOtherMouseDraggedListener(const EventListener* listener);
	void RemoveMouseMovedListener(const EventListener* listener);
	void RemoveMouseEnteredListener(const EventListener* listener);
	void RemoveMouseExitedListener(const EventListener* listener);
	void RemoveKeyDownListener(const EventListener* listener);
	void RemoveKeyUpListener(const EventListener* listener);
	void RemoveFlagsChangedListener(const EventListener* listener);
	void RemoveScrollWheelListener(const EventListener* listener);

	void RemoveMouseDownListener(size_t index);
	void RemoveMouseUpListener(size_t index);
	void RemoveMouseDraggedListener(size_t index);
	void RemoveRightMouseDownListener(size_t index);
	void RemoveRightMouseUpListener(size_t index);
	void RemoveRightMouseDraggedListener(size_t index);
	void RemoveOtherMouseDownListener(size_t index);
	void RemoveOtherMouseUpListener(size_t index);
	void RemoveOtherMouseDraggedListener(size_t index);
	void RemoveMouseMovedListener(size_t index);
	void RemoveMouseEnteredListener(size_t index);
	void RemoveMouseExitedListener(size_t index);
	void RemoveKeyDownListener(size_t index);
	void RemoveKeyUpListener(size_t index);
	void RemoveFlagsChangedListener(size_t index);
	void RemoveScrollWheelListener(size_t index);

private:
	std::vector<const EventListener*> _mouseDownListeners;
	std::vector<const EventListener*> _mouseUpListeners;
	std::vector<const EventListener*> _mouseDraggedListeners;
	std::vector<const EventListener*> _rightMouseDownListeners;
	std::vector<const EventListener*> _rightMouseUpListeners;
	std::vector<const EventListener*> _rightMouseDraggedListeners;
	std::vector<const EventListener*> _otherMouseDownListeners;
	std::vector<const EventListener*> _otherMouseUpListeners;
	std::vector<const EventListener*> _otherMouseDraggedListeners;
	std::vector<const EventListener*> _mouseMovedListeners;
	std::vector<const EventListener*> _mouseEnteredListeners;
	std::vector<const EventListener*> _mouseExitedListeners;
	std::vector<const EventListener*> _keyDownListeners;
	std::vector<const EventListener*> _keyUpListeners;
	std::vector<const EventListener*> _flagsChangedListeners;
	std::vector<const EventListener*> _scrollWheelListeners;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
