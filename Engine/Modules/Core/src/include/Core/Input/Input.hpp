//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: Input.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_CORE_INPUT_INPUT_HPP
#define CE_CORE_INPUT_INPUT_HPP

#include "Define/DynamicLinker.hpp"

#include "InputState.hpp"
#include "KeyShortcut.hpp"

#include "Types/KeyCode/KeyCode.hpp"

#include <array>
#include <concepts>
#include <span>
#include <utility>


namespace CE::Core {

/**
 * @class Input
 * @brief Input handling system
 * @details Answers every input query from the event-driven InputState it owns. The state is fed by the event hub, which
 *			already delivers standardized engine events regardless of the windowing backend, so a single
 *			class serves every platform: queries are focus-correct (only events delivered to the engine window count) and
 *			consistent within a frame, with no OS polling.
 *			The state is fully private (Input is its friend and only interface): every query and lifecycle operation is
 *			exposed here as a static method, so callers never touch the state directly.
 *			Managed as a singleton (Init/Shutdown/Get) so input can be queried statically from anywhere in the engine.
 */
class CE_CORE_API Input final {
public:
	/**
	 * @brief Creates the singleton input instance
	 * @details Idempotent: does nothing if the input system is already initialized. Must be called before any input
	 *			query and before the application subscribes the input state to the event hub.
	 */
	static void Init();

	/**
	 * @brief Destroys the singleton input instance
	 * @details Idempotent: does nothing if the input system is not initialized. The application must unsubscribe the
	 *			input state from the event hub BEFORE calling this, otherwise the hub would keep delegates pointing to
	 *			a destroyed state.
	 */
	static void Shutdown();

	/**
	 * @brief Gets the singleton input instance
	 * @return Input& Reference to the input instance
	 * @details Asserts (and crashes in release) if the input system is not initialized: call Init() first.
	 */
	[[nodiscard]] static Input& Get();

public:
	/**
	 * @brief Checks if a specific key is currently pressed
	 * @param keyCode The keycode of the key to check
	 * @return bool True while the key is held
	 */
	static bool IsKeyPressed(Types::KeyboardKeyCode keyCode);

	/**
	 * @brief Checks if a key transitioned to pressed during the current frame
	 * @param keyCode The keycode of the key to check
	 * @return bool True only in the frame the key went down (OS key repeats do not re-trigger it)
	 */
	static bool IsKeyJustPressed(Types::KeyboardKeyCode keyCode);

	/**
	 * @brief Checks if a key transitioned to released during the current frame
	 * @param keyCode The keycode of the key to check
	 * @return bool True only in the frame the key went up
	 */
	static bool IsKeyJustReleased(Types::KeyboardKeyCode keyCode);

	/**
	 * @brief Checks if every key of a chord is held at the same time
	 * @return bool True while all of them are down; false for an empty chord
	 * @details For a continuous action (Ctrl held while dragging). A one-shot command wants IsShortcutJustPressed,
	 *			which does not fire again on every frame the keys stay down.
	 */
	static bool AreKeysPressed(std::span<const Types::KeyboardKeyCode> keys);

	/**
	 * @brief AreKeysPressed(LeftControl, LeftShift, S), without building the array by hand
	 */
	template<std::same_as<Types::KeyboardKeyCode>... Keys> requires (sizeof...(Keys) > 0)
	static bool AreKeysPressed(const Keys... keys) {
		const std::array<Types::KeyboardKeyCode, sizeof...(Keys)> chord{keys...};
		return AreKeysPressed(std::span<const Types::KeyboardKeyCode>{chord});
	}

	/**
	 * @brief Checks if a chord was completed during the current frame, to run a shortcut once per press
	 * @return bool True in the one frame every key is held and at least one has just gone down
	 * @details The order the keys are pressed in does not matter: Shift then F1 and F1 then Shift both fire, on the
	 *			frame the second key goes down. Extra keys held alongside do not prevent it.
	 */
	static bool IsShortcutJustPressed(std::span<const Types::KeyboardKeyCode> keys);

	/**
	 * @brief IsShortcutJustPressed(LeftShift, F1), without building the array by hand
	 */
	template <std::same_as<Types::KeyboardKeyCode>... Keys> requires (sizeof...(Keys) > 0)
	static bool IsShortcutJustPressed(const Keys... keys) {
		const std::array<Types::KeyboardKeyCode, sizeof...(Keys)> chord{keys...};
		return IsShortcutJustPressed(std::span<const Types::KeyboardKeyCode>{chord});
	}

	/**
	 * @brief IsShortcutJustPressed for a shortcut stored in a descriptor
	 */
	static bool IsShortcutJustPressed(const KeyShortcut& shortcut) { return IsShortcutJustPressed(shortcut.GetKeys()); }

	/**
	 * @brief Checks if a specific mouse button is currently pressed
	 * @param buttonCode The button code of the mouse button to check
	 * @return bool True while the button is held
	 */
	static bool IsMouseButtonPressed(Types::MouseButtonCode buttonCode);

	/**
	 * @brief Checks if a mouse button transitioned to pressed during the current frame
	 * @param buttonCode The button code of the mouse button to check
	 * @return bool True only in the frame the button went down
	 */
	static bool IsMouseButtonJustPressed(Types::MouseButtonCode buttonCode);

	/**
	 * @brief Checks if a mouse button transitioned to released during the current frame
	 * @param buttonCode The button code of the mouse button to check
	 * @return bool True only in the frame the button went up
	 */
	static bool IsMouseButtonJustReleased(Types::MouseButtonCode buttonCode);

	/**
	 * @brief Gets the current X position of the mouse cursor
	 * @return f32 The X coordinate of the mouse cursor in window space (top-left origin)
	 */
	static f32 GetMouseX();

	/**
	 * @brief Gets the current Y position of the mouse cursor
	 * @return f32 The Y coordinate of the mouse cursor in window space (top-left origin)
	 */
	static f32 GetMouseY();

	/**
	 * @brief Gets the current X and Y position of the mouse cursor
	 * @return std::pair<f32, f32> The X and Y coordinates of the mouse cursor in window space (top-left origin)
	 */
	static std::pair<f32, f32> GetMouseXY();

	/**
	 * @brief Gets how far the cursor moved on X since the end of the previous frame
	 * @return f32 The X motion in window space; zero on the first position after startup or a focus loss
	 */
	static f32 GetMouseDeltaX();

	/**
	 * @brief Gets how far the cursor moved on Y since the end of the previous frame
	 * @return f32 The Y motion in window space (positive is down); zero on the first position after startup or a focus loss
	 */
	static f32 GetMouseDeltaY();

	/**
	 * @brief Gets how far the cursor moved since the end of the previous frame
	 * @return std::pair<f32, f32> The X and Y motion in window space (positive Y is down)
	 * @details Measured on the absolute cursor position, so it stops at the edges of the screen: a mouse-look that
	 *			must keep turning would need relative motion from a captured cursor, which the backends do not report yet.
	 */
	static std::pair<f32, f32> GetMouseDeltaXY();

	/**
	 * @brief Gets the horizontal scroll accumulated during the current frame
	 * @return f32 The X scroll delta
	 */
	static f32 GetScrollDeltaX();

	/**
	 * @brief Gets the vertical scroll accumulated during the current frame
	 * @return f32 The Y scroll delta
	 */
	static f32 GetScrollDeltaY();

	/**
	 * @brief Gets who the pointer's motion currently belongs to
	 */
	static Types::CursorMode GetCursorMode();

	/**
	 * @brief Shorthand for GetCursorMode() == Captured
	 */
	static bool IsCursorCaptured() { return GetCursorMode() == Types::CursorMode::Captured; }

public:
	/**
	 * @brief Decides who the pointer's motion goes to: the cursor, or whatever reads the mouse delta
	 * @param mode Normal moves the visible cursor; Captured hides it, holds it still and keeps the delta flowing with
	 *			no screen edge to stop it - what a mouse-look camera wants
	 * @details The Unreal-style switch between UI input and game input. Applied to the window straight away, and
	 *			released automatically when the window loses focus.
	 */
	static void SetCursorMode(Types::CursorMode mode);

	/**
	 * @brief Gives the input system the window SetCursorMode acts on
	 * @details Called by the application once the window exists. Without one the mode is only recorded.
	 */
	static void BindWindow(I_Window* window);

public:
	/**
	 * @brief Clears the state's per-frame transitions (just pressed/released, scroll and mouse deltas)
	 * @details Called by the application once at the end of each tick, after every layer had the chance to query the
	 *			state and before the next batch of native events is pumped. Held keys/buttons persist across frames.
	 */
	static void EndFrame();

	/**
	 * @brief Subscribes the input state to the event hub's keyboard/mouse/window multicast dispatchers
	 * @details Every platform hub exposes the same dispatcher member names, so the hub structs are taken
	 *			as template parameters. The application must call this BEFORE any other subscriber so the state is
	 *			updated first, and Init() must have run.
	 */
	template <typename KeyboardHub, typename MouseHub, typename WindowHub>
	static void SubscribeToHub(KeyboardHub& keyboardHub, MouseHub& mouseHub, WindowHub& windowHub) {
		Get()._state.SubscribeToHub(keyboardHub, mouseHub, windowHub);
	}

	/**
	 * @brief Removes the input state's subscriptions from the event hub dispatchers
	 * @details Must be called BEFORE Shutdown(), otherwise the hub would keep delegates pointing to a destroyed state.
	 */
	template <typename KeyboardHub, typename MouseHub, typename WindowHub>
	static void UnsubscribeFromHub(KeyboardHub& keyboardHub, MouseHub& mouseHub, WindowHub& windowHub) {
		Get()._state.UnsubscribeFromHub(keyboardHub, mouseHub, windowHub);
	}

private:
	/**
	 * @brief Private constructor
	 * @details Only Init() can create the singleton instance
	 */
	Input() = default;

private:
	InputState _state;								///< Event-driven input state, fed by the event hub and read by the queries

	static Input* _instance;						///< Singleton instance of the Input class
};

}

#endif //CE_CORE_INPUT_INPUT_HPP
