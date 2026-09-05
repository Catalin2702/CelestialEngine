//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: CocoaEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP

#include "Core/Hub/Events/I_EventHubDispatcher.hpp"

namespace NS {
	class Event;
	class Notification;
}

namespace CE::Core {

class CocoaWindow;

/**
 * @struct MetalRenderContextEventHub
 * @brief The render context channels, plus the one only the Metal path can raise
 * @details MTK::View reports a drawable resize of its own, which has no OpenGL equivalent - the GL backend learns
 *			about a resize from the window. So it is added here instead of in the shared struct, and a subscriber that
 *			wants it holds the concrete hub rather than the interface.
 */
struct MetalRenderContextEventHub: RenderContextEventHub {
	MulticastDispatcher<Events::ViewResizeEvent&> onResizeViewDispatcher;
};

/**
 * @class CocoaEventHubDispatcher
 * @brief The event hub fed by the AppKit and MetalKit callbacks
 * @details Holds the shared channels and translates the NS::Event and MTK::View callbacks into engine events.
 *			Everything backend-specific stops at the Receive* methods; what leaves through the hub is the same on
 *			every platform, bar the Metal-only drawable resize.
 */
class CE_CORE_API CocoaEventHubDispatcher final: public I_EventHubDispatcherBase<Types::WindowApi::Cocoa> {
public:
	// The user-declared destructor suppresses the implicit move constructor and move assignment, so a move of an owner
	// holding this dispatcher by value would silently fall back to a copy. They are re-declared here (memberwise: every
	// member is a nothrow-movable dispatcher). Copies stay deleted: this is a polymorphic type, and duplicating a hub
	// would deliver every event twice.
	CocoaEventHubDispatcher() = default;
	CocoaEventHubDispatcher(const CocoaEventHubDispatcher&) = delete;
	CocoaEventHubDispatcher(CocoaEventHubDispatcher&&) noexcept = default;
	~CocoaEventHubDispatcher() override = default;

	CocoaEventHubDispatcher& operator = (const CocoaEventHubDispatcher&) = delete;
	CocoaEventHubDispatcher& operator = (CocoaEventHubDispatcher&&) noexcept = default;

public:
	/**
	 * @brief Sets the window whose coordinate system native mouse positions are translated out of
	 * @param window Cocoa window input is reported against (non-owning; it must outlive this hub)
	 * @details One source, where there used to be two. The view was asked of the render context because the context
	 *			owned it; now the window does, and it is the window's own content view that input is actually
	 *			delivered to - so asking the window for it is both shorter and harder to get wrong.
	 */
	void SetSources(CocoaWindow* window);

public:

	/**
	 * @brief Multicasts the drawable resize the MetalKit view raises to every subscriber
	 * @details The one hook with no shared counterpart: OpenGL learns about a resize from the window instead.
	 */
	void DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent);

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
	void ReceiveAppTickEvent(f32 deltaTime);
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
	void ReceiveContextResizeViewEvent(f64 width, f64 height);
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
	[[nodiscard]] ApplicationEventHub& GetApplicationEventHub() override { return applicationEventHub; }
	[[nodiscard]] const ApplicationEventHub& GetApplicationEventHub() const override { return applicationEventHub; }

	[[nodiscard]] KeyboardEventHub& GetKeyboardEventHub() override { return keyboardEventHub; }
	[[nodiscard]] const KeyboardEventHub& GetKeyboardEventHub() const override { return keyboardEventHub; }

	[[nodiscard]] MouseEventHub& GetMouseEventHub() override { return mouseEventHub; }
	[[nodiscard]] const MouseEventHub& GetMouseEventHub() const override { return mouseEventHub; }

	[[nodiscard]] WindowEventHub& GetWindowEventHub() override { return windowEventHub; }
	[[nodiscard]] const WindowEventHub& GetWindowEventHub() const override { return windowEventHub; }

	// Hands out the base view of the extended struct: onResizeViewDispatcher is reachable only from this concrete type.
	[[nodiscard]] RenderContextEventHub& GetRenderContextEventHub() override { return renderContextEventHub; }
	[[nodiscard]] const RenderContextEventHub& GetRenderContextEventHub() const override { return renderContextEventHub; }

public:
	ApplicationEventHub applicationEventHub;
	KeyboardEventHub keyboardEventHub;
	MouseEventHub mouseEventHub;
	WindowEventHub windowEventHub;
	MetalRenderContextEventHub renderContextEventHub;

private:
	CocoaWindow* _window = nullptr;   ///< Non-owning; used to convert native mouse coordinates
};

}

#endif //CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
