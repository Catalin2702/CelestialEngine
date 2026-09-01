//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaPlatform.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP
#define CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP

#include "Core/Window/I_Platform.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

/**
 * @class CocoaPlatform
 * @brief AppKit's side of I_Platform, which is almost nothing
 * @details Cocoa has no library to initialise and no queue to drain by hand: NSApplication is brought up by the
 *			application itself, and its run loop delivers events without being asked. So this exists to make the
 *			abstraction total - the owner holds one I_Platform whatever the backend - rather than to do work.
 *
 *			That is a real difference between the backends, not a gap to be filled later: GLFW hands you a queue and
 *			expects you to pump it, AppKit pumps itself and calls you back.
 */
class CE_CORE_API CocoaPlatform final: public I_PlatformBase<Types::WindowApi::Cocoa> {
public:
	CocoaPlatform() = default;

	CocoaPlatform(const CocoaPlatform&) = delete;

	CocoaPlatform(CocoaPlatform&&) = delete;

	~CocoaPlatform() override = default;

public:
	CocoaPlatform& operator = (const CocoaPlatform&) = delete;
	CocoaPlatform& operator = (CocoaPlatform&&) = delete;

public:
	/**
	 * @brief Does nothing
	 * @details The NSApplication run loop delivers events on its own, so there is no queue for a frame to drain.
	 *			Calling this every frame is correct and free; it is the GLFW backend that needs the call.
	 */
	void PollEvents() const override {}
};

}

#endif //CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP
