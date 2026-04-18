//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-18
//

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplicationDelegate.hpp"

#include <AppKit/AppKit.hpp>


namespace CE::Core::Application {

CocoaApplication::CocoaApplication() {
	// Set the singleton instance pointer to this instance
	assert(_stInstance == nullptr && "CocoaApplication already exists!");
	_stInstance = this;

	_appCocoa = NS::RetainPtr(NS::Application::sharedApplication());

	_appDelegate = std::make_unique<CocoaApplicationDelegate>();
}

CocoaApplication::~CocoaApplication() {
	_appCocoa.reset();
	_appDelegate.reset();
}

void CocoaApplication::Run() {
	_appCocoa->setDelegate(_appDelegate.get());

	_appCocoa->setActivationPolicy(NS::ActivationPolicyRegular);
	_appCocoa->activateIgnoringOtherApps(true);
}

void CocoaApplication::Quit() {
	_appCocoa->terminate(nullptr);
}

void CocoaApplication::Update() {
}

void CocoaApplication::OnEvent(Events::I_Event& event) {
}

}
