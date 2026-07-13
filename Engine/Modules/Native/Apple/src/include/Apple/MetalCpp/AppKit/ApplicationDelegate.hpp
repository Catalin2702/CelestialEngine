//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: CocoaApplicationDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#include "Define/DynamicLinker.hpp"

#include <AppKit/AppKit.hpp>

#include <functional>

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_APPLICATIONDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_APPLICATIONDELEGATE_HPP

namespace NS {
	class Application;
	class Notification;
}

using AppDelegateCallback = std::function<void(NS::Notification*)>;

namespace CE::Native {

class CE_API ApplicationDelegate: public NS::I_ApplicationDelegate {
public:
	ApplicationDelegate() = default;

	~ApplicationDelegate() override = default;

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
	 * @brief Set the callback for the applicationDidFinishLaunching method
	 * @param callback
	 */
	void SetApplicationDidFinishLaunchingCallback(AppDelegateCallback callback);

	/**
	 * @ Set the callback for the applicationWillFinishLaunching method
	 * @param callback
	 */
	void SetApplicationWillFinishLaunchingCallback(AppDelegateCallback callback);

	/**
	 * Set the value returned by applicationShouldTerminateAfterLastWindowClosed method
	 * @param should
	 */
	void SetApplicationShouldTerminateAfterLastWindowClosed(bool should);

private:
	AppDelegateCallback _appDidFinishLaunchingCallback;
	AppDelegateCallback _appWillFinishLaunchingCallback;
	bool _appShouldTerminate = true;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_APPLICATIONDELEGATE_HPP
