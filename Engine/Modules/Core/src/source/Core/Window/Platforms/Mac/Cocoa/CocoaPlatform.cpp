//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaPlatform.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp"

#include "Tools/Tools.hpp"

#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

#include <objc/message.h>

#include <stdexcept>


namespace CE::Core {

CocoaPlatform::CocoaPlatform() {
	_application = NS::Application::sharedApplication();
	if (not _application) [[unlikely]] {
		constexpr auto error = "CocoaPlatform::CocoaPlatform: Could not reach the NSApplication singleton!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_applicationDelegate.SetApplicationDidFinishLaunchingDelegate(
		EventDelegate<NS::Notification*>::FromConstMethod<CocoaPlatform, &CocoaPlatform::_OnDidFinishLaunching>(this)
	);

	// AppKit would otherwise quit the process the moment the last window closes, which would run the teardown from
	// under the engine instead of through it. The engine ends the loop itself when it sees the close event.
	_applicationDelegate.SetApplicationShouldTerminateAfterLastWindowClosed(false);

	_application->setDelegate(&_applicationDelegate);
}

CocoaPlatform::~CocoaPlatform() {
	// The delegate is a member and is about to go; NSApplication is not, and it holds the pointer.
	if (_application)
		_application->setDelegate(nullptr);
}

void CocoaPlatform::Prepare() {
	// Before finishLaunching, not after: the policy is what decides whether this process gets a Dock icon and a menu
	// bar at all, and AppKit reads it while coming up. CocoaApplication set it from applicationWillFinishLaunching for
	// the same reason.
	if (_application->activationPolicy() != NS::ActivationPolicyRegular) [[likely]] {
		if (not _application->setActivationPolicy(NS::ActivationPolicyRegular)) [[unlikely]] {
			constexpr auto error = "CocoaPlatform::Prepare: Could not set the application's activation policy!";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
	}

	// The one step NSApplication::run() would have done for us. It posts applicationWillFinishLaunching and
	// applicationDidFinishLaunching synchronously and then returns, instead of taking the thread - which is the whole
	// reason this backend can share the engine's run loop with every other one.
	//
	// Sent through the runtime because the metal-cpp fork does not declare this selector; every other call in this
	// file compiles down to the same objc_msgSend.
	using FinishLaunching = void (*)(id, SEL);
	reinterpret_cast<FinishLaunching>(objc_msgSend)(reinterpret_cast<id>(_application), sel_registerName("finishLaunching"));

	_application->activateIgnoringOtherApps(true);

	// Announced here rather than from applicationDidFinishLaunching, even though the delegate is installed and would
	// eventually be called: AppKit does not promise that notification synchronously from finishLaunching, and it must
	// not be. Start() calls Prepare() and then immediately needs a window - waiting on a notification that may only
	// arrive on the first pump would mean the first frame runs before there is anything to render into.
	onReadyDispatcher.Dispatch();
}

void CocoaPlatform::PollEvents() const {
	// One pool per pump. AppKit hands out autoreleased events, and with no NSApplication::run() there is no pool being
	// drained for us: without this they would accumulate for the life of the process.
	const auto autoreleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	// An expiration already in the past is what makes this non-blocking, and it has to be in the past rather than
	// "now": given an instant that has not passed yet, AppKit is entitled to run the run loop until it arrives, and
	// the sources it services there are the ones Core Animation drives off the display. That is a wait of up to one
	// refresh, handed back to AppKit, which is exactly what this backend is not doing. NSDate::distantPast is the
	// canonical value; the fork does not expose it, so any comfortably past instant does the same job.
	const auto expiration = NS::Date::dateWithTimeIntervalSinceNow(-1.0);

	while (NS::Event* const event = _application->nextEventMatchingMask(
		NS::EventMaskAny, expiration, NS::RunLoop::defaultMode(), true))
	{
		_application->sendEvent(event);
	}
}

void CocoaPlatform::_OnDidFinishLaunching(NS::Notification*) const {
	CE_CORE_TRACE("CocoaPlatform: NSApplication finished launching.");
}

}
