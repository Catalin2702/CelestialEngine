/*
 *
 * Copyright 2020-2021 Apple Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// AppKit/NSApplication.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"
#include "NSEvent.hpp"
#include "MetalCpp/Foundation/Foundation.hpp"

namespace NS {
_NS_OPTIONS(NS::UInteger, WindowStyleMask) {
	WindowStyleMaskBorderless = 0,
	WindowStyleMaskTitled = (1 << 0),
	WindowStyleMaskClosable = (1 << 1),
	WindowStyleMaskMiniaturizable = (1 << 2),
	WindowStyleMaskResizable = (1 << 3),
	WindowStyleMaskTexturedBackground = (1 << 8),
	WindowStyleMaskUnifiedTitleAndToolbar = (1 << 12),
	WindowStyleMaskFullScreen = (1 << 14),
	WindowStyleMaskFullSizeContentView = (1 << 15),
	WindowStyleMaskUtilityWindow = (1 << 4),
	WindowStyleMaskDocModalWindow = (1 << 6),
	WindowStyleMaskNonactivatingPanel = (1 << 7),
	WindowStyleMaskHUDWindow = (1 << 13)
};

_NS_ENUM(NS::UInteger, BackingStoreType) {
	BackingStoreRetained = 0,
	BackingStoreNonretained = 1,
	BackingStoreBuffered = 2
};

_NS_ENUM(NS::UInteger, ActivationPolicy) {
	ActivationPolicyRegular,
	ActivationPolicyAccessory,
	ActivationPolicyProhibited
};

class Application;
class Date;
class Event;
class Menu;
class String;

class ApplicationDelegate {
public:
	virtual ~ApplicationDelegate() = default;

	virtual void applicationWillFinishLaunching([[maybe_unused]] Notification* pNotification) {}

	virtual void applicationDidFinishLaunching([[maybe_unused]] Notification* pNotification) {}

	virtual bool applicationShouldTerminateAfterLastWindowClosed([[maybe_unused]] Application* pSender) { return false; }
};

class Application: public Referencing<Application> {
public:
	static Application* sharedApplication();

	void setDelegate(const ApplicationDelegate* pDelegate);

	[[nodiscard]] bool setActivationPolicy(ActivationPolicy activationPolicy) const;

	void activateIgnoringOtherApps(bool ignoreOtherApps) const;

	void setMainMenu(const Menu* pMenu) const;

	[[nodiscard]] ActivationPolicy activationPolicy() const;

	[[nodiscard]] Array* windows() const;

	[[nodiscard]] Event* nextEventMatchingMask(EventMask mask,  Date* expiration,  String* mode, bool dequeue) const;

	void sendEvent( Event* event) const;

	void postEvent(Event* event, bool atStart) const;

	void run() const;

	void terminate(const Object* pSender) const;

	void stop(const Object* pSender) const;
};

}

_NS_INLINE NS::Application* NS::Application::sharedApplication() {
	return sendMessage<Application*>(_APPKIT_PRIVATE_CLS(NSApplication), _APPKIT_PRIVATE_SEL(sharedApplication));
}

_NS_INLINE void NS::Application::setDelegate(const ApplicationDelegate* pAppDelegate) {
	// NOTE: this pWrapper is only held with a weak reference
	Value* pWrapper = Value::value(pAppDelegate);

	typedef void (*DispatchFunction)(Value*, SEL, void*);

	DispatchFunction willFinishLaunching = [](Value* pSelf, SEL, void* pNotification) {
		const auto pDel = reinterpret_cast<ApplicationDelegate*>(pSelf->pointerValue());
		pDel->applicationWillFinishLaunching(static_cast<Notification*>(pNotification));
	};

	DispatchFunction didFinishLaunching = [](Value* pSelf, SEL, void* pNotification) {
		const auto pDel = reinterpret_cast<ApplicationDelegate*>(pSelf->pointerValue());
		pDel->applicationDidFinishLaunching(static_cast<Notification*>(pNotification));
	};

	DispatchFunction shouldTerminateAfterLastWindowClosed = [](Value* pSelf, SEL, void* pApplication) {
		const auto pDel = reinterpret_cast<ApplicationDelegate*>(pSelf->pointerValue());
		pDel->applicationShouldTerminateAfterLastWindowClosed(static_cast<Application*>(pApplication));
	};

#ifdef __OBJC__
	class_addMethod((__bridge Class)_NS_PRIVATE_CLS(NSValue), _APPKIT_PRIVATE_SEL(applicationWillFinishLaunching_), (
		IMP)willFinishLaunching, "v@:@" );
	class_addMethod((__bridge Class)_NS_PRIVATE_CLS(NSValue), _APPKIT_PRIVATE_SEL(applicationDidFinishLaunching_), (IMP)
		didFinishLaunching, "v@:@" );
	class_addMethod((__bridge Class)_NS_PRIVATE_CLS(NSValue), _APPKIT_PRIVATE_SEL(
		applicationShouldTerminateAfterLastWindowClosed_), (IMP)shouldTerminateAfterLastWindowClosed, "B@:@" );
#else
	class_addMethod((Class)_NS_PRIVATE_CLS(NSValue), _APPKIT_PRIVATE_SEL(applicationWillFinishLaunching_),
					(IMP)willFinishLaunching, "v@:@");
	class_addMethod((Class)_NS_PRIVATE_CLS(NSValue), _APPKIT_PRIVATE_SEL(applicationDidFinishLaunching_),
					(IMP)didFinishLaunching, "v@:@");
	class_addMethod((Class)_NS_PRIVATE_CLS(NSValue),
					_APPKIT_PRIVATE_SEL(applicationShouldTerminateAfterLastWindowClosed_),
					(IMP)shouldTerminateAfterLastWindowClosed, "B@:@");
#endif

	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setDelegate_), pWrapper);
}

_NS_INLINE bool NS::Application::setActivationPolicy(const ActivationPolicy activationPolicy) const {
	return sendMessage<bool>(this, _APPKIT_PRIVATE_SEL(setActivationPolicy_), activationPolicy);
}

_NS_INLINE void NS::Application::activateIgnoringOtherApps(const bool ignoreOtherApps) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(activateIgnoringOtherApps_), (ignoreOtherApps ? YES : NO));
}

_NS_INLINE void NS::Application::setMainMenu(const  Menu* pMenu) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setMainMenu_), pMenu);
}

_NS_INLINE NS::ActivationPolicy NS::Application::activationPolicy() const {
	return sendMessage<ActivationPolicy>(this, _APPKIT_PRIVATE_SEL(activationPolicy));
}

_NS_INLINE NS::Array* NS::Application::windows() const {
	return sendMessage<NS::Array*>(this, _APPKIT_PRIVATE_SEL(windows));
}

_NS_INLINE NS::Event* NS::Application::nextEventMatchingMask(const EventMask mask, Date* expiration,  String* mode, const bool dequeue) const {
	return sendMessage<Event*>(this, _APPKIT_PRIVATE_SEL(nextEventMatchingMask_untilDate_inMode_dequeue_), mask, expiration, mode, dequeue);
}

_NS_INLINE void NS::Application::sendEvent( Event* event) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(sendEvent_), event);
}

_NS_INLINE void NS::Application::postEvent(Event* event, const bool atStart) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(postEvent_atStart_), event, atStart);
}

_NS_INLINE void NS::Application::run() const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(run));
}

_NS_INLINE void NS::Application::terminate(const Object* pSender) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(terminate_), pSender);
}

_NS_INLINE void NS::Application::stop(const Object* pSender) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(stop_), pSender);
}
