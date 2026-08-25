//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyboardKeyCode.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-09
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Types/KeyCode/KeyboardKeyCode.hpp"

#include <imgui.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <GLFW/glfw3.h>

namespace CE::Types {

const char* ToString(const KeyboardKeyCode key) {
	switch (key) {
		case KeyboardKeyCode::A: return "A";
		case KeyboardKeyCode::B: return "B";
		case KeyboardKeyCode::C: return "C";
		case KeyboardKeyCode::D: return "D";
		case KeyboardKeyCode::E: return "E";
		case KeyboardKeyCode::F: return "F";
		case KeyboardKeyCode::G: return "G";
		case KeyboardKeyCode::H: return "H";
		case KeyboardKeyCode::I: return "I";
		case KeyboardKeyCode::J: return "J";
		case KeyboardKeyCode::K: return "K";
		case KeyboardKeyCode::L: return "L";
		case KeyboardKeyCode::M: return "M";
		case KeyboardKeyCode::N: return "N";
		case KeyboardKeyCode::O: return "O";
		case KeyboardKeyCode::P: return "P";
		case KeyboardKeyCode::Q: return "Q";
		case KeyboardKeyCode::R: return "R";
		case KeyboardKeyCode::S: return "S";
		case KeyboardKeyCode::T: return "T";
		case KeyboardKeyCode::U: return "U";
		case KeyboardKeyCode::V: return "V";
		case KeyboardKeyCode::W: return "W";
		case KeyboardKeyCode::X: return "X";
		case KeyboardKeyCode::Y: return "Y";
		case KeyboardKeyCode::Z: return "Z";

		case KeyboardKeyCode::Num0: return "0";
		case KeyboardKeyCode::Num1: return "1";
		case KeyboardKeyCode::Num2: return "2";
		case KeyboardKeyCode::Num3: return "3";
		case KeyboardKeyCode::Num4: return "4";
		case KeyboardKeyCode::Num5: return "5";
		case KeyboardKeyCode::Num6: return "6";
		case KeyboardKeyCode::Num7: return "7";
		case KeyboardKeyCode::Num8: return "8";
		case KeyboardKeyCode::Num9: return "9";

		case KeyboardKeyCode::F1: return "F1";
		case KeyboardKeyCode::F2: return "F2";
		case KeyboardKeyCode::F3: return "F3";
		case KeyboardKeyCode::F4: return "F4";
		case KeyboardKeyCode::F5: return "F5";
		case KeyboardKeyCode::F6: return "F6";
		case KeyboardKeyCode::F7: return "F7";
		case KeyboardKeyCode::F8: return "F8";
		case KeyboardKeyCode::F9: return "F9";
		case KeyboardKeyCode::F10: return "F10";
		case KeyboardKeyCode::F11: return "F11";
		case KeyboardKeyCode::F12: return "F12";

		case KeyboardKeyCode::Left: return "Left Arrow";
		case KeyboardKeyCode::Right: return "Right Arrow";
		case KeyboardKeyCode::Up: return "Up Arrow";
		case KeyboardKeyCode::Down: return "Down Arrow";

		case KeyboardKeyCode::Space: return "Space";
		case KeyboardKeyCode::Enter: return "Enter";
		case KeyboardKeyCode::Escape: return "Escape";
		case KeyboardKeyCode::Tab: return "Tab";
		case KeyboardKeyCode::Backspace: return "Backspace";
		case KeyboardKeyCode::LeftShift: return "Left Shift";
		case KeyboardKeyCode::RightShift: return "Right Shift";
		case KeyboardKeyCode::LeftControl: return "Left Control";
		case KeyboardKeyCode::RightControl: return "Right Control";
		case KeyboardKeyCode::LeftAlt: return "Left Alt";
		case KeyboardKeyCode::RightAlt: return "Right Alt";
		case KeyboardKeyCode::LeftSuper: return "Left Super";
		case KeyboardKeyCode::RightSuper: return "Right Super";

		case KeyboardKeyCode::Unknown:
		default: return "Unknown";
	}
}

KeyboardKeyCode KeyboardKeyCodeFromGlfw(const int keycode) {
	switch (keycode) {
		case GLFW_KEY_A: return KeyboardKeyCode::A;
		case GLFW_KEY_B: return KeyboardKeyCode::B;
		case GLFW_KEY_C: return KeyboardKeyCode::C;
		case GLFW_KEY_D: return KeyboardKeyCode::D;
		case GLFW_KEY_E: return KeyboardKeyCode::E;
		case GLFW_KEY_F: return KeyboardKeyCode::F;
		case GLFW_KEY_G: return KeyboardKeyCode::G;
		case GLFW_KEY_H: return KeyboardKeyCode::H;
		case GLFW_KEY_I: return KeyboardKeyCode::I;
		case GLFW_KEY_J: return KeyboardKeyCode::J;
		case GLFW_KEY_K: return KeyboardKeyCode::K;
		case GLFW_KEY_L: return KeyboardKeyCode::L;
		case GLFW_KEY_M: return KeyboardKeyCode::M;
		case GLFW_KEY_N: return KeyboardKeyCode::N;
		case GLFW_KEY_O: return KeyboardKeyCode::O;
		case GLFW_KEY_P: return KeyboardKeyCode::P;
		case GLFW_KEY_Q: return KeyboardKeyCode::Q;
		case GLFW_KEY_R: return KeyboardKeyCode::R;
		case GLFW_KEY_S: return KeyboardKeyCode::S;
		case GLFW_KEY_T: return KeyboardKeyCode::T;
		case GLFW_KEY_U: return KeyboardKeyCode::U;
		case GLFW_KEY_V: return KeyboardKeyCode::V;
		case GLFW_KEY_W: return KeyboardKeyCode::W;
		case GLFW_KEY_X: return KeyboardKeyCode::X;
		case GLFW_KEY_Y: return KeyboardKeyCode::Y;
		case GLFW_KEY_Z: return KeyboardKeyCode::Z;

		case GLFW_KEY_0: return KeyboardKeyCode::Num0;
		case GLFW_KEY_1: return KeyboardKeyCode::Num1;
		case GLFW_KEY_2: return KeyboardKeyCode::Num2;
		case GLFW_KEY_3: return KeyboardKeyCode::Num3;
		case GLFW_KEY_4: return KeyboardKeyCode::Num4;
		case GLFW_KEY_5: return KeyboardKeyCode::Num5;
		case GLFW_KEY_6: return KeyboardKeyCode::Num6;
		case GLFW_KEY_7: return KeyboardKeyCode::Num7;
		case GLFW_KEY_8: return KeyboardKeyCode::Num8;
		case GLFW_KEY_9: return KeyboardKeyCode::Num9;

		case GLFW_KEY_F1: return KeyboardKeyCode::F1;
		case GLFW_KEY_F2: return KeyboardKeyCode::F2;
		case GLFW_KEY_F3: return KeyboardKeyCode::F3;
		case GLFW_KEY_F4: return KeyboardKeyCode::F4;
		case GLFW_KEY_F5: return KeyboardKeyCode::F5;
		case GLFW_KEY_F6: return KeyboardKeyCode::F6;
		case GLFW_KEY_F7: return KeyboardKeyCode::F7;
		case GLFW_KEY_F8: return KeyboardKeyCode::F8;
		case GLFW_KEY_F9: return KeyboardKeyCode::F9;
		case GLFW_KEY_F10: return KeyboardKeyCode::F10;
		case GLFW_KEY_F11: return KeyboardKeyCode::F11;
		case GLFW_KEY_F12: return KeyboardKeyCode::F12;

		case GLFW_KEY_LEFT: return KeyboardKeyCode::Left;
		case GLFW_KEY_RIGHT: return KeyboardKeyCode::Right;
		case GLFW_KEY_UP: return KeyboardKeyCode::Up;
		case GLFW_KEY_DOWN: return KeyboardKeyCode::Down;

		case GLFW_KEY_SPACE: return KeyboardKeyCode::Space;
		case GLFW_KEY_ENTER: return KeyboardKeyCode::Enter;
		case GLFW_KEY_ESCAPE: return KeyboardKeyCode::Escape;
		case GLFW_KEY_TAB: return KeyboardKeyCode::Tab;
		case GLFW_KEY_BACKSPACE: return KeyboardKeyCode::Backspace;
		case GLFW_KEY_LEFT_SHIFT: return KeyboardKeyCode::LeftShift;
		case GLFW_KEY_RIGHT_SHIFT: return KeyboardKeyCode::RightShift;
		case GLFW_KEY_LEFT_CONTROL: return KeyboardKeyCode::LeftControl;
		case GLFW_KEY_RIGHT_CONTROL: return KeyboardKeyCode::RightControl;
		case GLFW_KEY_LEFT_ALT: return KeyboardKeyCode::LeftAlt;
		case GLFW_KEY_RIGHT_ALT: return KeyboardKeyCode::RightAlt;
		case GLFW_KEY_LEFT_SUPER: return KeyboardKeyCode::LeftSuper;
		case GLFW_KEY_RIGHT_SUPER: return KeyboardKeyCode::RightSuper;

		default: return KeyboardKeyCode::Unknown;
	}
}

int GlfwKeyCodeFromKeyboard(const KeyboardKeyCode keycode) {
	switch (keycode) {
		case KeyboardKeyCode::A: return GLFW_KEY_A;
		case KeyboardKeyCode::B: return GLFW_KEY_B;
		case KeyboardKeyCode::C: return GLFW_KEY_C;
		case KeyboardKeyCode::D: return GLFW_KEY_D;
		case KeyboardKeyCode::E: return GLFW_KEY_E;
		case KeyboardKeyCode::F: return GLFW_KEY_F;
		case KeyboardKeyCode::G: return GLFW_KEY_G;
		case KeyboardKeyCode::H: return GLFW_KEY_H;
		case KeyboardKeyCode::I: return GLFW_KEY_I;
		case KeyboardKeyCode::J: return GLFW_KEY_J;
		case KeyboardKeyCode::K: return GLFW_KEY_K;
		case KeyboardKeyCode::L: return GLFW_KEY_L;
		case KeyboardKeyCode::M: return GLFW_KEY_M;
		case KeyboardKeyCode::N: return GLFW_KEY_N;
		case KeyboardKeyCode::O: return GLFW_KEY_O;
		case KeyboardKeyCode::P: return GLFW_KEY_P;
		case KeyboardKeyCode::Q: return GLFW_KEY_Q;
		case KeyboardKeyCode::R: return GLFW_KEY_R;
		case KeyboardKeyCode::S: return GLFW_KEY_S;
		case KeyboardKeyCode::T: return GLFW_KEY_T;
		case KeyboardKeyCode::U: return GLFW_KEY_U;
		case KeyboardKeyCode::V: return GLFW_KEY_V;
		case KeyboardKeyCode::W: return GLFW_KEY_W;
		case KeyboardKeyCode::X: return GLFW_KEY_X;
		case KeyboardKeyCode::Y: return GLFW_KEY_Y;
		case KeyboardKeyCode::Z: return GLFW_KEY_Z;

		case KeyboardKeyCode::Num0: return GLFW_KEY_0;
		case KeyboardKeyCode::Num1: return GLFW_KEY_1;
		case KeyboardKeyCode::Num2: return GLFW_KEY_2;
		case KeyboardKeyCode::Num3: return GLFW_KEY_3;
		case KeyboardKeyCode::Num4: return GLFW_KEY_4;
		case KeyboardKeyCode::Num5: return GLFW_KEY_5;
		case KeyboardKeyCode::Num6: return GLFW_KEY_6;
		case KeyboardKeyCode::Num7: return GLFW_KEY_7;
		case KeyboardKeyCode::Num8: return GLFW_KEY_8;
		case KeyboardKeyCode::Num9: return GLFW_KEY_9;

		case KeyboardKeyCode::F1: return GLFW_KEY_F1;
		case KeyboardKeyCode::F2: return GLFW_KEY_F2;
		case KeyboardKeyCode::F3: return GLFW_KEY_F3;
		case KeyboardKeyCode::F4: return GLFW_KEY_F4;
		case KeyboardKeyCode::F5: return GLFW_KEY_F5;
		case KeyboardKeyCode::F6: return GLFW_KEY_F6;
		case KeyboardKeyCode::F7: return GLFW_KEY_F7;
		case KeyboardKeyCode::F8: return GLFW_KEY_F8;
		case KeyboardKeyCode::F9: return GLFW_KEY_F9;
		case KeyboardKeyCode::F10: return GLFW_KEY_F10;
		case KeyboardKeyCode::F11: return GLFW_KEY_F11;
		case KeyboardKeyCode::F12: return GLFW_KEY_F12;

		case KeyboardKeyCode::Left: return GLFW_KEY_LEFT;
		case KeyboardKeyCode::Right: return GLFW_KEY_RIGHT;
		case KeyboardKeyCode::Up: return GLFW_KEY_UP;
		case KeyboardKeyCode::Down: return GLFW_KEY_DOWN;

		case KeyboardKeyCode::Space: return GLFW_KEY_SPACE;
		case KeyboardKeyCode::Enter: return GLFW_KEY_ENTER;
		case KeyboardKeyCode::Escape: return GLFW_KEY_ESCAPE;
		case KeyboardKeyCode::Tab: return GLFW_KEY_TAB;
		case KeyboardKeyCode::Backspace: return GLFW_KEY_BACKSPACE;
		case KeyboardKeyCode::LeftShift: return GLFW_KEY_LEFT_SHIFT;
		case KeyboardKeyCode::RightShift: return GLFW_KEY_RIGHT_SHIFT;
		case KeyboardKeyCode::LeftControl: return GLFW_KEY_LEFT_CONTROL;
		case KeyboardKeyCode::RightControl: return GLFW_KEY_RIGHT_CONTROL;
		case KeyboardKeyCode::LeftAlt: return GLFW_KEY_LEFT_ALT;
		case KeyboardKeyCode::RightAlt: return GLFW_KEY_RIGHT_ALT;
		case KeyboardKeyCode::LeftSuper: return GLFW_KEY_LEFT_SUPER;
		case KeyboardKeyCode::RightSuper: return GLFW_KEY_RIGHT_SUPER;

		case KeyboardKeyCode::Unknown:
		default: return GLFW_KEY_UNKNOWN;
	}
}

ImGuiKey ImGuiKeyFromKeyboard(const KeyboardKeyCode keycode) {
	switch (keycode) {
		case KeyboardKeyCode::A: return ImGuiKey_A;
		case KeyboardKeyCode::B: return ImGuiKey_B;
		case KeyboardKeyCode::C: return ImGuiKey_C;
		case KeyboardKeyCode::D: return ImGuiKey_D;
		case KeyboardKeyCode::E: return ImGuiKey_E;
		case KeyboardKeyCode::F: return ImGuiKey_F;
		case KeyboardKeyCode::G: return ImGuiKey_G;
		case KeyboardKeyCode::H: return ImGuiKey_H;
		case KeyboardKeyCode::I: return ImGuiKey_I;
		case KeyboardKeyCode::J: return ImGuiKey_J;
		case KeyboardKeyCode::K: return ImGuiKey_K;
		case KeyboardKeyCode::L: return ImGuiKey_L;
		case KeyboardKeyCode::M: return ImGuiKey_M;
		case KeyboardKeyCode::N: return ImGuiKey_N;
		case KeyboardKeyCode::O: return ImGuiKey_O;
		case KeyboardKeyCode::P: return ImGuiKey_P;
		case KeyboardKeyCode::Q: return ImGuiKey_Q;
		case KeyboardKeyCode::R: return ImGuiKey_R;
		case KeyboardKeyCode::S: return ImGuiKey_S;
		case KeyboardKeyCode::T: return ImGuiKey_T;
		case KeyboardKeyCode::U: return ImGuiKey_U;
		case KeyboardKeyCode::V: return ImGuiKey_V;
		case KeyboardKeyCode::W: return ImGuiKey_W;
		case KeyboardKeyCode::X: return ImGuiKey_X;
		case KeyboardKeyCode::Y: return ImGuiKey_Y;
		case KeyboardKeyCode::Z: return ImGuiKey_Z;

		case KeyboardKeyCode::Num0: return ImGuiKey_0;
		case KeyboardKeyCode::Num1: return ImGuiKey_1;
		case KeyboardKeyCode::Num2: return ImGuiKey_2;
		case KeyboardKeyCode::Num3: return ImGuiKey_3;
		case KeyboardKeyCode::Num4: return ImGuiKey_4;
		case KeyboardKeyCode::Num5: return ImGuiKey_5;
		case KeyboardKeyCode::Num6: return ImGuiKey_6;
		case KeyboardKeyCode::Num7: return ImGuiKey_7;
		case KeyboardKeyCode::Num8: return ImGuiKey_8;
		case KeyboardKeyCode::Num9: return ImGuiKey_9;

		case KeyboardKeyCode::F1: return ImGuiKey_F1;
		case KeyboardKeyCode::F2: return ImGuiKey_F2;
		case KeyboardKeyCode::F3: return ImGuiKey_F3;
		case KeyboardKeyCode::F4: return ImGuiKey_F4;
		case KeyboardKeyCode::F5: return ImGuiKey_F5;
		case KeyboardKeyCode::F6: return ImGuiKey_F6;
		case KeyboardKeyCode::F7: return ImGuiKey_F7;
		case KeyboardKeyCode::F8: return ImGuiKey_F8;
		case KeyboardKeyCode::F9: return ImGuiKey_F9;
		case KeyboardKeyCode::F10: return ImGuiKey_F10;
		case KeyboardKeyCode::F11: return ImGuiKey_F11;
		case KeyboardKeyCode::F12: return ImGuiKey_F12;

		case KeyboardKeyCode::Left: return ImGuiKey_LeftArrow;
		case KeyboardKeyCode::Right: return ImGuiKey_RightArrow;
		case KeyboardKeyCode::Up: return ImGuiKey_UpArrow;
		case KeyboardKeyCode::Down: return ImGuiKey_DownArrow;

		case KeyboardKeyCode::Space: return ImGuiKey_Space;
		case KeyboardKeyCode::Enter: return ImGuiKey_Enter;
		case KeyboardKeyCode::Escape: return ImGuiKey_Escape;
		case KeyboardKeyCode::Tab: return ImGuiKey_Tab;
		case KeyboardKeyCode::Backspace: return ImGuiKey_Backspace;
		case KeyboardKeyCode::LeftShift: return ImGuiKey_LeftShift;
		case KeyboardKeyCode::RightShift: return ImGuiKey_RightShift;
		case KeyboardKeyCode::LeftAlt: return ImGuiKey_LeftAlt;
		case KeyboardKeyCode::RightAlt: return ImGuiKey_RightAlt;

#if CE_PLATFORM_MACOS
		case KeyboardKeyCode::LeftControl: return ImGuiKey_LeftSuper;
		case KeyboardKeyCode::RightControl: return ImGuiKey_RightSuper;

		case KeyboardKeyCode::LeftSuper: return ImGuiKey_LeftCtrl;
		case KeyboardKeyCode::RightSuper: return ImGuiKey_RightCtrl;
#else
		case KeyboardKeyCode::LeftControl: return ImGuiKey_LeftCtrl;
		case KeyboardKeyCode::RightControl: return ImGuiKey_RightCtrl;

		case KeyboardKeyCode::LeftSuper: return ImGuiKey_LeftSuper;
		case KeyboardKeyCode::RightSuper: return ImGuiKey_RightSuper;
#endif

		case KeyboardKeyCode::Unknown:
		default: return ImGuiKey_None;
	}
}

KeyboardKeyCode KeyboardKeyCodeFromCocoa(const unsigned short keyCode) {
	// macOS virtual key codes from HIToolbox/Events.h
	switch (keyCode) {
		// Letters
		case 0: return KeyboardKeyCode::A;
		case 11: return KeyboardKeyCode::B;
		case 8: return KeyboardKeyCode::C;
		case 2: return KeyboardKeyCode::D;
		case 14: return KeyboardKeyCode::E;
		case 3: return KeyboardKeyCode::F;
		case 5: return KeyboardKeyCode::G;
		case 4: return KeyboardKeyCode::H;
		case 34: return KeyboardKeyCode::I;
		case 38: return KeyboardKeyCode::J;
		case 40: return KeyboardKeyCode::K;
		case 37: return KeyboardKeyCode::L;
		case 46: return KeyboardKeyCode::M;
		case 45: return KeyboardKeyCode::N;
		case 31: return KeyboardKeyCode::O;
		case 35: return KeyboardKeyCode::P;
		case 12: return KeyboardKeyCode::Q;
		case 15: return KeyboardKeyCode::R;
		case 1: return KeyboardKeyCode::S;
		case 17: return KeyboardKeyCode::T;
		case 32: return KeyboardKeyCode::U;
		case 9: return KeyboardKeyCode::V;
		case 13: return KeyboardKeyCode::W;
		case 7: return KeyboardKeyCode::X;
		case 16: return KeyboardKeyCode::Y;
		case 6: return KeyboardKeyCode::Z;

		// Numbers
		case 29: return KeyboardKeyCode::Num0;
		case 18: return KeyboardKeyCode::Num1;
		case 19: return KeyboardKeyCode::Num2;
		case 20: return KeyboardKeyCode::Num3;
		case 21: return KeyboardKeyCode::Num4;
		case 23: return KeyboardKeyCode::Num5;
		case 22: return KeyboardKeyCode::Num6;
		case 26: return KeyboardKeyCode::Num7;
		case 28: return KeyboardKeyCode::Num8;
		case 25: return KeyboardKeyCode::Num9;

		// Function keys
		case 122: return KeyboardKeyCode::F1;
		case 120: return KeyboardKeyCode::F2;
		case 99: return KeyboardKeyCode::F3;
		case 118: return KeyboardKeyCode::F4;
		case 96: return KeyboardKeyCode::F5;
		case 97: return KeyboardKeyCode::F6;
		case 98: return KeyboardKeyCode::F7;
		case 100: return KeyboardKeyCode::F8;
		case 101: return KeyboardKeyCode::F9;
		case 109: return KeyboardKeyCode::F10;
		case 103: return KeyboardKeyCode::F11;
		case 111: return KeyboardKeyCode::F12;

		// Arrow keys
		case 123: return KeyboardKeyCode::Left;
		case 124: return KeyboardKeyCode::Right;
		case 125: return KeyboardKeyCode::Down;
		case 126: return KeyboardKeyCode::Up;

		// Special keys
		case 49: return KeyboardKeyCode::Space;
		case 36: return KeyboardKeyCode::Enter;
		case 53: return KeyboardKeyCode::Escape;
		case 48: return KeyboardKeyCode::Tab;
		case 51: return KeyboardKeyCode::Backspace;
		case 56: return KeyboardKeyCode::LeftShift;
		case 60: return KeyboardKeyCode::RightShift;
		case 59: return KeyboardKeyCode::LeftControl;
		case 62: return KeyboardKeyCode::RightControl;
		case 58: return KeyboardKeyCode::LeftAlt;
		case 61: return KeyboardKeyCode::RightAlt;
		case 55: return KeyboardKeyCode::LeftSuper;  // Command key
		case 54: return KeyboardKeyCode::RightSuper; // Command key

		default: return KeyboardKeyCode::Unknown;
	}
}

unsigned short CocoaKeyCodeFromKeyboard(const KeyboardKeyCode keyCode) {
	// macOS virtual key codes from HIToolbox/Events.h
	switch (keyCode) {
		// Letters
		case KeyboardKeyCode::A: return 0;
		case KeyboardKeyCode::B: return 11;
		case KeyboardKeyCode::C: return 8;
		case KeyboardKeyCode::D: return 2;
		case KeyboardKeyCode::E: return 14;
		case KeyboardKeyCode::F: return 3;
		case KeyboardKeyCode::G: return 5;
		case KeyboardKeyCode::H: return 4;
		case KeyboardKeyCode::I: return 34;
		case KeyboardKeyCode::J: return 38;
		case KeyboardKeyCode::K: return 40;
		case KeyboardKeyCode::L: return 37;
		case KeyboardKeyCode::M: return 46;
		case KeyboardKeyCode::N: return 45;
		case KeyboardKeyCode::O: return 31;
		case KeyboardKeyCode::P: return 35;
		case KeyboardKeyCode::Q: return 12;
		case KeyboardKeyCode::R: return 15;
		case KeyboardKeyCode::S: return 1;
		case KeyboardKeyCode::T: return 17;
		case KeyboardKeyCode::U: return 32;
		case KeyboardKeyCode::V: return 9;
		case KeyboardKeyCode::W: return 13;
		case KeyboardKeyCode::X: return 7;
		case KeyboardKeyCode::Y: return 16;
		case KeyboardKeyCode::Z: return 6;

		// Numbers
		case KeyboardKeyCode::Num0: return 29;
		case KeyboardKeyCode::Num1: return 18;
		case KeyboardKeyCode::Num2: return 19;
		case KeyboardKeyCode::Num3: return 20;
		case KeyboardKeyCode::Num4: return 21;
		case KeyboardKeyCode::Num5: return 23;
		case KeyboardKeyCode::Num6: return 22;
		case KeyboardKeyCode::Num7: return 26;
		case KeyboardKeyCode::Num8: return 28;
		case KeyboardKeyCode::Num9: return 25;

		// Function keys
		case KeyboardKeyCode::F1: return 122;
		case KeyboardKeyCode::F2: return 120;
		case KeyboardKeyCode::F3: return 99;
		case KeyboardKeyCode::F4: return 118;
		case KeyboardKeyCode::F5: return 96;
		case KeyboardKeyCode::F6: return 97;
		case KeyboardKeyCode::F7: return 98;
		case KeyboardKeyCode::F8: return 100;
		case KeyboardKeyCode::F9: return 101;
		case KeyboardKeyCode::F10: return 109;
		case KeyboardKeyCode::F11: return 103;
		case KeyboardKeyCode::F12: return 111;

		// Arrow keys
		case KeyboardKeyCode::Left: return 123;
		case KeyboardKeyCode::Right: return 124;
		case KeyboardKeyCode::Down: return 125;
		case KeyboardKeyCode::Up: return 126;

		// Special keys
		case KeyboardKeyCode::Space: return 49;
		case KeyboardKeyCode::Enter: return 36;
		case KeyboardKeyCode::Escape: return 53;
		case KeyboardKeyCode::Tab: return 48;
		case KeyboardKeyCode::Backspace: return 51;
		case KeyboardKeyCode::LeftShift: return 56;
		case KeyboardKeyCode::RightShift: return 60;
		case KeyboardKeyCode::LeftControl: return 59;
		case KeyboardKeyCode::RightControl: return 62;
		case KeyboardKeyCode::LeftAlt: return 58;
		case KeyboardKeyCode::RightAlt: return 61;
		case KeyboardKeyCode::LeftSuper: return 55;  // Command key
		case KeyboardKeyCode::RightSuper: return 54; // Command key

		case KeyboardKeyCode::Unknown:
		default: return 0;
	}
}

}
