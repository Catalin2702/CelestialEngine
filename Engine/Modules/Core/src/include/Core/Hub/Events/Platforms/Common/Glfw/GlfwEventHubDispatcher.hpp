//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: GlfwEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP

#include "Core/Hub/Events/I_EventHubDispatcher.hpp"


namespace CE::Core {

/**
 * @class GlfwEventHubDispatcher
 * @brief The event hub fed by the GLFW callbacks
 * @details Holds the shared channels and translates GLFW's raw C callback arguments into engine events. Everything
 *			backend-specific stops at the Receive* methods; what leaves through the hub is the same on every platform.
 */
class CE_CORE_API GlfwEventHubDispatcher final: public I_EventHubDispatcherBase<Types::WindowApi::GLFW> {
public:
	// The user-declared destructor suppresses the implicit move constructor and move assignment, so a move of an owner
	// holding this dispatcher by value would silently fall back to a copy. They are re-declared here (memberwise: every
	// member is a nothrow-movable dispatcher). Copies stay deleted: this is a polymorphic type, and duplicating a hub
	// would deliver every event twice.
	GlfwEventHubDispatcher() = default;
	GlfwEventHubDispatcher(const GlfwEventHubDispatcher&) = delete;
	GlfwEventHubDispatcher(GlfwEventHubDispatcher&&) noexcept = default;
	~GlfwEventHubDispatcher() override = default;

	GlfwEventHubDispatcher& operator = (const GlfwEventHubDispatcher&) = delete;
	GlfwEventHubDispatcher& operator = (GlfwEventHubDispatcher&&) noexcept = default;

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
	void ReceiveMousePositionEvent(f64 xPos, f64 yPos);
	/**
	 * @brief Translates the raw mouse dragged callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseDraggedEvent(int button, int action, int mods, f64 xPos, f64 yPos);
	/**
	 * @brief Translates the raw mouse wheel scroll callback into the engine event and dispatches it through the hub
	 */
	void ReceiveMouseWheelScrollEvent(f64 xOffset, f64 yOffset);
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
	[[nodiscard]] ApplicationEventHub& GetApplicationEventHub() override { return applicationEventHub; }
	[[nodiscard]] const ApplicationEventHub& GetApplicationEventHub() const override { return applicationEventHub; }

	[[nodiscard]] KeyboardEventHub& GetKeyboardEventHub() override { return keyboardEventHub; }
	[[nodiscard]] const KeyboardEventHub& GetKeyboardEventHub() const override { return keyboardEventHub; }

	[[nodiscard]] MouseEventHub& GetMouseEventHub() override { return mouseEventHub; }
	[[nodiscard]] const MouseEventHub& GetMouseEventHub() const override { return mouseEventHub; }

	[[nodiscard]] WindowEventHub& GetWindowEventHub() override { return windowEventHub; }
	[[nodiscard]] const WindowEventHub& GetWindowEventHub() const override { return windowEventHub; }

	[[nodiscard]] RenderContextEventHub& GetRenderContextEventHub() override { return renderContextEventHub; }
	[[nodiscard]] const RenderContextEventHub& GetRenderContextEventHub() const override { return renderContextEventHub; }

public:
	ApplicationEventHub applicationEventHub;
	KeyboardEventHub keyboardEventHub;
	MouseEventHub mouseEventHub;
	WindowEventHub windowEventHub;
	RenderContextEventHub renderContextEventHub;
};

}

#endif //CE_CORE_MAINHUB_EVENTS_GLFWEVENTHUBDISPATCHER_HPP
