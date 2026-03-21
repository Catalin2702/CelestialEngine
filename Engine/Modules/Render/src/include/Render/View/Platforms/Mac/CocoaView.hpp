//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac
// File: CocoaView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_VIEW_MAC_COCOAVIEW_HPP
#define CE_RENDER_VIEW_MAC_COCOAVIEW_HPP

#include "Render/View/I_InputView.hpp"

#include <Foundation/Foundation.hpp>


namespace NS {
class View;
class Window;
}

namespace CE::Render::View {

struct CocoaCallbacks {
#pragma region KeyCallbacks
	KeyPressedEventCallbackFn KeyPressedEventCallback;						///< Callback for key pressed events
	KeyReleasedEventCallbackFn KeyReleasedEventCallback;					///< Callback for key released events
	KeyTypedEventCallbackFn KeyTypedEventCallback;							///< Callback for key typed events
#pragma endregion

#pragma region MouseCallbacks
	MouseMovedEventCallbackFn MouseMovedEventCallback;						///< Callback for mouse moved events
	MouseScrolledEventCallbackFn MouseScrolledEventCallback;				///< Callback for mouse scrolled events
	MouseButtonPressedEventCallbackFn MouseButtonPressedEventCallback;		///< Callback for mouse button pressed events
	MouseButtonReleasedEventCallbackFn MouseButtonReleasedEventCallback;	///< Callback for mouse button released events
	MouseDraggedEventCallbackFn MouseDraggedEventCallback;					///< Callback for mouse dragged events
#pragma endregion
};

class CocoaView final: public I_InputView{
public:
	/**
	 * @brief Constructor
	 * @param window Pointer to the NS::Window that this view will be associated with
	 * @details Initialize the view and set the content view using the window
	 */
	CocoaView(NS::Window* window);
	~CocoaView() override;

public:
	void SetCocoaCallbacks(const CocoaCallbacks& callbacks);

	void SetKeyPressedEventCallback(const KeyPressedEventCallbackFn& callback) override;
	void SetKeyReleasedEventCallback(const KeyReleasedEventCallbackFn& callback) override;
	void SetKeyTypedEventCallback(const KeyTypedEventCallbackFn& callback) override;
	void SetMouseMovedEventCallback(const MouseMovedEventCallbackFn& callback) override;
	void SetMouseScrolledEventCallback(const MouseScrolledEventCallbackFn& callback) override;
	void SetMouseButtonPressedEventCallback(const MouseButtonPressedEventCallbackFn& callback) override;
	void SetMouseButtonReleasedEventCallback(const MouseButtonReleasedEventCallbackFn& callback) override;
	void SetMouseDraggedEventCallback(const MouseDraggedEventCallbackFn& callback) override;

private:
	NS::SharedPtr<NS::View> _view;						///< Opaque pointer to the NSView subclass instance
};

}

#endif //CE_RENDER_VIEW_MAC_COCOAVIEW_HPP
