//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui
// File: ImGuiHelpers.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include "Core/Layers/ImGui/ImGuiHelpers.hpp"

#include <string>

namespace CE::Core {

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

}
