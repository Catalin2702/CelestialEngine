//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp"

#include <Foundation/Foundation.hpp>

#include <utility>

namespace CE::Native {

void NsApplicationDelegate::applicationDidFinishLaunching(NS::Notification* notification) {
	if (_appDidFinishLaunchingCallback)
		_appDidFinishLaunchingCallback(notification);
}

void NsApplicationDelegate::applicationWillFinishLaunching(NS::Notification* notification) {
	if (_appWillFinishLaunchingCallback)
		_appWillFinishLaunchingCallback(notification);
}

bool NsApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application*) {
	return _appShouldTerminate;
}

void NsApplicationDelegate::SetApplicationDidFinishLaunchingCallback(AppDelegateCallback callback) {
	_appDidFinishLaunchingCallback = std::move(callback);
}

void NsApplicationDelegate::SetApplicationWillFinishLaunchingCallback(AppDelegateCallback callback) {
	_appWillFinishLaunchingCallback = std::move(callback);
}

void NsApplicationDelegate::SetApplicationShouldTerminateAfterLastWindowClosed(bool should) {
	_appShouldTerminate = should;
}

}
