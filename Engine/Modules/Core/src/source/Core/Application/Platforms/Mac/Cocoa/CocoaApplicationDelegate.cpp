//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-18
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"


namespace CE::Core::Application {

void CocoaApplicationDelegate::applicationDidFinishLaunching(NS::Notification* pNotification) {}

void CocoaApplicationDelegate::applicationWillFinishLaunching(NS::Notification* pNotification) {
	auto& app = dynamic_cast<CocoaApplication&>(I_Application::StGet());
	app.StartDisplayLink();
}

bool CocoaApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* pSender) {
	return true;
}

}
