//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaPlatform.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP
#define CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP

#include "Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp"
#include "Core/Window/I_Platform.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>


namespace NS {
	class Application;
	class Notification;
}

namespace CE::Core {

/**
 * @class CocoaPlatform
 * @brief NSApplication's lifetime and AppKit's event queue, owned once instead of per window
 * @details The same job GlfwPlatform does, for the backend that spells it differently. AppKit has no glfwInit, but it
 *			has the same process-wide state: the NSApplication singleton, its delegate, the activation policy that
 *			decides whether the process has a Dock icon at all, and the queue every window's events arrive through.
 *
 *			The one real difference is who owns the thread, and it is resolved here rather than lived with. AppKit is
 *			usually run the other way round - NSApplication::run() takes the thread and never gives it back - but it
 *			does not have to be: finishLaunching brings the application up without starting a loop, and pumping the
 *			queue by hand afterwards is exactly what glfwPollEvents does underneath. That choice is what lets the
 *			engine keep its own run loop on every backend, and it is why PollEvents here is not the no-op it looks
 *			like it should be.
 *
 *			Owned by the application and declared before the window, so AppKit is up before a window needs it and torn
 *			down after the last one is gone.
 */
class CE_CORE_API CocoaPlatform final: public I_PlatformBase<Types::WindowApi::Cocoa> {
public:
	/**
	 * @brief Creates the NSApplication singleton and installs the delegate that reports its launch
	 * @details Creation only: nothing here makes AppKit usable yet, which is Prepare()'s job. Splitting the two is
	 *			what lets the application subscribe to onReadyDispatcher in between.
	 */
	CocoaPlatform();

	CocoaPlatform(const CocoaPlatform&) = delete;

	CocoaPlatform(CocoaPlatform&&) = delete;

	/**
	 * @brief Detaches the delegate before it can outlive this object
	 */
	~CocoaPlatform() override;

public:
	CocoaPlatform& operator = (const CocoaPlatform&) = delete;
	CocoaPlatform& operator = (CocoaPlatform&&) = delete;

public:
	/**
	 * @brief Finishes AppKit's launch, brings the process to the foreground, and announces that windows can be created
	 * @details Everything CocoaApplication used to do from applicationDidFinishLaunching, minus the window reveal,
	 *			which belongs to whoever owns the window. finishLaunching posts both launch notifications
	 *			synchronously, so onReadyDispatcher has fired by the time this returns - the same as on GLFW, reached
	 *			the long way round.
	 */
	void Prepare() override;

	/**
	 * @brief Drains AppKit's event queue and delivers the events to their windows
	 * @details Non-blocking: it takes what is already queued and returns, so the engine's own run loop keeps the
	 *			thread and keeps pacing frames. This is the manual half of the bargain struck by not calling
	 *			NSApplication::run().
	 */
	void PollEvents() const override;

	/**
	 * @brief Does nothing
	 * @details AppKit reports no library-level errors of its own: what would be a GLFW error here is either an
	 *			exception at creation or a native event the window already routes.
	 */
	void ConnectToEventHub(I_EventHubDispatcher&) override {}

public:
	/**
	 * @brief Gets the NSApplication singleton
	 * @details Not on the interface: it is here for the Cocoa-only code that has to reach AppKit itself - the menu
	 *			bar, and Quit, which has to stop the application and not only the loop.
	 */
	[[nodiscard]] NS::Application* GetApplication() const { return _application; }

private:
	/**
	 * @brief Notes that AppKit has finished launching
	 * @details Informational only. The notification arrives on the first pump, long after Prepare() has already told
	 *			everyone the backend is ready - which is why the readiness is announced there and not here. Bound all
	 *			the same because NsApplicationDelegate asserts on an unbound dispatcher.
	 */
	void _OnDidFinishLaunching(NS::Notification* notification) const;

private:
	/// Borrowed, not owned: NSApplication is a singleton the process itself keeps alive, and releasing it would be
	/// releasing something we were never handed a reference to.
	NS::Application* _application = nullptr;

	Native::NsApplicationDelegate _applicationDelegate;
};

}

#endif //CE_CORE_WINDOW_MAC_COCOAPLATFORM_HPP
