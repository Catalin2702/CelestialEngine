//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: InputState.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_INPUT_INPUTSTATE_HPP
#define CE_CORE_INPUT_INPUTSTATE_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <array>
#include <bitset>
#include <cstdint>
#include <utility>

namespace CE::Core {

/**
 * @class InputState
 * @brief Event-driven snapshot of the keyboard and mouse state
 * @details Fed by the event hub: it subscribes to the keyboard/mouse/window multicast dispatchers and mirrors every event
 *			into a per-frame snapshot (held keys/buttons, cursor position, scroll deltas and just-pressed/just-released
 *			transitions). Owned by the I_Input singleton instance: platform I_Input implementations answer their queries
 *			from this state instead of polling the OS, which keeps every query focus-correct and consistent within a frame.
 *			The state must be the FIRST subscriber on the hub so it is already up to date when any other subscriber
 *			(layers, ImGui) handles the same event, and the application must call EndFrame() once at the end of each tick,
 *			before the next batch of native events is pumped, to clear the per-frame transitions.
 *			A window focus loss resets all held keys/buttons: their release events would otherwise be delivered to another
 *			window and the keys would remain stuck as "down".
 */
class CE_API InputState {
	enum HubSubscription: std::size_t {
		KeyPressed = 0,
		KeyReleased,
		MouseMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseDragged,
		MouseWheelScrolled,
		WindowFocus,
		_Count
	};

public:
	static constexpr std::size_t stMaxKeys = 512;			///< KeyboardKeyCode values are GLFW-based; the highest one (RightSuper) is 347
	static constexpr std::size_t stMaxMouseButtons = 16;	///< MouseButtonCode buttons are 0-7 (wheel pseudo-buttons 8-9 are not tracked as buttons)

public:
	/**
	 * @brief Checks if a key is currently held down
	 * @param keyCode The key to check
	 * @return bool True while the key is held
	 */
	[[nodiscard]] bool IsKeyDown(Types::KeyboardKeyCode keyCode) const;

	/**
	 * @brief Checks if a key transitioned to pressed during the current frame
	 * @param keyCode The key to check
	 * @return bool True only in the frame the key went down (OS key repeats do not re-trigger it)
	 */
	[[nodiscard]] bool IsKeyJustPressed(Types::KeyboardKeyCode keyCode) const;

	/**
	 * @brief Checks if a key transitioned to released during the current frame
	 * @param keyCode The key to check
	 * @return bool True only in the frame the key went up
	 */
	[[nodiscard]] bool IsKeyJustReleased(Types::KeyboardKeyCode keyCode) const;

	/**
	 * @brief Checks if a mouse button is currently held down
	 * @param buttonCode The mouse button to check
	 * @return bool True while the button is held
	 */
	[[nodiscard]] bool IsMouseButtonDown(Types::MouseButtonCode buttonCode) const;

	/**
	 * @brief Checks if a mouse button transitioned to pressed during the current frame
	 * @param buttonCode The mouse button to check
	 * @return bool True only in the frame the button went down
	 */
	[[nodiscard]] bool IsMouseButtonJustPressed(Types::MouseButtonCode buttonCode) const;

	/**
	 * @brief Checks if a mouse button transitioned to released during the current frame
	 * @param buttonCode The mouse button to check
	 * @return bool True only in the frame the button went up
	 */
	[[nodiscard]] bool IsMouseButtonJustReleased(Types::MouseButtonCode buttonCode) const;

	/**
	 * @brief Gets the last known X position of the mouse cursor in window space (top-left origin)
	 */
	[[nodiscard]] float GetMouseX() const { return _mouseX; }

	/**
	 * @brief Gets the last known Y position of the mouse cursor in window space (top-left origin)
	 */
	[[nodiscard]] float GetMouseY() const { return _mouseY; }

	/**
	 * @brief Gets the last known mouse cursor position in window space (top-left origin)
	 */
	[[nodiscard]] std::pair<float, float> GetMouseXY() const { return {_mouseX, _mouseY}; }

	/**
	 * @brief Gets the horizontal scroll accumulated during the current frame
	 */
	[[nodiscard]] float GetScrollDeltaX() const { return _scrollDeltaX; }

	/**
	 * @brief Gets the vertical scroll accumulated during the current frame
	 */
	[[nodiscard]] float GetScrollDeltaY() const { return _scrollDeltaY; }

public:
	/**
	 * @brief Clears the per-frame transitions (just pressed/released, scroll deltas)
	 * @details Must be called once at the end of each application tick, after every layer had the chance to query the
	 *			state and before the next batch of native events is pumped. Held keys/buttons persist across frames and
	 *			are only cleared by their release events (or by Reset on focus loss).
	 */
	void EndFrame();

	/**
	 * @brief Releases every held key and mouse button
	 * @details Invoked when the window loses input focus: the matching release events would be delivered to another
	 *			window, so without this reset the keys would remain stuck as "down". The released keys are reported as
	 *			just-released for the current frame.
	 */
	void Reset();

public:
	/**
	 * @brief Subscribes the state to the event hub's keyboard/mouse/window multicast dispatchers
	 * @details Both platform hubs (Cocoa and GLFW) expose the same dispatcher member names, so the hub structs are taken
	 *			as template parameters. Call this BEFORE any other subscriber so the state is updated first.
	 */
	template <typename KeyboardHub, typename MouseHub, typename WindowHub>
	void SubscribeToHub(KeyboardHub& keyboardHub, MouseHub& mouseHub, WindowHub& windowHub) {
		using state = InputState;

		_hubHandles[KeyPressed] = keyboardHub.onPressedMulticastDispatcher.Subscribe(
			EventDelegate<Events::KeyPressedEvent&>::FromMethod<state, &state::_OnKeyPressed>(this)
		);
		_hubHandles[KeyReleased] = keyboardHub.onReleasedMulticastDispatcher.Subscribe(
			EventDelegate<Events::KeyReleasedEvent&>::FromMethod<state, &state::_OnKeyReleased>(this)
		);

		_hubHandles[MouseMoved] = mouseHub.onMovedMulticastDispatcher.Subscribe(
			EventDelegate<Events::MouseMovedEvent&>::FromMethod<state, &state::_OnMouseMoved>(this)
		);
		_hubHandles[MouseButtonPressed] = mouseHub.onButtonPressedMulticastDispatcher.Subscribe(
			EventDelegate<Events::MouseButtonPressedEvent&>::FromMethod<state, &state::_OnMouseButtonPressed>(this)
		);
		_hubHandles[MouseButtonReleased] = mouseHub.onButtonReleasedMulticastDispatcher.Subscribe(
			EventDelegate<Events::MouseButtonReleasedEvent&>::FromMethod<state, &state::_OnMouseButtonReleased>(this)
		);
		_hubHandles[MouseDragged] = mouseHub.onDraggedMulticastDispatcher.Subscribe(
			EventDelegate<Events::MouseDraggedEvent&>::FromMethod<state, &state::_OnMouseDragged>(this)
		);
		_hubHandles[MouseWheelScrolled] = mouseHub.onWheelScrolledMulticastDispatcher.Subscribe(
			EventDelegate<Events::MouseWheelScrolledEvent&>::FromMethod<state, &state::_OnMouseWheelScrolled>(this)
		);

		_hubHandles[WindowFocus] = windowHub.onFocusMulticastDispatcher.Subscribe(
			EventDelegate<Events::WindowFocusEvent&>::FromMethod<state, &state::_OnWindowFocus>(this)
		);
	}

	/**
	 * @brief Removes the state's subscriptions from the event hub dispatchers
	 */
	template <typename KeyboardHub, typename MouseHub, typename WindowHub>
	void UnsubscribeFromHub(KeyboardHub& keyboardHub, MouseHub& mouseHub, WindowHub& windowHub) {
		keyboardHub.onPressedMulticastDispatcher.Unsubscribe(_hubHandles[KeyPressed]);
		keyboardHub.onReleasedMulticastDispatcher.Unsubscribe(_hubHandles[KeyReleased]);

		mouseHub.onMovedMulticastDispatcher.Unsubscribe(_hubHandles[MouseMoved]);
		mouseHub.onButtonPressedMulticastDispatcher.Unsubscribe(_hubHandles[MouseButtonPressed]);
		mouseHub.onButtonReleasedMulticastDispatcher.Unsubscribe(_hubHandles[MouseButtonReleased]);
		mouseHub.onDraggedMulticastDispatcher.Unsubscribe(_hubHandles[MouseDragged]);
		mouseHub.onWheelScrolledMulticastDispatcher.Unsubscribe(_hubHandles[MouseWheelScrolled]);

		windowHub.onFocusMulticastDispatcher.Unsubscribe(_hubHandles[WindowFocus]);
	}

private:
	void _OnKeyPressed(const Events::KeyPressedEvent& event);
	void _OnKeyReleased(const Events::KeyReleasedEvent& event);
	void _OnMouseMoved(const Events::MouseMovedEvent& event);
	void _OnMouseButtonPressed(const Events::MouseButtonPressedEvent& event);
	void _OnMouseButtonReleased(const Events::MouseButtonReleasedEvent& event);
	void _OnMouseDragged(const Events::MouseDraggedEvent& event);
	void _OnMouseWheelScrolled(const Events::MouseWheelScrolledEvent& event);
	void _OnWindowFocus(const Events::WindowFocusEvent& event);

private:
	std::bitset<stMaxKeys> _keysDown;						///< Keys currently held (persists across frames)
	std::bitset<stMaxKeys> _keysJustPressed;				///< Keys that went down during the current frame
	std::bitset<stMaxKeys> _keysJustReleased;				///< Keys that went up during the current frame

	std::bitset<stMaxMouseButtons> _buttonsDown;			///< Mouse buttons currently held (persists across frames)
	std::bitset<stMaxMouseButtons> _buttonsJustPressed;		///< Mouse buttons that went down during the current frame
	std::bitset<stMaxMouseButtons> _buttonsJustReleased;	///< Mouse buttons that went up during the current frame

	float _mouseX = 0.0f, _mouseY = 0.0f;					///< Last known cursor position in window space (top-left origin)
	float _scrollDeltaX = 0.0f, _scrollDeltaY = 0.0f;		///< Scroll accumulated during the current frame

	std::array<uint32_t, _Count> _hubHandles{};				///< Handles of the hub subscriptions, used for unsubscription
};

}

#endif //CE_CORE_INPUT_INPUTSTATE_HPP
