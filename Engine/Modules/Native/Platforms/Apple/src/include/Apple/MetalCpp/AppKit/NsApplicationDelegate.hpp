//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsApplicationDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-18
//

#pragma once

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>


#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_NSAPPLICATIONDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_NSAPPLICATIONDELEGATE_HPP

namespace NS {
	class Application;
	class Notification;
}

namespace CE::Native {

class CE_API NsApplicationDelegate: public NS::I_ApplicationDelegate {
public:
	NsApplicationDelegate() = default;

	~NsApplicationDelegate() override = default;

public:
	/**
	 * @brief Called when the application has finished launching
	 * @details This method is called by the Cocoa framework when the application has completed its launch process. It can be used to perform any final initialization steps that require the application to be fully launched, such as setting up the main window or starting the main loop.
	 */
	void applicationDidFinishLaunching(NS::Notification* notification) override;

	/**
	 * @brief Called when the application is about to finish launching
	 * @details This method is called by the Cocoa framework before the application has completed its launch process. It can be used to perform any necessary setup before the application is fully launched, such as configuring application-wide settings or initializing resources that need to be ready before the main loop starts.
	 */
	void applicationWillFinishLaunching(NS::Notification* notification) override;

	/**
	 * @brief Determines whether the application should terminate after the last window is closed
	 * @return bool True if the application should terminate, false otherwise
	 * @details This method is called by the Cocoa framework when the last window of the application is closed. Returning true will cause the application to terminate, while returning false will keep the application running in the background. This allows for applications that can continue running without any open windows, such as menu bar applications or background services.
	 */
	bool applicationShouldTerminateAfterLastWindowClosed(NS::Application*) override;

public:
	/**
	 * @brief Set the delegate for the applicationDidFinishLaunching method
	 * @param delegate
	 */
	void SetApplicationDidFinishLaunchingDelegate(const EventDelegate<NS::Notification*>& delegate);

	/**
	 * @ Set the delegate for the applicationWillFinishLaunching method
	 * @param delegate
	 */
	void SetApplicationWillFinishLaunchingDelegate(const EventDelegate<NS::Notification*>& delegate);

	/**
	 * Set the value returned by applicationShouldTerminateAfterLastWindowClosed method
	 * @param should
	 */
	void SetApplicationShouldTerminateAfterLastWindowClosed(bool should);

private:
	CallbackDispatcher<void, NS::Notification*> _appDidFinishLaunchingDispatcher;
	CallbackDispatcher<void, NS::Notification*> _appWillFinishLaunchingDispatcher;
	bool _appShouldTerminate = true;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_NSAPPLICATIONDELEGATE_HPP
