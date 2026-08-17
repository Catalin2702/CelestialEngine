//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: CocoaEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP

#include "Core/MainHub/Events/I_ApplicationEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_KeyboardEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_MouseEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_RenderContextEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_WindowEventHubDispatcher.hpp"

#include "Utility/Delegate/Dispatcher.hpp"


namespace NS {
	class Event;
	class Notification;
}

namespace CE::Core {

class MetalContext;
class CocoaWindow;

class CE_CORE_API CocoaEventHubDispatcher:
	public I_ApplicationEventHubDispatcher,
	public I_KeyboardEventHubDispatcher,
	public I_MouseEventHubDispatcher,
	public I_RenderContextEventHubDispatcher,
	public I_WindowEventHubDispatcher
{
public:
	struct CocoaApplicationEventHub {
		MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
		MulticastDispatcher<Events::AppRenderEvent&> onRenderMulticastDispatcher;
		MulticastDispatcher<Events::AppTickEvent&> onTickMulticastDispatcher;
		MulticastDispatcher<Events::AppUpdateEvent&> onUpdateMulticastDispatcher;
	};

	struct CocoaKeyboardEventHub {
		MulticastDispatcher<Events::KeyPressedEvent&> onPressedMulticastDispatcher;
		MulticastDispatcher<Events::KeyReleasedEvent&> onReleasedMulticastDispatcher;
		MulticastDispatcher<Events::KeyTypedEvent&> onTypedMulticastDispatcher;
	};

	struct CocoaMouseEventHub {
		MulticastDispatcher<Events::MouseMovedEvent&> onMovedMulticastDispatcher;
		MulticastDispatcher<Events::MouseButtonPressedEvent&> onButtonPressedMulticastDispatcher;
		MulticastDispatcher<Events::MouseButtonReleasedEvent&> onButtonReleasedMulticastDispatcher;
		MulticastDispatcher<Events::MouseDraggedEvent&> onDraggedMulticastDispatcher;
		MulticastDispatcher<Events::MouseWheelScrolledEvent&> onWheelScrolledMulticastDispatcher;
	};

	struct CocoaWindowEventHub {
		MulticastDispatcher<Events::WindowCloseEvent&> onCloseMulticastDispatcher;
		MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
		MulticastDispatcher<Events::WindowResizeEvent&> onResizeMulticastDispatcher;
		MulticastDispatcher<Events::WindowFocusEvent&> onFocusMulticastDispatcher;	///< Fired on didBecomeKey/didResignKey; input state resets held keys on focus loss
	};

	struct MetalRenderContextEventHub {
		MulticastDispatcher<Events::VSyncEvent&> onChangeVSyncDispatcher;
		MulticastDispatcher<Events::ViewResizeEvent&> onResizeViewDispatcher;
	};

public:
	CocoaEventHubDispatcher() = default;
	~CocoaEventHubDispatcher() override = default;

public:
	/**
	 * @brief Sets the render view / window used to translate native mouse coordinates into engine (top-left) space
	 * @param context Metal render context owning the view input is reported against (non-owning)
	 * @param window Cocoa window used to resolve the frame size for the Y flip (non-owning)
	 */
	void SetSources(MetalContext* context, CocoaWindow* window);

public:
#pragma region DispatchApplicationEvent
	/**
	 * @brief Multicasts the app tick event to every subscriber of the matching hub dispatcher
	 */
	void DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) override;
	/**
	 * @brief Multicasts the app update event to every subscriber of the matching hub dispatcher
	 */
	void DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) override;
	/**
	 * @brief Multicasts the app render event to every subscriber of the matching hub dispatcher
	 */
	void DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) override;
	/**
	 * @brief Multicasts the app error event to every subscriber of the matching hub dispatcher
	 */
	void DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) override;
#pragma endregion

#pragma region DispatchKeyboardEvent
	/**
	 * @brief Multicasts the key pressed event to every subscriber of the matching hub dispatcher
	 */
	void DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) override;
	/**
	 * @brief Multicasts the key released event to every subscriber of the matching hub dispatcher
	 */
	void DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) override;
	/**
	 * @brief Multicasts the key typed event to every subscriber of the matching hub dispatcher
	 */
	void DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) override;
#pragma endregion

#pragma region DispatchMouseEvent
	/**
	 * @brief Multicasts the mouse moved event to every subscriber of the matching hub dispatcher
	 */
	void DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) override;
	/**
	 * @brief Multicasts the mouse button pressed event to every subscriber of the matching hub dispatcher
	 */
	void DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) override;
	/**
	 * @brief Multicasts the mouse button released event to every subscriber of the matching hub dispatcher
	 */
	void DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) override;
	/**
	 * @brief Multicasts the mouse dragged event to every subscriber of the matching hub dispatcher
	 */
	void DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) override;
	/**
	 * @brief Multicasts the mouse wheel scrolled event to every subscriber of the matching hub dispatcher
	 */
	void DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) override;
#pragma endregion

#pragma region DispatchRenderContextEvent
	/**
	 * @brief Multicasts the render context change v sync event to every subscriber of the matching hub dispatcher
	 */
	void DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& VSyncChangeEvent) override;
	/**
	 * @brief Multicasts the render context resize view event to every subscriber of the matching hub dispatcher
	 */
	void DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent);
#pragma endregion

#pragma region DispatchWindowEvent
	/**
	 * @brief Multicasts the window close event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) override;
	/**
	 * @brief Multicasts the window error event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowErrorEvent(Events::ErrorEvent&) override;
	/**
	 * @brief Multicasts the window resize event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) override;
	/**
	 * @brief Multicasts a window focus change to every subscriber of the window focus hub
	 */
	void DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) override;
#pragma endregion

public:
#pragma region ReceiveApplicationEvent
	/**
	 * @brief Translates the raw app error callback into the engine event and dispatches it through the hub
	 */
	void ReceiveAppErrorEvent(int errorCode, const char* description);
	/**
	 * @brief Translates the raw app render callback into the engine event and dispatches it through the hub
	 */
	void ReceiveAppRenderEvent();
	/**
	 * @brief Translates the raw app tick callback into the engine event and dispatches it through the hub
	 */
	void ReceiveAppTickEvent();
	/**
	 * @brief Translates the raw app update callback into the engine event and dispatches it through the hub
	 */
	void ReceiveAppUpdateEvent();
#pragma endregion

#pragma region ReceiveKeyboardEvent
	/**
	 * @brief Translates the raw key down callback into the engine event and dispatches it through the hub
	 */
	void ReceiveKeyDownEvent(const NS::Event* event);
	/**
	 * @brief Translates the raw key up callback into the engine event and dispatches it through the hub
	 */
	void ReceiveKeyUpEvent(const NS::Event* event);
	/**
	 * @brief Translates a native flagsChanged event into a KeyPressed/KeyReleased event for the modifier key
	 * @param event Native flagsChanged event carrying the modifier's keyCode and the new modifier flags
	 * @details AppKit never sends keyDown/keyUp for modifier keys (Shift/Ctrl/Alt/Cmd): they arrive as a single
	 *			flagsChanged. The device-dependent modifier bits in modifierFlags tell whether the specific left/right
	 *			key is now down (the device-independent masks cannot distinguish e.g. releasing LeftShift while
	 *			RightShift is still held). Untracked modifiers (CapsLock, Fn) are ignored.
	 */
	void ReceiveFlagsChangedEvent(const NS::Event* event);
#pragma endregion

#pragma region ReceiveMouseEvent
	/**
	 * @brief Translates the raw mouse button down callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseButtonDownEvent(const NS::Event* event);
	/**
	 * @brief Translates the raw mouse button up callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseButtonUpEvent(const NS::Event* event);
	/**
	 * @brief Translates the raw mouse dragged callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseDraggedEvent(const NS::Event* event);
	/**
	 * @brief Translates the raw mouse moved callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseMovedEvent(const NS::Event* event);
	/**
	 * @brief Translates the raw scroll wheel callback into the engine event and dispatches it through the hub
	 */
	void ReceiveScrollWheelEvent(const NS::Event* event);
#pragma endregion

#pragma region ReceiveRenderContextEvent
	/**
	 * @brief Translates the raw context change v sync callback into the engine event and dispatches it through the hub
	 */
	void ReceiveContextChangeVSyncEvent(bool state);
	/**
	 * @brief Translates the raw context resize view callback into the engine event and dispatches it through the hub
	 */
	void ReceiveContextResizeViewEvent(double width, double height);
#pragma endregion

#pragma region ReceiveWindowEvent
	/**
	 * @brief Translates the raw window will close callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowWillCloseEvent(const NS::Notification* notification);
	/**
	 * @brief Translates the raw window error callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowErrorEvent(int errorCode, const char* description);
	/**
	 * @brief Translates the raw window resize callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowResizeEvent(unsigned int width, unsigned int height);
	/**
	 * @brief Translates the native didBecomeKey notification into a WindowFocusEvent(focused = true)
	 */
	void ReceiveWindowDidBecomeKeyEvent(const NS::Notification* notification);
	/**
	 * @brief Translates the native didResignKey notification into a WindowFocusEvent(focused = false)
	 * @details Focus loss is what lets the input state release held keys: their keyUp events would otherwise be
	 *			delivered to another window and the keys would remain stuck as "down".
	 */
	void ReceiveWindowDidResignKeyEvent(const NS::Notification* notification);
#pragma endregion

public:
	CocoaApplicationEventHub cocoaApplicationEventHub;
	CocoaKeyboardEventHub cocoaKeyboardEventHub;
	CocoaMouseEventHub cocoaMouseEventHub;
	CocoaWindowEventHub cocoaWindowEventHub;
	MetalRenderContextEventHub metalRenderContextEventHub;

private:
	MetalContext* _context = nullptr; ///< Non-owning; used to convert native mouse coordinates
	CocoaWindow* _window = nullptr;   ///< Non-owning; used to convert native mouse coordinates
};

}

#endif //CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
