//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-18
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
#define CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP

#include "Core/Application/I_Application.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>

#include <memory>

namespace NS {
class Application;
}

namespace CE::Core::Application {
class CocoaApplicationDelegate;
}

namespace CE::Core::Application {

/**
 * @class CocoaApplication
 * @brief macOS-specific application implementation using Cocoa
 * @details Provides a macOS-specific implementation of the I_Application interface using Cocoa for application management.
 *			Manages the application lifecycle and event handling for a native macOS application.
 */
class CE_API CocoaApplication: public I_Application {
public:
	/**
	 * @brief Default constructor
	 */
	CocoaApplication();

	/**
	 * @brief Copy constructor (deleted)
	 */
	CocoaApplication(const CocoaApplication& other) = delete;

	/**
	 * @brief Move constructor (deleted)
	 */
	CocoaApplication(CocoaApplication&& other) noexcept = delete;

	/**
	 * @brief Destructor
	 */
	~CocoaApplication() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 */
	CocoaApplication& operator=(const CocoaApplication& other) = delete;

	/**
	 * @brief Move assignment operator (deleted)
	 */
	CocoaApplication& operator=(CocoaApplication&& other) = delete;

public:
	/**
	 * @brief Runs the application
	 * @details Starts the AppKit Cocoa application loop
	 */
	void Run() override;

	/**
	 * @brief Quits the application
	 * @details Terminates the AppKit Cocoa application loop
	 */
	void Quit() override;

	/**
	 * @brief Updates the application state
	 * @details Called every frame to update the application. Updates all layers in the layer stack.
	 */
	void Update() override;

	/**
	 * @brief Handles events
	 * @param event Reference to the event to be processed
	 * @details Dispatches events to the appropriate layers in the layer stack
	 */
	void OnEvent(Events::I_Event& event) override;

private:
	NS::SharedPtr<NS::Application> _appCocoa;				///< Pointer to the Cocoa application instance
	std::unique_ptr<CocoaApplicationDelegate> _appDelegate;	///< Delegate for handling Cocoa application events
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
