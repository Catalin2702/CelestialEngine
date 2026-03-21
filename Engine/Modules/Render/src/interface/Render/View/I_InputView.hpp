//
// Module: CelestialEngine/Engine/Modules/Render/View
// File: I_InputView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_VIEW_I_VIEW_HPP
#define CE_RENDER_VIEW_I_VIEW_HPP

#include "Utility/Callback/Callback.hpp"

namespace CE::Events {
class I_Event;

class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;

class MouseMovedEvent;
class MouseScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseDraggedEvent;
}

using KeyPressedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyPressedEvent>;
using KeyReleasedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyReleasedEvent>;
using KeyTypedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyTypedEvent>;

using MouseMovedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseMovedEvent>;
using MouseScrolledEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseScrolledEvent>;
using MouseButtonPressedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseButtonPressedEvent>;
using MouseButtonReleasedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseButtonReleasedEvent>;
using MouseDraggedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseDraggedEvent>;


namespace CE::Render::View {

class I_InputView {
public:
	virtual ~I_InputView() = default;

public:
	virtual void SetKeyPressedEventCallback(const KeyPressedEventCallbackFn& callback) = 0;
	virtual void SetKeyReleasedEventCallback(const KeyReleasedEventCallbackFn& callback) = 0;
	virtual void SetKeyTypedEventCallback(const KeyTypedEventCallbackFn& callback) = 0;

	virtual void SetMouseMovedEventCallback(const MouseMovedEventCallbackFn& callback) = 0;
	virtual void SetMouseScrolledEventCallback(const MouseScrolledEventCallbackFn& callback) = 0;
	virtual void SetMouseButtonPressedEventCallback(const MouseButtonPressedEventCallbackFn& callback) = 0;
	virtual void SetMouseButtonReleasedEventCallback(const MouseButtonReleasedEventCallbackFn& callback) = 0;
	virtual void SetMouseDraggedEventCallback(const MouseDraggedEventCallbackFn& callback) = 0;
};

}

#endif //CE_RENDER_VIEW_I_VIEW_HPP
