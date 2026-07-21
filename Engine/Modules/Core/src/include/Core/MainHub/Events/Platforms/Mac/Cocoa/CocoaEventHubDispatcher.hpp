//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: CocoaEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
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

class CocoaEventHubDispatcher:
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
	};

	struct MetalRenderContextEventHub {
		MulticastDispatcher<Events::VSyncChangeEvent&> onChangeVSyncDispatcher;
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
	void DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) override;
	void DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) override;
	void DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) override;
	void DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) override;
#pragma endregion

#pragma region DispatchKeyboardEvent
	void DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) override;
	void DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) override;
	void DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) override;
#pragma endregion

#pragma region DispatchMouseEvent
	void DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) override;
	void DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) override;
	void DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) override;
	void DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) override;
	void DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) override;
#pragma endregion

#pragma region DispatchRenderContextEvent
	void DispatchRenderContextChangeVSyncEvent(Events::VSyncChangeEvent& VSyncChangeEvent) override;
	void DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent);
#pragma endregion

#pragma region DispatchWindowEvent
	void DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) override;
	void DispatchWindowErrorEvent(Events::ErrorEvent&) override;
	void DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) override;
#pragma endregion

public:
#pragma region ReceiveApplicationEvent
	void ReceiveAppErrorEvent(int errorCode, const char* description);
	void ReceiveAppRenderEvent();
	void ReceiveAppTickEvent();
	void ReceiveAppUpdateEvent();
#pragma endregion

#pragma region ReceiveKeyboardEvent
	void ReceiveKeyDownEvent(const NS::Event* event);
	void ReceiveKeyUpEvent(const NS::Event* event);
#pragma endregion

#pragma region ReceiveMouseEvent
	void ReceiveMouseButtonDownEvent(const NS::Event* event);
	void ReceiveMouseButtonUpEvent(const NS::Event* event);
	void ReceiveMouseDraggedEvent(const NS::Event* event);
	void ReceiveMouseMovedEvent(const NS::Event* event);
	void ReceiveScrollWheelEvent(const NS::Event* event);
#pragma endregion

#pragma region ReceiveRenderContextEvent
	void ReceiveContextChangeVSyncEvent(bool state);
	void ReceiveContextResizeViewEvent(double width, double height);
#pragma endregion

#pragma region ReceiveWindowEvent
	void ReceiveWindowWillCloseEvent(const NS::Notification* notification);
	void ReceiveWindowErrorEvent(int errorCode, const char* description);
	void ReceiveWindowResizeEvent(unsigned int width, unsigned int height);
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
