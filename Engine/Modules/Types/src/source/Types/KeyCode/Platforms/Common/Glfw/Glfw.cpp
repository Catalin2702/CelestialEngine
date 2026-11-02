//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode/Platforms/Common/Glfw
// File: Glfw.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/KeyCode/Platforms/Common/Glfw/Glfw.hpp"

#include <GLFW/glfw3.h>


namespace CE::Types {

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

}
