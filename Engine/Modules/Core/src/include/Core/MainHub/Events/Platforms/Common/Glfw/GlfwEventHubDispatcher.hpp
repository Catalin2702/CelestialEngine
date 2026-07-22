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
		Utility::MulticastDispatcher<Events::WindowFocusEvent&> onFocusMulticastDispatcher;	///< Fired on GLFW focus callback; input state resets held keys on focus loss
	};

	struct OpenGlRenderContextEventHub {
		Utility::MulticastDispatcher<Events::VSyncEvent&> onChangeVSyncDispatcher;
	};

public:
	GlfwEventHubDispatcher() = default;
	~GlfwEventHubDispatcher() override = default;

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

#pragma region DispatchWindowEvent
	/**
	 * @brief Multicasts the window resize event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) override;
	/**
	 * @brief Multicasts the window close event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) override;
	/**
	 * @brief Multicasts the window error event to every subscriber of the matching hub dispatcher
	 */
	void DispatchWindowErrorEvent(Events::ErrorEvent&) override;
	/**
	 * @brief Multicasts a window focus change to every subscriber of the window focus hub
	 */
	void DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) override;
#pragma endregion

#pragma region DispatchRenderContextEvent
	/**
	 * @brief Multicasts the render context change v sync event to every subscriber of the matching hub dispatcher
	 */
	void DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& VSyncChangeEvent) override;
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
	 * @brief Translates the raw key callback into the engine event and dispatches it through the hub
	 */
	void ReceiveKeyEvent(int key, int action, int scancode, int mods);
	/**
	 * @brief Translates the raw char callback into the engine event and dispatches it through the hub
	 */
	void ReceiveCharEvent(unsigned int codepoint);
#pragma endregion

#pragma region ReceiveMouseEvent
	/**
	 * @brief Translates the raw mouse button callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseButtonEvent(int button, int action, int mods);
	/**
	 * @brief Translates the raw mouse position callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMousePositionEvent(double xPos, double yPos);
	/**
	 * @brief Translates the raw mouse dragged callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseDraggedEvent(int button, int action, int mods, double xPos, double yPos);
	/**
	 * @brief Translates the raw mouse wheel scroll callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseWheelScrollEvent(double xOffset, double yOffset);
#pragma endregion

#pragma region ReceiveRenderContextEvent
	/**
	 * @brief Translates the raw context change v sync callback into the engine event and dispatches it through the hub
	 */
	void ReceiveContextChangeVSyncEvent(bool state);
#pragma endregion

#pragma region ReceiveWindowEvent
	/**
	 * @brief Translates the raw window resize callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowResizeEvent(int width, int height);
	/**
	 * @brief Translates the raw window close callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowCloseEvent();
	/**
	 * @brief Translates the raw window error callback into the engine event and dispatches it through the hub
	 */
	void ReceiveWindowErrorEvent(int errorCode, const char* description);
	/**
	 * @brief Translates the raw GLFW focus callback value into a WindowFocusEvent
	 * @param focused GLFW_TRUE when the window gained input focus, GLFW_FALSE when it lost it
	 * @details Focus loss is what lets the input state release held keys: their key-up callbacks would otherwise be
	 *			delivered to another window and the keys would remain stuck as "down".
	 */
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
