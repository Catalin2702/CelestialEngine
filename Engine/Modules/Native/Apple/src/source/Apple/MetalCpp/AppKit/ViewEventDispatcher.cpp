//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#include "Apple/MetalCpp/AppKit/ViewEventDispatcher.hpp"
#include "Tools/Log/Log.hpp"

static void CallListeners(const std::vector<const std::function<void(NS::Event*)>*>& vec, NS::Event* notification) {
	for (const auto listener: vec) {
		if (listener)
			(*listener)(notification);
	}
}

static void ExtendListeners(std::vector<const std::function<void(NS::Event*)>*>& vec, const std::vector<const std::function<void(NS::Event*)>*>& source) {
	vec.reserve(vec.size() + source.size());
	vec.insert(vec.end(), source.begin(), source.end());
}

static void RemoveItem(std::vector<const std::function<void(NS::Event*)>*>& vec, const std::function<void(NS::Event*)>* item) {
	std::erase_if(vec, [&](const auto l) {
		return l == nullptr or l == item;
	});
}

static void RemoveIndex(std::vector<const std::function<void(NS::Event*)>*>& vec, const size_t index, const char* functionCalled) {
	if (index >= vec.size()) {
		const auto message = std::string(functionCalled) + ": Index " + std::to_string(index) + " Vector size: " + std::to_string(vec.size());
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}
	vec.erase(vec.begin() + static_cast<long>(index));
}

namespace NS {

void ViewEventDispatcher::DispatchMouseDown(Event* event) {
	CallListeners(_mouseDownListeners, event);
}

void ViewEventDispatcher::DispatchMouseUp(Event* event) {
	CallListeners(_mouseUpListeners, event);
}

void ViewEventDispatcher::DispatchMouseDragged(Event* event) {
	CallListeners(_mouseDraggedListeners, event);
}

void ViewEventDispatcher::DispatchRightMouseDown(Event* event) {
	CallListeners(_rightMouseDownListeners, event);
}

void ViewEventDispatcher::DispatchRightMouseUp(Event* event) {
	CallListeners(_rightMouseUpListeners, event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(Event* event) {
	CallListeners(_rightMouseDraggedListeners, event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(Event* event) {
	CallListeners(_otherMouseDownListeners, event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(Event* event) {
	CallListeners(_otherMouseUpListeners, event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(Event* event) {
	CallListeners(_otherMouseDraggedListeners, event);
}

void ViewEventDispatcher::DispatchMouseMoved(Event* event) {
	CallListeners(_mouseMovedListeners, event);
}

void ViewEventDispatcher::DispatchMouseEntered(Event* event) {
	CallListeners(_mouseEnteredListeners, event);
}

void ViewEventDispatcher::DispatchMouseExited(Event* event) {
	CallListeners(_mouseExitedListeners, event);
}

void ViewEventDispatcher::DispatchKeyDown(Event* event) {
	CallListeners(_keyDownListeners, event);
}

void ViewEventDispatcher::DispatchKeyUp(Event* event) {
	CallListeners(_keyUpListeners, event);
}

void ViewEventDispatcher::DispatchFlagsChanged(Event* event) {
	CallListeners(_flagsChangedListeners, event);
}

void ViewEventDispatcher::DispatchScrollWheel(Event* event) {
	CallListeners(_scrollWheelListeners, event);
}

void ViewEventDispatcher::AddMouseDownListener(const EventListener* listener) {
	_mouseDownListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseUpListener(const EventListener* listener) {
	_mouseUpListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseDraggedListener(const EventListener* listener) {
	_mouseDraggedListeners.push_back(listener);
}

void ViewEventDispatcher::AddRightMouseDownListener(const EventListener* listener) {
	_rightMouseDownListeners.push_back(listener);
}

void ViewEventDispatcher::AddRightMouseUpListener(const EventListener* listener) {
	_rightMouseUpListeners.push_back(listener);
}

void ViewEventDispatcher::AddRightMouseDraggedListener(const EventListener* listener) {
	_rightMouseDraggedListeners.push_back(listener);
}

void ViewEventDispatcher::AddOtherMouseDownListener(const EventListener* listener) {
	_otherMouseDownListeners.push_back(listener);
}

void ViewEventDispatcher::AddOtherMouseUpListener(const EventListener* listener) {
	_otherMouseUpListeners.push_back(listener);
}

void ViewEventDispatcher::AddOtherMouseDraggedListener(const EventListener* listener) {
	_otherMouseDraggedListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseMovedListener(const EventListener* listener) {
	_mouseMovedListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseEnteredListener(const EventListener* listener) {
	_mouseEnteredListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseExitedListener(const EventListener* listener) {
	_mouseExitedListeners.push_back(listener);
}

void ViewEventDispatcher::AddKeyDownListener(const EventListener* listener) {
	_keyDownListeners.push_back(listener);
}

void ViewEventDispatcher::AddKeyUpListener(const EventListener* listener) {
	_keyUpListeners.push_back(listener);
}

void ViewEventDispatcher::AddFlagsChangedListener(const EventListener* listener) {
	_flagsChangedListeners.push_back(listener);
}

void ViewEventDispatcher::AddScrollWheelListener(const EventListener* listener) {
	_scrollWheelListeners.push_back(listener);
}

void ViewEventDispatcher::AddMouseDownListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseDownListeners, listeners);
}

void ViewEventDispatcher::AddMouseUpListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseUpListeners, listeners);
}

void ViewEventDispatcher::AddMouseDraggedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseDraggedListeners, listeners);
}

void ViewEventDispatcher::AddRightMouseDownListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_rightMouseDownListeners, listeners);
}

void ViewEventDispatcher::AddRightMouseUpListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_rightMouseUpListeners, listeners);
}

void ViewEventDispatcher::AddRightMouseDraggedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_rightMouseDraggedListeners, listeners);
}

void ViewEventDispatcher::AddOtherMouseDownListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_otherMouseDownListeners, listeners);
}

void ViewEventDispatcher::AddOtherMouseUpListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_otherMouseUpListeners, listeners);
}

void ViewEventDispatcher::AddOtherMouseDraggedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_otherMouseDraggedListeners, listeners);
}

void ViewEventDispatcher::AddMouseMovedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseMovedListeners, listeners);
}

void ViewEventDispatcher::AddMouseEnteredListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseEnteredListeners, listeners);
}

void ViewEventDispatcher::AddMouseExitedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_mouseExitedListeners, listeners);
}

void ViewEventDispatcher::AddKeyDownListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_keyDownListeners, listeners);
}

void ViewEventDispatcher::AddKeyUpListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_keyUpListeners, listeners);
}

void ViewEventDispatcher::AddFlagsChangedListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_flagsChangedListeners, listeners);
}

void ViewEventDispatcher::AddScrollWheelListeners(const std::vector<const EventListener*>& listeners) {
	ExtendListeners(_scrollWheelListeners, listeners);
}

void ViewEventDispatcher::RemoveMouseDownListener(const EventListener* listener) {
	RemoveItem(_mouseDownListeners, listener);
}

void ViewEventDispatcher::RemoveMouseUpListener(const EventListener* listener) {
	RemoveItem(_mouseUpListeners, listener);
}

void ViewEventDispatcher::RemoveMouseDraggedListener(const EventListener* listener) {
	RemoveItem(_mouseDraggedListeners, listener);
}

void ViewEventDispatcher::RemoveRightMouseDownListener(const EventListener* listener) {
	RemoveItem(_rightMouseDownListeners, listener);
}

void ViewEventDispatcher::RemoveRightMouseUpListener(const EventListener* listener) {
	RemoveItem(_rightMouseUpListeners, listener);
}

void ViewEventDispatcher::RemoveRightMouseDraggedListener(const EventListener* listener) {
	RemoveItem(_rightMouseDraggedListeners, listener);
}

void ViewEventDispatcher::RemoveOtherMouseDownListener(const EventListener* listener) {
	RemoveItem(_otherMouseDownListeners, listener);
}

void ViewEventDispatcher::RemoveOtherMouseUpListener(const EventListener* listener) {
	RemoveItem(_otherMouseUpListeners, listener);
}

void ViewEventDispatcher::RemoveOtherMouseDraggedListener(const EventListener* listener) {
	RemoveItem(_otherMouseDraggedListeners, listener);
}

void ViewEventDispatcher::RemoveMouseMovedListener(const EventListener* listener) {
	RemoveItem(_mouseMovedListeners, listener);
}

void ViewEventDispatcher::RemoveMouseEnteredListener(const EventListener* listener) {
	RemoveItem(_mouseEnteredListeners, listener);
}

void ViewEventDispatcher::RemoveMouseExitedListener(const EventListener* listener) {
	RemoveItem(_mouseExitedListeners, listener);
}

void ViewEventDispatcher::RemoveKeyDownListener(const EventListener* listener) {
	RemoveItem(_keyDownListeners, listener);
}

void ViewEventDispatcher::RemoveKeyUpListener(const EventListener* listener) {
	RemoveItem(_keyUpListeners, listener);
}

void ViewEventDispatcher::RemoveFlagsChangedListener(const EventListener* listener) {
	RemoveItem(_flagsChangedListeners, listener);
}

void ViewEventDispatcher::RemoveScrollWheelListener(const EventListener* listener) {
	RemoveItem(_scrollWheelListeners, listener);
}

void ViewEventDispatcher::RemoveMouseDownListener(const size_t index) {
	RemoveIndex(_mouseDownListeners, index, "ViewEventDispatcher::RemoveMouseDownListener");
}

void ViewEventDispatcher::RemoveMouseUpListener(const size_t index) {
	RemoveIndex(_mouseUpListeners, index, "ViewEventDispatcher::RemoveMouseUpListener");
}

void ViewEventDispatcher::RemoveMouseDraggedListener(const size_t index) {
	RemoveIndex(_mouseDraggedListeners, index, "ViewEventDispatcher::RemoveMouseDraggedListener");
}

void ViewEventDispatcher::RemoveRightMouseDownListener(const size_t index) {
	RemoveIndex(_rightMouseDownListeners, index, "ViewEventDispatcher::RemoveRightMouseDownListener");
}

void ViewEventDispatcher::RemoveRightMouseUpListener(const size_t index) {
	RemoveIndex(_rightMouseUpListeners, index, "ViewEventDispatcher::RemoveRightMouseUpListener");
}

void ViewEventDispatcher::RemoveRightMouseDraggedListener(const size_t index) {
	RemoveIndex(_rightMouseDraggedListeners, index, "ViewEventDispatcher::RemoveRightMouseDraggedListener");
}

void ViewEventDispatcher::RemoveOtherMouseDownListener(const size_t index) {
	RemoveIndex(_otherMouseDownListeners, index, "ViewEventDispatcher::RemoveOtherMouseDownListener");
}

void ViewEventDispatcher::RemoveOtherMouseUpListener(const size_t index) {
	RemoveIndex(_otherMouseUpListeners, index, "ViewEventDispatcher::RemoveOtherMouseUpListener");
}

void ViewEventDispatcher::RemoveOtherMouseDraggedListener(const size_t index) {
	RemoveIndex(_otherMouseDraggedListeners, index, "ViewEventDispatcher::RemoveOtherMouseDraggedListener");
}

void ViewEventDispatcher::RemoveMouseMovedListener(const size_t index) {
	RemoveIndex(_mouseMovedListeners, index, "ViewEventDispatcher::RemoveMouseMovedListener");
}

void ViewEventDispatcher::RemoveMouseEnteredListener(const size_t index) {
	RemoveIndex(_mouseEnteredListeners, index, "ViewEventDispatcher::RemoveMouseEnteredListener");
}

void ViewEventDispatcher::RemoveMouseExitedListener(const size_t index) {
	RemoveIndex(_mouseExitedListeners, index, "ViewEventDispatcher::RemoveMouseExitedListener");
}

void ViewEventDispatcher::RemoveKeyDownListener(const size_t index) {
	RemoveIndex(_keyDownListeners, index, "ViewEventDispatcher::RemoveKeyDownListener");
}

void ViewEventDispatcher::RemoveKeyUpListener(const size_t index) {
	RemoveIndex(_keyUpListeners, index, "ViewEventDispatcher::RemoveKeyUpListener");
}

void ViewEventDispatcher::RemoveFlagsChangedListener(const size_t index) {
	RemoveIndex(_flagsChangedListeners, index, "ViewEventDispatcher::RemoveFlagsChangedListener");
}

void ViewEventDispatcher::RemoveScrollWheelListener(const size_t index) {
	RemoveIndex(_scrollWheelListeners, index, "ViewEventDispatcher::RemoveScrollWheelListener");
}

}
