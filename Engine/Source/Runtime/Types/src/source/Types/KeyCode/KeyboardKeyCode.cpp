//
// Module: CelestialEngine/Engine/Source/Runtime/Types/KeyCode
// File: KeyboardKeyCode.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-09
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "Types/KeyCode/KeyboardKeyCode.hpp"


namespace CE::Types {

std::string_view format_as(const KeyboardKeyCode key) {
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

}
