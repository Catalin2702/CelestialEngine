//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: GlfwEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP

#include "Core/MainHub/Events/I_ApplicationEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_KeyboardEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_MouseEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_RenderContextEventHubDispatcher.hpp"
#include "Core/MainHub/Events/I_WindowEventHubDispatcher.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

namespace CE::Core {

class GlfwEventHubDispatcher:
	public I_ApplicationEventHubDispatcher,
	public I_KeyboardEventHubDispatcher,
	public I_MouseEventHubDispatcher,
	public I_RenderContextEventHubDispatcher,
	public I_WindowEventHubDispatcher
{
public:
	struct GlfwApplicationEventHub {
		Utility::MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppRenderEvent&> onRenderMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppTickEvent&> onTickMulticastDispatcher;
		Utility::MulticastDispatcher<Events::AppUpdateEvent&> onUpdateMulticastDispatcher;
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

	struct GlfwWindowEventHub {
		Utility::MulticastDispatcher<Events::WindowResizeEvent&> onResizeMulticastDispatcher;
		Utility::MulticastDispatcher<Events::WindowCloseEvent&> onCloseMulticastDispatcher;
		Utility::MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
		Utility::MulticastDispatcher<Events::WindowFocusEvent&> onFocusMulticastDispatcher;
	};

	struct OpenGlRenderContextEventHub {
		Utility::MulticastDispatcher<Events::VSyncEvent&> onChangeVSyncDispatcher;
	};

public:
	GlfwEventHubDispatcher() = default;
	~GlfwEventHubDispatcher() override = default;

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

#pragma region DispatchWindowEvent
	void DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) override;
	void DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) override;
	void DispatchWindowErrorEvent(Events::ErrorEvent&) override;
	void DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) override;
#pragma endregion

#pragma region DispatchRenderContextEvent
	void DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& VSyncChangeEvent) override;
#pragma endregion

public:
#pragma region ReceiveApplicationEvent
	void ReceiveAppErrorEvent(int errorCode, const char* description);
	void ReceiveAppRenderEvent();
	void ReceiveAppTickEvent();
	void ReceiveAppUpdateEvent();
#pragma endregion

#pragma region ReceiveKeyboardEvent
	void ReceiveKeyEvent(int key, int action, int scancode, int mods);
	void ReceiveCharEvent(unsigned int codepoint);
#pragma endregion

#pragma region ReceiveMouseEvent
	void ReceiveMouseButtonEvent(int button, int action, int mods);
	void ReceiveMousePositionEvent(double xPos, double yPos);
	void ReceiveMouseDraggedEvent(int button, int action, int mods, double xPos, double yPos);
	void ReceiveMouseWheelScrollEvent(double xOffset, double yOffset);
#pragma endregion

#pragma region ReceiveRenderContextEvent
	void ReceiveContextChangeVSyncEvent(bool state);
#pragma endregion

#pragma region ReceiveWindowEvent
	void ReceiveWindowResizeEvent(int width, int height);
	void ReceiveWindowCloseEvent();
	void ReceiveWindowErrorEvent(int errorCode, const char* description);
	void ReceiveWindowFocusEvent(int focused);
#pragma endregion

public:
	GlfwApplicationEventHub glfwApplicationEventHub;
	GlfwKeyboardEventHub glfwKeyboardEventHub;
	GlfwMouseEventHub glfwMouseEventHub;
	GlfwWindowEventHub glfwWindowEventHub;
	OpenGlRenderContextEventHub openGlRenderContextEventHub;
};

}

#endif //CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
