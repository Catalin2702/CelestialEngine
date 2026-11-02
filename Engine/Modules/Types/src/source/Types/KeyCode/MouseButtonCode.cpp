//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: MouseButtonCode.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-09
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/KeyCode/MouseButtonCode.hpp"


namespace CE::Types {

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

}
