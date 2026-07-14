//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: CocoaEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP

#include "Core/MainHub/Events/I_EventHubDispatcher.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

namespace NS {
	class Event;
	class Notification;
}

namespace CE::Core {

class MetalContext;
class CocoaWindow;

/**
 * @class CocoaEventHubDispatcher
 * @brief Cocoa/Metal-specific event hub
 * @details Mirrors GlfwEventHubDispatcher for the Cocoa back-end. The native view (NsViewEventDispatcher) and window
 *			(NsWindowEventDispatcher) unicast dispatchers are bound to the Receive* methods (see
 *			CocoaApplication::SetEventHubDispatcher). Each Receive* method translates the raw AppKit event into the engine's
 *			concrete event type and multicast-dispatches it to every subscriber (ImGui layer, error logger, ...).
 *
 *			Mouse coordinates arrive in AppKit's bottom-left window space; converting them to the engine's top-left space
 *			needs the render view and window, so the hub is given non-owning pointers to the MetalContext and CocoaWindow.
 */
class CocoaEventHubDispatcher: public I_EventHubDispatcher {
public:
	struct CocoaApplicationEventHub {
		Utility::MulticastDispatcher<Events::WindowResizeEvent&> onResizeMulticastDispatcher;
		Utility::MulticastDispatcher<Events::WindowCloseEvent&> onCloseMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppTickEvent&> onTickMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppUpdateEvent&> onUpdateMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppRenderEvent&> onRenderMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppErrorEvent&> onErrorMulticastDispatcher;
	};

	struct CocoaKeyboardEventHub {
		Utility::MulticastDispatcher<Events::KeyPressedEvent&> onPressedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::KeyReleasedEvent&> onReleasedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::KeyTypedEvent&> onTypedMulticastDispatcher;
	};

	struct CocoaMouseEventHub {
		Utility::MulticastDispatcher<Events::MouseMovedEvent&> onMovedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseButtonPressedEvent&> onButtonPressedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseButtonReleasedEvent&> onButtonReleasedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseDraggedEvent&> onDraggedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseWheelScrolledEvent&> onWheelScrolledMulticastDispatcher;
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
#pragma region ApplicationEvent
	void DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) override;
	void DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) override;

	void DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) override;
	void DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) override;
	void DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) override;
	void DispatchAppErrorEvent(Events::AppErrorEvent& appErrorEvent) override;
#pragma endregion

#pragma region KeyboardEvent
	void DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) override;
	void DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) override;
	void DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) override;
#pragma endregion

#pragma region MouseEvent
	void DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) override;
	void DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) override;
	void DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) override;
	void DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) override;
	void DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) override;
#pragma endregion

public:
	// Bound to the native view/window unicast dispatchers; translate the raw AppKit event and multicast the engine event.
	void ReceiveWindowWillCloseEvent(const NS::Notification* notification);
	void ReceiveWindowResizeEvent(unsigned int width, unsigned int height);
	void ReceiveAppErrorEvent(int errorCode, const char* description);

	void ReceiveKeyDownEvent(const NS::Event* event);
	void ReceiveKeyUpEvent(const NS::Event* event);

	void ReceiveMouseButtonDownEvent(const NS::Event* event);
	void ReceiveMouseButtonUpEvent(const NS::Event* event);
	void ReceiveMouseDraggedEvent(const NS::Event* event);
	void ReceiveMouseMovedEvent(const NS::Event* event);
	void ReceiveScrollWheelEvent(const NS::Event* event);

public:
	CocoaApplicationEventHub cocoaApplicationEventHub;
	CocoaKeyboardEventHub cocoaKeyboardEventHub;
	CocoaMouseEventHub cocoaMouseEventHub;

private:
	MetalContext* _context = nullptr; ///< Non-owning; used to convert native mouse coordinates
	CocoaWindow* _window = nullptr;   ///< Non-owning; used to convert native mouse coordinates
};

}

#endif //CE_CORE_MAINHUB_EVENTS_COCOAEVENTHUBDISPATCHER_HPP
