//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation/DisplayLink
// File: DisplayLink.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
#define CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP

// C wrapper function declarations
extern "C" {
	typedef void (*DisplayLinkCallbackFn)(void* userData);

	void* CE_DisplayLink_Create(void);
	void CE_DisplayLink_Destroy(void* displayLink);
	void CE_DisplayLink_SetCallback(void* displayLink, DisplayLinkCallbackFn callback, void* userData);
	void CE_DisplayLink_Start(void* displayLink);
	void CE_DisplayLink_Stop(void* displayLink);
}

namespace CA {

/**
 * @class DisplayLink
 * @brief C++ wrapper for the CVDisplayLink-based display synchronization
 */
class DisplayLink {
public:
	using TickFn = void(*)(void*);

	DisplayLink() : _handle(nullptr) {}

	~DisplayLink() {
		if (_handle) {
			CE_DisplayLink_Destroy(_handle);
			_handle = nullptr;
		}
	}

	// Non-copyable
	DisplayLink(const DisplayLink&) = delete;
	DisplayLink& operator=(const DisplayLink&) = delete;

	// Movable
	DisplayLink(DisplayLink&& other) noexcept : _handle(other._handle) {
		other._handle = nullptr;
	}

	DisplayLink& operator=(DisplayLink&& other) noexcept {
		if (this != &other) {
			if (_handle) {
				CE_DisplayLink_Destroy(_handle);
			}
			_handle = other._handle;
			other._handle = nullptr;
		}
		return *this;
	}

	/**
	 * @brief Creates a new DisplayLink instance
	 * @return Pointer to a new DisplayLink, or nullptr on failure
	 */
	static DisplayLink* create() {
		auto* link = new DisplayLink();
		link->_handle = CE_DisplayLink_Create();
		if (!link->_handle) {
			delete link;
			return nullptr;
		}
		return link;
	}

	/**
	 * @brief Sets the callback function to be called on each display refresh
	 */
	void setCallback(TickFn callback, void* userData) const {
		if (_handle) {
			CE_DisplayLink_SetCallback(_handle, callback, userData);
		}
	}

	/**
	 * @brief Starts the display link
	 */
	void start() const {
		if (_handle) {
			CE_DisplayLink_Start(_handle);
		}
	}

	/**
	 * @brief Stops the display link
	 */
	void stop() const {
		if (_handle) {
			CE_DisplayLink_Stop(_handle);
		}
	}

	/**
	 * @brief Checks if the display link is valid
	 */
	[[nodiscard]] bool isValid() const {
		return _handle != nullptr;
	}

private:
	void* _handle;
};

}

#endif //CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
