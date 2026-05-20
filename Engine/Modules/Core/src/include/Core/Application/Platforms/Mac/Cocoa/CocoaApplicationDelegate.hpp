//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplicationDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-05-20
//

#pragma once

#include "Define/DynamicLinker.hpp"

#include "MetalCpp/AppKit/AppKit.hpp"

#ifndef CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATIONDELEGATE_HPP
#define CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATIONDELEGATE_HPP

namespace NS {
class Application;
class Notification;
}

namespace CE::Core::Application {

class CE_API CocoaApplicationDelegate: public NS::ApplicationDelegate {
public:
	/**
	 * @brief Default constructor
	 */
	CocoaApplicationDelegate() = default;

	/**
	 * @brief Default destructor
	 */
	~CocoaApplicationDelegate() override = default;

public:
	/**
	 * @brief Called when the application has finished launching
	 * @details This method is called by the Cocoa framework when the application has completed its launch process. It can be used to perform any final initialization steps that require the application to be fully launched, such as setting up the main window or starting the main loop.
	 */
	void applicationDidFinishLaunching(NS::Notification*) override;

	/**
	 * @brief Called when the application is about to finish launching
	 * @details This method is called by the Cocoa framework before the application has completed its launch process. It can be used to perform any necessary setup before the application is fully launched, such as configuring application-wide settings or initializing resources that need to be ready before the main loop starts.
	 */
	void applicationWillFinishLaunching(NS::Notification*) override {}

	/**
	 * @brief Determines whether the application should terminate after the last window is closed
	 * @return bool True if the application should terminate, false otherwise
	 * @details This method is called by the Cocoa framework when the last window of the application is closed. Returning true will cause the application to terminate, while returning false will keep the application running in the background. This allows for applications that can continue running without any open windows, such as menu bar applications or background services.
	 */
	bool applicationShouldTerminateAfterLastWindowClosed(NS::Application*) override;
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATIONDELEGATE_HPP
