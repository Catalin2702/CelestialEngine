//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: Input.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_INPUT_INPUT_HPP
#define CE_CORE_INPUT_INPUT_HPP

#include "Define/DynamicLinker.hpp"

#include "Core/Input/I_Input.hpp"

#include "Core/Input/InputState.hpp"

#include <utility>

namespace CE::Core {

/**
 * @class Input
 * @brief Platform-agnostic input handling implementation
 * @details Implements the I_Input interface by reading the event-driven InputState it owns. The state is fed by the
 *			event hub, which already delivers standardized engine events regardless of the windowing backend (Cocoa or
 *			GLFW), so a single implementation serves every platform: queries are focus-correct (only events delivered
 *			to the engine window count) and consistent within a frame, with no OS polling.
 *			This class is instantiated as the singleton instance of I_Input.
 */
class CE_API Input final: public I_Input {
public:
	/**
	 * @brief Creates the singleton input instance
	 * @details Idempotent: does nothing if the input system is already initialized. Must be called before any input
	 *			query and before the application subscribes the input state to the event hub. No platform parameter is
	 *			needed: the event hub already delivers standardized engine events for every windowing backend.
	 */
	static void Init();

	/**
	 * @brief Destroys the singleton input instance
	 * @details Idempotent: does nothing if the input system is not initialized. The application must unsubscribe the
	 *			input state from the event hub BEFORE calling this, otherwise the hub would keep delegates pointing to
	 *			a destroyed state.
	 */
	static void Shutdown();

public:
	[[nodiscard]] InputState& GetState() override { return _state; }

protected:
	[[nodiscard]] bool _IsKeyPressedImpl(Types::KeyboardKeyCode keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(Types::MouseButtonCode buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] std::pair<float, float> _GetMouseXYImpl() override;

private:
	InputState _state;							///< Event-driven input state, fed by the event hub and read by the queries
};

}

#endif //CE_CORE_INPUT_INPUT_HPP
