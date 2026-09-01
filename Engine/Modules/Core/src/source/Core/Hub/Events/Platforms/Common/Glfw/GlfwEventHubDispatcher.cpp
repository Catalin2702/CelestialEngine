//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: GlfwEventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <GLFW/glfw3.h>

namespace CE::Core {

void GlfwEventHubDispatcher::DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) {
	applicationEventHub.onTickMulticastDispatcher.Dispatch(appTickEvent);
}

void GlfwEventHubDispatcher::DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) {
	applicationEventHub.onUpdateMulticastDispatcher.Dispatch(appUpdateEvent);
}

void GlfwEventHubDispatcher::DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) {
	applicationEventHub.onRenderMulticastDispatcher.Dispatch(appRenderEvent);
}

void GlfwEventHubDispatcher::DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) {
	applicationEventHub.onErrorMulticastDispatcher.Dispatch(appErrorEvent);
}

void GlfwEventHubDispatcher::DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) {
	keyboardEventHub.onPressedMulticastDispatcher.Dispatch(keyPressedEvent);
}

void GlfwEventHubDispatcher::DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) {
	keyboardEventHub.onReleasedMulticastDispatcher.Dispatch(keyReleasedEvent);
}

void GlfwEventHubDispatcher::DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) {
	keyboardEventHub.onTypedMulticastDispatcher.Dispatch(keyTypedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) {
	mouseEventHub.onMovedMulticastDispatcher.Dispatch(mouseMovedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) {
	mouseEventHub.onButtonPressedMulticastDispatcher.Dispatch(mouseButtonPressedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) {
	mouseEventHub.onButtonReleasedMulticastDispatcher.Dispatch(mouseButtonReleasedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) {
	mouseEventHub.onDraggedMulticastDispatcher.Dispatch(mouseDraggedEvent);
}

void GlfwEventHubDispatcher::DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) {
	mouseEventHub.onWheelScrolledMulticastDispatcher.Dispatch(mouseWheelScrolledEvent);
}

void GlfwEventHubDispatcher::DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) {
	windowEventHub.onResizeMulticastDispatcher.Dispatch(windowResizeEvent);
}

void GlfwEventHubDispatcher::DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) {
	windowEventHub.onCloseMulticastDispatcher.Dispatch(windowCloseEvent);
}

void GlfwEventHubDispatcher::DispatchWindowErrorEvent(Events::ErrorEvent& errorEvent) {
	windowEventHub.onErrorMulticastDispatcher.Dispatch(errorEvent);
}

void GlfwEventHubDispatcher::DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) {
	windowEventHub.onFocusMulticastDispatcher.Dispatch(windowFocusEvent);
}

void GlfwEventHubDispatcher::DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& VSyncChangeEvent) {
	renderContextEventHub.onChangeVSyncDispatcher.Dispatch(VSyncChangeEvent);
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
	Events::ErrorEvent appErrorEvent{errorCode, description};
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

void GlfwEventHubDispatcher::ReceiveMousePositionEvent(const f64 xPos, const f64 yPos) {
	Events::MouseMovedEvent mouseMovedEvent{static_cast<f32>(xPos), static_cast<f32>(yPos)};
	DispatchMouseMovedEvent(mouseMovedEvent);
}

void GlfwEventHubDispatcher::ReceiveMouseDraggedEvent(const int button, const int, const int, const f64 xPos, const f64 yPos) {
	Events::MouseDraggedEvent mouseDraggedEvent{Types::MouseButtonKeyCodeFromGlfw(button), static_cast<f32>(xPos), static_cast<f32>(yPos)};
	DispatchMouseDraggedEvent(mouseDraggedEvent);
}

void GlfwEventHubDispatcher::ReceiveMouseWheelScrollEvent(const f64 xOffset, const f64 yOffset) {
	Events::MouseWheelScrolledEvent mouseWheelScrolledEvent{static_cast<f32>(xOffset), static_cast<f32>(yOffset)};
	DispatchMouseWheelScrolledEvent(mouseWheelScrolledEvent);
}

void GlfwEventHubDispatcher::ReceiveContextChangeVSyncEvent(const bool state) {
	Events::VSyncEvent VSyncChangeEvent{state};
	DispatchRenderContextChangeVSyncEvent(VSyncChangeEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowResizeEvent(const int width, const int height) {
	Events::WindowResizeEvent windowResizeEvent{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
	DispatchWindowResizeEvent(windowResizeEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowCloseEvent() {
	Events::WindowCloseEvent windowCloseEvent;
	DispatchWindowCloseEvent(windowCloseEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowErrorEvent(const int errorCode, const char* description) {
	Events::ErrorEvent errorEvent{errorCode, description};
	DispatchWindowErrorEvent(errorEvent);
}

void GlfwEventHubDispatcher::ReceiveWindowFocusEvent(const int focused) {
	Events::WindowFocusEvent windowFocusEvent{focused == GLFW_TRUE};
	DispatchWindowFocusEvent(windowFocusEvent);
}

}
