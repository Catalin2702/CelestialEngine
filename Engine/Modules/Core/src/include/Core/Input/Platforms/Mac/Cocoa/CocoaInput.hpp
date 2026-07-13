//
// Module: CelestialEngine/Engine/Core/Input/Platforms/Mac/Cocoa
// File: CocoaInput.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_INPUT_MAC_COCOAINPUT_HPP
#define CE_CORE_INPUT_MAC_COCOAINPUT_HPP

#include "Core/Input/I_Input.hpp"

#include "Define/DynamicLinker.hpp"

#include <utility>

namespace NS {
	class Window;
}


namespace CE::Core {

class CE_API CocoaInput final: public I_Input {
protected:
	[[nodiscard]] bool _IsKeyPressedImpl(Types::KeyboardKeyCode keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(Types::MouseButtonCode buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] std::pair<float, float> _GetMouseXYImpl() override;

private:
	void _InitWindow();

private:
	NS::Window* _window = nullptr;				///< Cached pointer to the Cocoa window for input state queries
};

}

#endif //CE_CORE_INPUT_MAC_COCOAINPUT_HPP