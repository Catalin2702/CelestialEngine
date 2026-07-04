//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: CocoaApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Apple/MetalCpp/AppKit/ApplicationDelegate.hpp"

#include <Foundation/Foundation.hpp>

#include <utility>

namespace NS {

void ApplicationDelegate::applicationDidFinishLaunching(Notification* notification) {
	if (_appDidFinishLaunchingCallback)
		_appDidFinishLaunchingCallback(notification);
}

void ApplicationDelegate::applicationWillFinishLaunching(Notification* notification) {
	if (_appWillFinishLaunchingCallback)
		_appWillFinishLaunchingCallback(notification);
}

bool ApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(Application*) {
	return _appShouldTerminate;
}

void ApplicationDelegate::SetApplicationDidFinishLaunchingCallback(AppDelegateCallback callback) {
	_appDidFinishLaunchingCallback = std::move(callback);
}

void ApplicationDelegate::SetApplicationWillFinishLaunchingCallback(AppDelegateCallback callback) {
	_appWillFinishLaunchingCallback = std::move(callback);
}

void ApplicationDelegate::SetApplicationShouldTerminateAfterLastWindowClosed(bool should) {
	_appShouldTerminate = should;
}

}
