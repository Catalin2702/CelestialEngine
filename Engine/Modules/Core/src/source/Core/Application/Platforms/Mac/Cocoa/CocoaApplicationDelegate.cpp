//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplicationDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"


namespace CE::Core::Application {

void CocoaApplicationDelegate::applicationDidFinishLaunching(NS::Notification*) {
	auto& app = dynamic_cast<CocoaApplication&>(I_Application::StGet());
	app.SetRunning(true);
}

bool CocoaApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application*) {
	return true;
}

}
