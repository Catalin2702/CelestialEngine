//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: MouseButtonCode.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-09
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#include "Types/KeyCode/MouseButtonCode.hpp"

#include <imgui.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <GLFW/glfw3.h>


namespace CE::KeyCode {

const char* ToString(const MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case MouseButtonCode::Left: return "Left";
		case MouseButtonCode::Right: return "Right";
		case MouseButtonCode::Middle: return "Middle";
		case MouseButtonCode::Button4: return "Button4";
		case MouseButtonCode::Button5: return "Button5";
		case MouseButtonCode::Button6: return "Button6";
		case MouseButtonCode::Button7: return "Button7";
		case MouseButtonCode::Button8: return "Button8";

		case MouseButtonCode::WheelX: return "WheelX";
		case MouseButtonCode::WheelY: return "WheelY";

		case MouseButtonCode::Unknown:
		default: return "Unknown";
	}
}

MouseButtonCode MouseButtonKeyCodeFromGlfw(const int buttonCode) {
	switch (buttonCode) {
		case GLFW_MOUSE_BUTTON_LEFT: return MouseButtonCode::Left;
		case GLFW_MOUSE_BUTTON_RIGHT: return MouseButtonCode::Right;
		case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButtonCode::Middle;
		case GLFW_MOUSE_BUTTON_4: return MouseButtonCode::Button4;
		case GLFW_MOUSE_BUTTON_5: return MouseButtonCode::Button5;
		case GLFW_MOUSE_BUTTON_6: return MouseButtonCode::Button6;
		case GLFW_MOUSE_BUTTON_7: return MouseButtonCode::Button7;
		case GLFW_MOUSE_BUTTON_8: return MouseButtonCode::Button8;

		default: return MouseButtonCode::Unknown;
	}
}

int GlfwKeyCodeFromMouseButton(const MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case MouseButtonCode::Left: return GLFW_MOUSE_BUTTON_LEFT;
		case MouseButtonCode::Right: return GLFW_MOUSE_BUTTON_RIGHT;
		case MouseButtonCode::Middle:
		case MouseButtonCode::WheelX:
		case MouseButtonCode::WheelY: return GLFW_MOUSE_BUTTON_MIDDLE;
		case MouseButtonCode::Button4: return GLFW_MOUSE_BUTTON_4;
		case MouseButtonCode::Button5: return GLFW_MOUSE_BUTTON_5;
		case MouseButtonCode::Button6: return GLFW_MOUSE_BUTTON_6;
		case MouseButtonCode::Button7: return GLFW_MOUSE_BUTTON_7;
		case MouseButtonCode::Button8: return GLFW_MOUSE_BUTTON_8;

		case MouseButtonCode::Unknown:
		default: return GLFW_KEY_UNKNOWN;
	}
}

int ImGuiKeyFromMouseButton(const MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case MouseButtonCode::Left: return 0;
		case MouseButtonCode::Right: return 1;
		case MouseButtonCode::Middle:
		case MouseButtonCode::WheelX:
		case MouseButtonCode::WheelY: return 2;

		default: return 5; // No valid mapping for this button
	}
}

MouseButtonCode MouseButtonKeyCodeFromCocoa(const int buttonNumber) {
	switch (buttonNumber) {
		case 0: return KeyCode::MouseButtonCode::Left;
		case 1: return KeyCode::MouseButtonCode::Right;
		case 2: return KeyCode::MouseButtonCode::Middle;
		default: return KeyCode::MouseButtonCode::Unknown;
	}
}

int CocoaButtonNumberFromMouseButton(const MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case MouseButtonCode::Left: return 0;
		case MouseButtonCode::Right: return 1;
		case MouseButtonCode::Middle:
		case MouseButtonCode::WheelX:
		case MouseButtonCode::WheelY: return 2;

		case MouseButtonCode::Unknown:
		default: return -1;
	}
}

}
