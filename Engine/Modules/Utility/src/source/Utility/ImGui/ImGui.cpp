//
// Module: CelestialEngine/Engine/Modules/Utility/ImGui
// File: ImGui.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "Utility/ImGui/ImGui.hpp"

#if CE_WINDOW_GLFW
#include <GLFW/glfw3.h>
#endif

#include <string>

namespace CE::Utility {

ImGuiKey ImGuiKeyFromKeyboard(const Types::KeyboardKeyCode keycode) {
	switch (keycode) {
		case Types::KeyboardKeyCode::A: return ImGuiKey_A;
		case Types::KeyboardKeyCode::B: return ImGuiKey_B;
		case Types::KeyboardKeyCode::C: return ImGuiKey_C;
		case Types::KeyboardKeyCode::D: return ImGuiKey_D;
		case Types::KeyboardKeyCode::E: return ImGuiKey_E;
		case Types::KeyboardKeyCode::F: return ImGuiKey_F;
		case Types::KeyboardKeyCode::G: return ImGuiKey_G;
		case Types::KeyboardKeyCode::H: return ImGuiKey_H;
		case Types::KeyboardKeyCode::I: return ImGuiKey_I;
		case Types::KeyboardKeyCode::J: return ImGuiKey_J;
		case Types::KeyboardKeyCode::K: return ImGuiKey_K;
		case Types::KeyboardKeyCode::L: return ImGuiKey_L;
		case Types::KeyboardKeyCode::M: return ImGuiKey_M;
		case Types::KeyboardKeyCode::N: return ImGuiKey_N;
		case Types::KeyboardKeyCode::O: return ImGuiKey_O;
		case Types::KeyboardKeyCode::P: return ImGuiKey_P;
		case Types::KeyboardKeyCode::Q: return ImGuiKey_Q;
		case Types::KeyboardKeyCode::R: return ImGuiKey_R;
		case Types::KeyboardKeyCode::S: return ImGuiKey_S;
		case Types::KeyboardKeyCode::T: return ImGuiKey_T;
		case Types::KeyboardKeyCode::U: return ImGuiKey_U;
		case Types::KeyboardKeyCode::V: return ImGuiKey_V;
		case Types::KeyboardKeyCode::W: return ImGuiKey_W;
		case Types::KeyboardKeyCode::X: return ImGuiKey_X;
		case Types::KeyboardKeyCode::Y: return ImGuiKey_Y;
		case Types::KeyboardKeyCode::Z: return ImGuiKey_Z;

		case Types::KeyboardKeyCode::Num0: return ImGuiKey_0;
		case Types::KeyboardKeyCode::Num1: return ImGuiKey_1;
		case Types::KeyboardKeyCode::Num2: return ImGuiKey_2;
		case Types::KeyboardKeyCode::Num3: return ImGuiKey_3;
		case Types::KeyboardKeyCode::Num4: return ImGuiKey_4;
		case Types::KeyboardKeyCode::Num5: return ImGuiKey_5;
		case Types::KeyboardKeyCode::Num6: return ImGuiKey_6;
		case Types::KeyboardKeyCode::Num7: return ImGuiKey_7;
		case Types::KeyboardKeyCode::Num8: return ImGuiKey_8;
		case Types::KeyboardKeyCode::Num9: return ImGuiKey_9;

		case Types::KeyboardKeyCode::F1: return ImGuiKey_F1;
		case Types::KeyboardKeyCode::F2: return ImGuiKey_F2;
		case Types::KeyboardKeyCode::F3: return ImGuiKey_F3;
		case Types::KeyboardKeyCode::F4: return ImGuiKey_F4;
		case Types::KeyboardKeyCode::F5: return ImGuiKey_F5;
		case Types::KeyboardKeyCode::F6: return ImGuiKey_F6;
		case Types::KeyboardKeyCode::F7: return ImGuiKey_F7;
		case Types::KeyboardKeyCode::F8: return ImGuiKey_F8;
		case Types::KeyboardKeyCode::F9: return ImGuiKey_F9;
		case Types::KeyboardKeyCode::F10: return ImGuiKey_F10;
		case Types::KeyboardKeyCode::F11: return ImGuiKey_F11;
		case Types::KeyboardKeyCode::F12: return ImGuiKey_F12;

		case Types::KeyboardKeyCode::Left: return ImGuiKey_LeftArrow;
		case Types::KeyboardKeyCode::Right: return ImGuiKey_RightArrow;
		case Types::KeyboardKeyCode::Up: return ImGuiKey_UpArrow;
		case Types::KeyboardKeyCode::Down: return ImGuiKey_DownArrow;

		case Types::KeyboardKeyCode::Space: return ImGuiKey_Space;
		case Types::KeyboardKeyCode::Enter: return ImGuiKey_Enter;
		case Types::KeyboardKeyCode::Escape: return ImGuiKey_Escape;
		case Types::KeyboardKeyCode::Tab: return ImGuiKey_Tab;
		case Types::KeyboardKeyCode::Backspace: return ImGuiKey_Backspace;
		case Types::KeyboardKeyCode::LeftShift: return ImGuiKey_LeftShift;
		case Types::KeyboardKeyCode::RightShift: return ImGuiKey_RightShift;
		case Types::KeyboardKeyCode::LeftAlt: return ImGuiKey_LeftAlt;
		case Types::KeyboardKeyCode::RightAlt: return ImGuiKey_RightAlt;

#if CE_PLATFORM_MACOS
		case Types::KeyboardKeyCode::LeftControl: return ImGuiKey_LeftSuper;
		case Types::KeyboardKeyCode::RightControl: return ImGuiKey_RightSuper;

		case Types::KeyboardKeyCode::LeftSuper: return ImGuiKey_LeftCtrl;
		case Types::KeyboardKeyCode::RightSuper: return ImGuiKey_RightCtrl;
#else
		case Types::KeyboardKeyCode::LeftControl: return ImGuiKey_LeftCtrl;
		case Types::KeyboardKeyCode::RightControl: return ImGuiKey_RightCtrl;

		case Types::KeyboardKeyCode::LeftSuper: return ImGuiKey_LeftSuper;
		case Types::KeyboardKeyCode::RightSuper: return ImGuiKey_RightSuper;
#endif

		case Types::KeyboardKeyCode::Unknown:
		default: return ImGuiKey_None;
	}
}

int ImGuiKeyFromMouseButton(const Types::MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case Types::MouseButtonCode::Left: return 0;
		case Types::MouseButtonCode::Right: return 1;
		case Types::MouseButtonCode::Middle:
		case Types::MouseButtonCode::WheelX:
		case Types::MouseButtonCode::WheelY: return 2;

		default: return 5; // No valid mapping for this button
	}
}

void SetImGuiIniFile(ImGuiIO& io, const std::filesystem::path& iniPath) {
	// Never reassigned once initialised: ImGui keeps the raw pointer, so a second call with a
	// different name must not move the storage out from under a live context
	static const std::string iniFilePath = iniPath.string();

	io.IniFilename = iniFilePath.empty() ? nullptr : iniFilePath.c_str();
}

// Only with a GLFW window (CE_WINDOW_GLFW, derived from CE_RENDER_BACKENDS).
#if CE_WINDOW_GLFW
ImGuiKey GlfwKeyToImGuiKey(const int key) {
	switch (key) {
		case GLFW_KEY_TAB: return ImGuiKey_Tab;
		case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
		case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
		case GLFW_KEY_UP: return ImGuiKey_UpArrow;
		case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
		case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
		case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
		case GLFW_KEY_HOME: return ImGuiKey_Home;
		case GLFW_KEY_END: return ImGuiKey_End;
		case GLFW_KEY_INSERT: return ImGuiKey_Insert;
		case GLFW_KEY_DELETE: return ImGuiKey_Delete;
		case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
		case GLFW_KEY_SPACE: return ImGuiKey_Space;
		case GLFW_KEY_ENTER: return ImGuiKey_Enter;
		case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
		case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
		case GLFW_KEY_COMMA: return ImGuiKey_Comma;
		case GLFW_KEY_MINUS: return ImGuiKey_Minus;
		case GLFW_KEY_PERIOD: return ImGuiKey_Period;
		case GLFW_KEY_SLASH: return ImGuiKey_Slash;
		case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
		case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
		case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
		case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
		case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
		case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
		case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
		case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
		case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
		case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
		case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
		case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
		case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
		case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
		case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
		case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
		case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
		case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
		case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
		case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
		case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
		case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
		case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
		case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
		case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
		case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
		case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
		case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
		case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
		case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
		case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
		case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
		case GLFW_KEY_MENU: return ImGuiKey_Menu;
		case GLFW_KEY_0: return ImGuiKey_0;
		case GLFW_KEY_1: return ImGuiKey_1;
		case GLFW_KEY_2: return ImGuiKey_2;
		case GLFW_KEY_3: return ImGuiKey_3;
		case GLFW_KEY_4: return ImGuiKey_4;
		case GLFW_KEY_5: return ImGuiKey_5;
		case GLFW_KEY_6: return ImGuiKey_6;
		case GLFW_KEY_7: return ImGuiKey_7;
		case GLFW_KEY_8: return ImGuiKey_8;
		case GLFW_KEY_9: return ImGuiKey_9;
		case GLFW_KEY_A: return ImGuiKey_A;
		case GLFW_KEY_B: return ImGuiKey_B;
		case GLFW_KEY_C: return ImGuiKey_C;
		case GLFW_KEY_D: return ImGuiKey_D;
		case GLFW_KEY_E: return ImGuiKey_E;
		case GLFW_KEY_F: return ImGuiKey_F;
		case GLFW_KEY_G: return ImGuiKey_G;
		case GLFW_KEY_H: return ImGuiKey_H;
		case GLFW_KEY_I: return ImGuiKey_I;
		case GLFW_KEY_J: return ImGuiKey_J;
		case GLFW_KEY_K: return ImGuiKey_K;
		case GLFW_KEY_L: return ImGuiKey_L;
		case GLFW_KEY_M: return ImGuiKey_M;
		case GLFW_KEY_N: return ImGuiKey_N;
		case GLFW_KEY_O: return ImGuiKey_O;
		case GLFW_KEY_P: return ImGuiKey_P;
		case GLFW_KEY_Q: return ImGuiKey_Q;
		case GLFW_KEY_R: return ImGuiKey_R;
		case GLFW_KEY_S: return ImGuiKey_S;
		case GLFW_KEY_T: return ImGuiKey_T;
		case GLFW_KEY_U: return ImGuiKey_U;
		case GLFW_KEY_V: return ImGuiKey_V;
		case GLFW_KEY_W: return ImGuiKey_W;
		case GLFW_KEY_X: return ImGuiKey_X;
		case GLFW_KEY_Y: return ImGuiKey_Y;
		case GLFW_KEY_Z: return ImGuiKey_Z;
		case GLFW_KEY_F1: return ImGuiKey_F1;
		case GLFW_KEY_F2: return ImGuiKey_F2;
		case GLFW_KEY_F3: return ImGuiKey_F3;
		case GLFW_KEY_F4: return ImGuiKey_F4;
		case GLFW_KEY_F5: return ImGuiKey_F5;
		case GLFW_KEY_F6: return ImGuiKey_F6;
		case GLFW_KEY_F7: return ImGuiKey_F7;
		case GLFW_KEY_F8: return ImGuiKey_F8;
		case GLFW_KEY_F9: return ImGuiKey_F9;
		case GLFW_KEY_F10: return ImGuiKey_F10;
		case GLFW_KEY_F11: return ImGuiKey_F11;
		case GLFW_KEY_F12: return ImGuiKey_F12;
		default: return ImGuiKey_None;
	}
}
#endif

}
