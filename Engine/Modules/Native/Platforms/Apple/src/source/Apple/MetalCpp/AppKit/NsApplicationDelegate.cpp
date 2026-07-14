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

#include <cassert>

namespace CE::Native {

void NsApplicationDelegate::applicationDidFinishLaunching(NS::Notification*) {
	assert(_appDidFinishLaunchingDispatcher.IsBound() and "NsApplicationDelegate::applicationDidFinishLaunching: The delegate is not bound.");
	_appDidFinishLaunchingDispatcher.Execute();
}

void NsApplicationDelegate::applicationWillFinishLaunching(NS::Notification*) {
	if (_appWillFinishLaunchingDispatcher.IsBound())
		_appWillFinishLaunchingDispatcher.Execute();
}

bool NsApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application*) {
	return _appShouldTerminate;
}

void NsApplicationDelegate::SetApplicationDidFinishLaunchingDelegate(const EventDelegate<>& delegate) {
	assert(delegate.IsValid() and "NsApplicationDelegate::SetApplicationDidFinishLaunchingDelegate: The delegate is not valid!");
	_appDidFinishLaunchingDispatcher.Bind(delegate);
}

void NsApplicationDelegate::SetApplicationWillFinishLaunchingDelegate(const EventDelegate<>& delegate) {
	assert(delegate.IsValid() and "MNsApplicationDelegate::SetApplicationWillFinishLaunchingDelegate: The delegate is not valid!");
	_appWillFinishLaunchingDispatcher.Bind(delegate);
}

void NsApplicationDelegate::SetApplicationShouldTerminateAfterLastWindowClosed(const bool should) {
	_appShouldTerminate = should;
}

}
