//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode/Platforms/Mac/Cocoa
// File: Cocoa.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/KeyCode/Platforms/Mac/Cocoa/Cocoa.hpp"



namespace CE::Types {

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

MouseButtonCode MouseButtonKeyCodeFromCocoa(const long buttonNumber) {
	switch (buttonNumber) {
		case 0: return MouseButtonCode::Left;
		case 1: return MouseButtonCode::Right;
		case 2: return MouseButtonCode::Middle;
		default: return MouseButtonCode::Unknown;
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
