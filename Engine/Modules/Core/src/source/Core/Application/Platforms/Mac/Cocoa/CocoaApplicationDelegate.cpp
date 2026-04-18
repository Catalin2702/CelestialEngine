//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-18
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"


namespace CE::Core::Application {

void CocoaApplicationDelegate::applicationDidFinishLaunching(NS::Notification* pNotification) {}

void CocoaApplicationDelegate::applicationWillFinishLaunching(NS::Notification* pNotification) {}

bool CocoaApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* pSender) {
	return false;
}

}
