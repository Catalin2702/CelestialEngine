//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: GlfwEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP

#include "Core/MainHub/Events/I_EventHubDispatcher.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

namespace CE::Core {

class GlfwEventHubDispatcher: public I_EventHubDispatcher {
public:
	struct GlfwApplicationEventHub {
		Utility::MulticastDispatcher<Events::WindowResizeEvent&> onResizeMulticastDispatcher;
		Utility::MulticastDispatcher<Events::WindowCloseEvent&> onCloseMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppTickEvent&> onTickMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppUpdateEvent&> onUpdateMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppRenderEvent&> onRenderMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppErrorEvent&> onErrorMulticastDispatcher;
	};

	struct GlfwKeyboardEventHub {
		Utility::MulticastDispatcher<Events::KeyPressedEvent&> onPressedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::KeyReleasedEvent&> onReleasedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::KeyTypedEvent&> onTypedMulticastDispatcher;
	};

	struct GlfwMouseEventHub {
		Utility::MulticastDispatcher<Events::MouseMovedEvent&> onMovedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseButtonPressedEvent&> onButtonPressedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseButtonReleasedEvent&> onButtonReleasedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseDraggedEvent&> onDraggedMulticastDispatcher;
		Utility::MulticastDispatcher<Events::MouseWheelScrolledEvent&> onWheelScrolledMulticastDispatcher;
	};

public:
	GlfwEventHubDispatcher() = default;
	~GlfwEventHubDispatcher() override = default;

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
	void ReceiveWindowResizeEvent(int width, int height);
	void ReceiveWindowCloseEvent();
	void ReceiveAppTickEvent();
	void ReceiveAppUpdateEvent();
	void ReceiveAppRenderEvent();
	void ReceiveAppErrorEvent(int errorCode, const char* description);

	void ReceiveKeyEvent(int key, int action, int scancode, int mods);
	void ReceiveCharEvent(unsigned int codepoint);

	void ReceiveMouseButtonEvent(int button, int action, int mods);
	void ReceiveMousePositionEvent(double xPos, double yPos);
	void ReceiveMouseDraggedEvent(int button, int action, int mods, double xPos, double yPos);
	void ReceiveMouseWheelScrollEvent(double xOffset, double yOffset);

public:
	GlfwApplicationEventHub glfwApplicationEventHub;
	GlfwKeyboardEventHub glfwKeyboardEventHub;
	GlfwMouseEventHub glfwMouseEventHub;
};

}

#endif //CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
