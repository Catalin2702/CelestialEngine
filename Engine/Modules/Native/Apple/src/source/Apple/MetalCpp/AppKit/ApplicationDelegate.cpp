//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: CocoaApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#include "Apple/MetalCpp/AppKit/ApplicationDelegate.hpp"

#include <Foundation/Foundation.hpp>

namespace NS {

void ApplicationDelegate::applicationDidFinishLaunching(Notification* notification) {
	if (appDidFinishLaunchingCallback)
		applicationDidFinishLaunching(notification);
}

void ApplicationDelegate::applicationWillFinishLaunching(Notification* notification) {
	if (appWillFinishLaunchingCallback)
		appWillFinishLaunchingCallback(notification);
}

bool ApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(Application*) {
	return true;
}

void ApplicationDelegate::SetApplicationDidFinishLaunchingCallback(const callback& callback) {
	appDidFinishLaunchingCallback = callback;
}

void ApplicationDelegate::SetApplicationWillFinishLaunchingCallback(const callback& callback) {
	appWillFinishLaunchingCallback = callback;
}

}
