//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: GlfwEventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/MainHub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <GLFW/glfw3.h>

namespace CE::Core {

void GlfwEventHubDispatcher::DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) {
	glfwApplicationEventHub.onResizeMulticastDispatcher.Dispatch(windowResizeEvent);
}

void GlfwEventHubDispatcher::DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) {
	glfwApplicationEventHub.onCloseMulticastDispatcher.Dispatch(windowCloseEvent);
}

void GlfwEventHubDispatcher::DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) {
	glfwApplicationEventHub.onTickMulticastDispatcher.Dispatch(appTickEvent);
}

void GlfwEventHubDispatcher::DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) {
	glfwApplicationEventHub.onUpdateMulticastDispatcher.Dispatch(appUpdateEvent);
}

void GlfwEventHubDispatcher::DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) {
	glfwApplicationEventHub.onRenderMulticastDispatcher.Dispatch(appRenderEvent);
}

void GlfwEventHubDispatcher::DispatchAppErrorEvent(Events::AppErrorEvent& appErrorEvent) {
	glfwApplicationEventHub.onErrorMulticastDispatcher.Dispatch(appErrorEvent);
}

void GlfwEventHubDispatcher::DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) {
	glfwKeyboardEventHub.onPressedMulticastDispatcher.Dispatch(keyPressedEvent);
}

void GlfwEventHubDispatcher::DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) {
	glfwKeyboardEventHub.onReleasedMulticastDispatcher.Dispatch(keyReleasedEvent);
}

void GlfwEventHubDispatcher::DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) {
	glfwKeyboardEventHub.onTypedMulticastDispatcher.Dispatch(keyTypedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) {
	glfwMouseEventHub.onMovedMulticastDispatcher.Dispatch(mouseMovedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) {
	glfwMouseEventHub.onButtonPressedMulticastDispatcher.Dispatch(mouseButtonPressedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) {
	glfwMouseEventHub.onButtonReleasedMulticastDispatcher.Dispatch(mouseButtonReleasedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) {
	glfwMouseEventHub.onDraggedMulticastDispatcher.Dispatch(mouseDraggedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) {
	glfwMouseEventHub.onWheelScrolledMulticastDispatcher.Dispatch(mouseWheelScrolledEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowResizeEvent(const int width, const int height) {
	Events::WindowResizeEvent windowResizeEvent{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
	DispatchWindowResizeEvent(windowResizeEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowCloseEvent() {
	Events::WindowCloseEvent windowCloseEvent;
	DispatchWindowCloseEvent(windowCloseEvent);
}

void GlfwEventHubDispatcher::ReceiveAppTickEvent() {
	Events::AppTickEvent appTickEvent;
	DispatchAppTickEvent(appTickEvent);
}

void GlfwEventHubDispatcher::ReceiveAppUpdateEvent() {
	Events::AppUpdateEvent appUpdateEvent;
	DispatchAppUpdateEvent(appUpdateEvent);
}

void GlfwEventHubDispatcher::ReceiveAppRenderEvent() {
	Events::AppRenderEvent appRenderEvent;
	DispatchAppRenderEvent(appRenderEvent);
}

void GlfwEventHubDispatcher::ReceiveAppErrorEvent(const int errorCode, const char* description) {
	Events::AppErrorEvent appErrorEvent{errorCode, description};
	DispatchAppErrorEvent(appErrorEvent);
}

void GlfwEventHubDispatcher::ReceiveKeyEvent(const int key, const int action, const int, const int) {
	switch (action) {
		case GLFW_PRESS: {
			Events::KeyPressedEvent keyPressedEvent{Types::KeyboardKeyCodeFromGlfw(key), 0};
			DispatchKeyPressedEvent(keyPressedEvent);
			break;
		}
		case GLFW_RELEASE: {
			Events::KeyReleasedEvent keyReleasedEvent{Types::KeyboardKeyCodeFromGlfw(key)};
			DispatchKeyReleasedEvent(keyReleasedEvent);
			break;
		}
		case GLFW_REPEAT: {
			Events::KeyPressedEvent keyPressedEvent{Types::KeyboardKeyCodeFromGlfw(key), 1};
			DispatchKeyPressedEvent(keyPressedEvent);
		}
		default:
			break;
	}
}

void GlfwEventHubDispatcher::ReceiveCharEvent(const unsigned int codepoint) {
	Events::KeyTypedEvent keyTypedEvent{codepoint};
	DispatchKeyTypedEvent(keyTypedEvent);
}

void GlfwEventHubDispatcher::ReceiveMouseButtonEvent(const int button, const int action, const int) {
	switch (action) {
		case GLFW_PRESS: {
			Events::MouseButtonPressedEvent mouseButtonPressedEvent{Types::MouseButtonKeyCodeFromGlfw(button)};
			DispatchMouseButtonPressedEvent(mouseButtonPressedEvent);
			break;
		}
		case GLFW_RELEASE: {
			Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{Types::MouseButtonKeyCodeFromGlfw(button)};
			DispatchMouseButtonReleasedEvent(mouseButtonReleasedEvent);
			break;
		}
		default:
			break;
	}
}

void GlfwEventHubDispatcher::ReceiveMousePositionEvent(const double xPos, const double yPos) {
	Events::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
	DispatchMouseMovedEvent(mouseMovedEvent);
}

void GlfwEventHubDispatcher::ReceiveMouseDraggedEvent(const int button, const int, const int, const double xPos, const double yPos) {
	Events::MouseDraggedEvent mouseDraggedEvent{Types::MouseButtonKeyCodeFromGlfw(button), static_cast<float>(xPos), static_cast<float>(yPos)};
	DispatchMouseDraggedEvent(mouseDraggedEvent);
}

void GlfwEventHubDispatcher::ReceiveMouseWheelScrollEvent(const double xOffset, const double yOffset) {
	Events::MouseWheelScrolledEvent mouseWheelScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
	DispatchMouseWheelScrolledEvent(mouseWheelScrolledEvent);
}

}
