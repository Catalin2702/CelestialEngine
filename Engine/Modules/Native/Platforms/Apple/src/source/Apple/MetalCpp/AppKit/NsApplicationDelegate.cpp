//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp"

#include <Foundation/Foundation.hpp>

#include <cassert>


namespace CE::Native {

// Both launch notifications are optional to handle, and AppKit posts them either way. Asking whether anyone is
// listening is the whole of it: this used to assert instead, which made "I only care about the terminate flag" - the
// commonest reason to install a delegate at all - an error rather than a choice.
void NsApplicationDelegate::applicationDidFinishLaunching(NS::Notification* notification) {
	if (_appDidFinishLaunchingDispatcher.IsBound())
		_appDidFinishLaunchingDispatcher.Execute(notification);
}

void NsApplicationDelegate::applicationWillFinishLaunching(NS::Notification* notification) {
	if (_appWillFinishLaunchingDispatcher.IsBound()) [[likely]]
		_appWillFinishLaunchingDispatcher.Execute(notification);
}

bool NsApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application*) {
	return _appShouldTerminate;
}

void NsApplicationDelegate::SetApplicationDidFinishLaunchingDelegate(const EventDelegate<NS::Notification*>& delegate) {
	assert(delegate.IsValid() and "NsApplicationDelegate::SetApplicationDidFinishLaunchingDelegate: The delegate is not valid!");
	_appDidFinishLaunchingDispatcher.Bind(delegate);
}

void NsApplicationDelegate::SetApplicationWillFinishLaunchingDelegate(const EventDelegate<NS::Notification*>& delegate) {
	assert(delegate.IsValid() and "MNsApplicationDelegate::SetApplicationWillFinishLaunchingDelegate: The delegate is not valid!");
	_appWillFinishLaunchingDispatcher.Bind(delegate);
}

void NsApplicationDelegate::SetApplicationShouldTerminateAfterLastWindowClosed(const bool should) {
	_appShouldTerminate = should;
}

}
