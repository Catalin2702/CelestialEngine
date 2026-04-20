//
// Module: CelestialEngine/Engine/Input/Platforms/Mac/Cocoa
// File: CocoaInput.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_INPUT_MAC_COCOAINPUT_HPP
#define CE_INPUT_MAC_COCOAINPUT_HPP

#include "Input/I_Input.hpp"

#include "Define/DynamicLinker.hpp"

#include <utility>

namespace NS {
	class Window;
}


namespace CE::Core::Input {

class CE_API CocoaInput final: public I_Input {
protected:
	[[nodiscard]] bool _IsKeyPressedImpl(KeyCode::KeyboardKeyCode keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(KeyCode::MouseButtonCode buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] std::pair<float, float> _GetMouseXYImpl() override;

private:
	void _InitCocoaWindow();

private:
	NS::Window* _cocoaWindow = nullptr;				///< Cached pointer to the Cocoa window for input state queries
};

}

#endif //CE_INPUT_MAC_COCOAINPUT_HPP