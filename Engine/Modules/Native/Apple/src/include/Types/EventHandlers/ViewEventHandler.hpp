//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: ViewEventHandler.hpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-21
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWEVENTHANDLER_HPP

#include "Types/Types.hpp"
#include "Types/EventHandlers/I_ViewEventHandler.hpp"

namespace CE::Apple::Types {

class ViewEventHandler final: public I_ViewEventHandler {
public:
	void OnKeyPressed(NativeEventCallback callback) override;
	void OnKeyReleased(NativeEventCallback callback) override;
	void OnKeyTyped(NativeEventCallback callback) override;
	void OnMouseMoved(NativeEventCallback callback) override;
	void OnMouseScrolled(NativeEventCallback callback) override;
	void OnMouseButtonPressed(NativeEventCallback callback) override;
	void OnMouseButtonReleased(NativeEventCallback callback) override;
	void OnMouseDragged(NativeEventCallback callback) override;

public:
	void DispatchKeyPressed(NS::Event* event) override;
	void DispatchKeyReleased(NS::Event* event) override;
	void DispatchKeyTyped(NS::Event* event) override;
	void DispatchMouseMoved(NS::Event* event) override;
	void DispatchMouseScrolled(NS::Event* event) override;
	void DispatchMouseButtonPressed(NS::Event* event) override;
	void DispatchMouseButtonReleased(NS::Event* event) override;
	void DispatchMouseDragged(NS::Event* event) override;

private:
	NativeEventCallback _keyPressedCallback;
	NativeEventCallback _keyReleasedCallback;
	NativeEventCallback _keyTypedCallback;
	NativeEventCallback _mouseMovedCallback;
	NativeEventCallback _mouseScrolledCallback;
	NativeEventCallback _mouseButtonPressedCallback;
	NativeEventCallback _mouseButtonReleasedCallback;
	NativeEventCallback _mouseDraggedCallback;
};

}

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_VIEWEVENTHANDLER_HPP
