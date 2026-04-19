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
// AppKit/NSEvent.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"

#include <CoreGraphics/CGGeometry.h>
#include <Foundation/NSTypes.hpp>

namespace NS {
class Window;
class String;

// Event Types
using EventType = UInteger;

enum : EventType {
	EventTypeLeftMouseDown = 1,
	EventTypeLeftMouseUp = 2,
	EventTypeRightMouseDown = 3,
	EventTypeRightMouseUp = 4,
	EventTypeMouseMoved = 5,
	EventTypeLeftMouseDragged = 6,
	EventTypeRightMouseDragged = 7,
	EventTypeMouseEntered = 8,
	EventTypeMouseExited = 9,
	EventTypeKeyDown = 10,
	EventTypeKeyUp = 11,
	EventTypeFlagsChanged = 12,
	EventTypeAppKitDefined = 13,
	EventTypeSystemDefined = 14,
	EventTypeApplicationDefined = 15,
	EventTypePeriodic = 16,
	EventTypeCursorUpdate = 17,
	EventTypeScrollWheel = 22,
	EventTypeTabletPoint = 23,
	EventTypeTabletProximity = 24,
	EventTypeOtherMouseDown = 25,
	EventTypeOtherMouseUp = 26,
	EventTypeOtherMouseDragged = 27,
	EventTypeGesture = 29,
	EventTypeMagnify = 30,
	EventTypeSwipe = 31,
	EventTypeRotate = 18,
	EventTypeBeginGesture = 19,
	EventTypeEndGesture = 20,
	EventTypeSmartMagnify = 32,
	EventTypeQuickLook = 33,
	EventTypePressure = 34,
	EventTypeDirectTouch = 37,
	EventTypeChangeMode = 38,
	EventTypeMouseCancelled = 40,
};

// Event Mask
using EventMask = unsigned long long;

enum : EventMask {
	EventMaskLeftMouseDown = 1ULL << EventTypeLeftMouseDown,
	EventMaskLeftMouseUp = 1ULL << EventTypeLeftMouseUp,
	EventMaskRightMouseDown = 1ULL << EventTypeRightMouseDown,
	EventMaskRightMouseUp = 1ULL << EventTypeRightMouseUp,
	EventMaskMouseMoved = 1ULL << EventTypeMouseMoved,
	EventMaskLeftMouseDragged = 1ULL << EventTypeLeftMouseDragged,
	EventMaskRightMouseDragged = 1ULL << EventTypeRightMouseDragged,
	EventMaskMouseEntered = 1ULL << EventTypeMouseEntered,
	EventMaskMouseExited = 1ULL << EventTypeMouseExited,
	EventMaskKeyDown = 1ULL << EventTypeKeyDown,
	EventMaskKeyUp = 1ULL << EventTypeKeyUp,
	EventMaskFlagsChanged = 1ULL << EventTypeFlagsChanged,
	EventMaskAppKitDefined = 1ULL << EventTypeAppKitDefined,
	EventMaskSystemDefined = 1ULL << EventTypeSystemDefined,
	EventMaskApplicationDefined = 1ULL << EventTypeApplicationDefined,
	EventMaskPeriodic = 1ULL << EventTypePeriodic,
	EventMaskCursorUpdate = 1ULL << EventTypeCursorUpdate,
	EventMaskScrollWheel = 1ULL << EventTypeScrollWheel,
	EventMaskTabletPoint = 1ULL << EventTypeTabletPoint,
	EventMaskTabletProximity = 1ULL << EventTypeTabletProximity,
	EventMaskOtherMouseDown = 1ULL << EventTypeOtherMouseDown,
	EventMaskOtherMouseUp = 1ULL << EventTypeOtherMouseUp,
	EventMaskOtherMouseDragged = 1ULL << EventTypeOtherMouseDragged,
	EventMaskGesture = 1ULL << EventTypeGesture,
	EventMaskMagnify = 1ULL << EventTypeMagnify,
	EventMaskSwipe = 1ULL << EventTypeSwipe,
	EventMaskRotate = 1ULL << EventTypeRotate,
	EventMaskBeginGesture = 1ULL << EventTypeBeginGesture,
	EventMaskEndGesture = 1ULL << EventTypeEndGesture,
	EventMaskSmartMagnify = 1ULL << EventTypeSmartMagnify,
	EventMaskPressure = 1ULL << EventTypePressure,
	EventMaskDirectTouch = 1ULL << EventTypeDirectTouch,
	EventMaskChangeMode = 1ULL << EventTypeChangeMode,
	EventMaskMouseCancelled = 1ULL << EventTypeMouseCancelled,
	EventMaskAny = UIntegerMax,
};

// Event Modifier Flags (using existing KeyEquivalentModifierMask from NSMenuItem)
using EventModifierFlags = UInteger;


// Pointing Device Type
using PointingDeviceType = UInteger;

enum : PointingDeviceType {
	PointingDeviceTypeUnknown = 0,
	PointingDeviceTypePen = 1,
	PointingDeviceTypeCursor = 2,
	PointingDeviceTypeEraser = 3
};

// Event Button Mask
using EventButtonMask = UInteger;

enum : EventButtonMask {
	EventButtonMaskPenTip = 1 << 0,
	EventButtonMaskPenLowerSide = 1 << 1,
	EventButtonMaskPenUpperSide = 1 << 2
};

// Event Phase
using EventPhase = UInteger;

enum : EventPhase {
	EventPhaseNone = 0,
	EventPhaseBegan = 0x1 << 0,
	EventPhaseStationary = 0x1 << 1,
	EventPhaseChanged = 0x1 << 2,
	EventPhaseEnded = 0x1 << 3,
	EventPhaseCancelled = 0x1 << 4,
	EventPhaseMayBegin = 0x1 << 5,
};

// Event Subtype
using EventSubtype = short;

enum : EventSubtype {
	EventSubtypeWindowExposed = 0,
	EventSubtypeApplicationActivated = 1,
	EventSubtypeApplicationDeactivated = 2,
	EventSubtypeWindowMoved = 4,
	EventSubtypeScreenChanged = 8,
	EventSubtypePowerOff = 1,
	EventSubtypeMouseEvent = 0,
	EventSubtypeTabletPoint = 1,
	EventSubtypeTabletProximity = 2,
	EventSubtypeTouch = 3
};

// Pressure Behavior
using PressureBehavior = Integer;

enum : PressureBehavior {
	PressureBehaviorUnknown = -1,
	PressureBehaviorPrimaryDefault = 0,
	PressureBehaviorPrimaryClick = 1,
	PressureBehaviorPrimaryGeneric = 2,
	PressureBehaviorPrimaryAccelerator = 3,
	PressureBehaviorPrimaryDeepClick = 5,
	PressureBehaviorPrimaryDeepDrag = 6,
};

class Event: public NS::Referencing<Event> {
public:
	// Event Type and Properties
	[[nodiscard]] EventType type() const;

	[[nodiscard]] EventModifierFlags modifierFlags() const;

	[[nodiscard]] TimeInterval timestamp() const;

	[[nodiscard]] Window* window() const;

	[[nodiscard]] Integer windowNumber() const;

	// Mouse Event Properties
	[[nodiscard]] Integer clickCount() const;

	[[nodiscard]] Integer buttonNumber() const;

	[[nodiscard]] Integer eventNumber() const;

	[[nodiscard]] float pressure() const;

	[[nodiscard]] CGPoint locationInWindow() const;

	// Scroll Wheel Properties
	[[nodiscard]] CGFloat deltaX() const;

	[[nodiscard]] CGFloat deltaY() const;

	[[nodiscard]] CGFloat deltaZ() const;

	[[nodiscard]] bool hasPreciseScrollingDeltas() const;

	[[nodiscard]] CGFloat scrollingDeltaX() const;

	[[nodiscard]] CGFloat scrollingDeltaY() const;

	[[nodiscard]] EventPhase momentumPhase() const;

	[[nodiscard]] bool isDirectionInvertedFromDevice() const;

	// Key Event Properties
	[[nodiscard]] String* characters() const;

	[[nodiscard]] String* charactersIgnoringModifiers() const;

	[[nodiscard]] String* charactersByApplyingModifiers(EventModifierFlags modifiers) const;

	[[nodiscard]] bool isARepeat() const;

	[[nodiscard]] unsigned short keyCode() const;

	// Tracking Event Properties
	[[nodiscard]] Integer trackingNumber() const;

	[[nodiscard]] void* userData() const;

	// Generic Event Properties
	[[nodiscard]] EventSubtype subtype() const;

	[[nodiscard]] Integer data1() const;

	[[nodiscard]] Integer data2() const;

	// Magnification and Rotation
	[[nodiscard]] CGFloat magnification() const;

	[[nodiscard]] float rotation() const;

	// Tablet Event Properties
	[[nodiscard]] UInteger deviceID() const;

	[[nodiscard]] Integer absoluteX() const;

	[[nodiscard]] Integer absoluteY() const;

	[[nodiscard]] Integer absoluteZ() const;

	[[nodiscard]] EventButtonMask buttonMask() const;

	[[nodiscard]] CGPoint tilt() const;

	[[nodiscard]] float tangentialPressure() const;

	[[nodiscard]] UInteger vendorID() const;

	[[nodiscard]] UInteger tabletID() const;

	[[nodiscard]] UInteger pointingDeviceID() const;

	[[nodiscard]] UInteger systemTabletID() const;

	[[nodiscard]] UInteger vendorPointingDeviceType() const;

	[[nodiscard]] UInteger pointingDeviceSerialNumber() const;

	[[nodiscard]] unsigned long long uniqueID() const;

	[[nodiscard]] UInteger capabilityMask() const;

	[[nodiscard]] PointingDeviceType pointingDeviceType() const;

	[[nodiscard]] bool isEnteringProximity() const;

	// Gesture Event Properties
	[[nodiscard]] EventPhase phase() const;

	// Pressure Event Properties
	[[nodiscard]] Integer stage() const;

	[[nodiscard]] CGFloat stageTransition() const;

	[[nodiscard]] EventMask associatedEventsMask() const;

	[[nodiscard]] PressureBehavior pressureBehavior() const;

	// Class Methods
	static CGPoint mouseLocation();

	static EventModifierFlags currentModifierFlags();

	static UInteger pressedMouseButtons();

	static TimeInterval doubleClickInterval();

	static TimeInterval keyRepeatDelay();

	static TimeInterval keyRepeatInterval();

	static bool isMouseCoalescingEnabled();

	static void setMouseCoalescingEnabled(bool enabled);

	static bool isSwipeTrackingFromScrollEventsEnabled();

	static Event* otherEventWithType(EventType type, CGPoint location, EventModifierFlags modifierFlags, double timestamp,
		Integer windowNumber, void* context, Integer subtype, Integer data1,Integer data2);
};
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// Implementation
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::EventType NS::Event::type() const {
	return sendMessage<EventType>(this, _NS_PRIVATE_SEL(type));
}

_NS_INLINE NS::EventModifierFlags NS::Event::modifierFlags() const {
	return sendMessage<EventModifierFlags>(this, _NS_PRIVATE_SEL(modifierFlags));
}

_NS_INLINE NS::TimeInterval NS::Event::timestamp() const {
	return sendMessage<TimeInterval>(this, _NS_PRIVATE_SEL(timestamp));
}

_NS_INLINE NS::Window* NS::Event::window() const {
	return sendMessage<Window*>(this, _NS_PRIVATE_SEL(window));
}

_NS_INLINE NS::Integer NS::Event::windowNumber() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(windowNumber));
}

_NS_INLINE NS::Integer NS::Event::clickCount() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(clickCount));
}

_NS_INLINE NS::Integer NS::Event::buttonNumber() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(buttonNumber));
}

_NS_INLINE NS::Integer NS::Event::eventNumber() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(eventNumber));
}

_NS_INLINE float NS::Event::pressure() const {
	return sendMessage<float>(this, _NS_PRIVATE_SEL(pressure));
}

_NS_INLINE CGPoint NS::Event::locationInWindow() const {
	return sendMessage<CGPoint>(this, _NS_PRIVATE_SEL(locationInWindow));
}

_NS_INLINE CGFloat NS::Event::deltaX() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(deltaX));
}

_NS_INLINE CGFloat NS::Event::deltaY() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(deltaY));
}

_NS_INLINE CGFloat NS::Event::deltaZ() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(deltaZ));
}

_NS_INLINE bool NS::Event::hasPreciseScrollingDeltas() const {
	return sendMessage<bool>(this, _NS_PRIVATE_SEL(hasPreciseScrollingDeltas));
}

_NS_INLINE CGFloat NS::Event::scrollingDeltaX() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(scrollingDeltaX));
}

_NS_INLINE CGFloat NS::Event::scrollingDeltaY() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(scrollingDeltaY));
}

_NS_INLINE NS::EventPhase NS::Event::momentumPhase() const {
	return sendMessage<EventPhase>(this, _NS_PRIVATE_SEL(momentumPhase));
}

_NS_INLINE bool NS::Event::isDirectionInvertedFromDevice() const {
	return sendMessage<bool>(this, _NS_PRIVATE_SEL(isDirectionInvertedFromDevice));
}

_NS_INLINE NS::String* NS::Event::characters() const {
	return sendMessage<String*>(this, _NS_PRIVATE_SEL(characters));
}

_NS_INLINE NS::String* NS::Event::charactersIgnoringModifiers() const {
	return sendMessage<String*>(this, _NS_PRIVATE_SEL(charactersIgnoringModifiers));
}

_NS_INLINE NS::String* NS::Event::charactersByApplyingModifiers(const EventModifierFlags modifiers) const {
	return sendMessage<String*>(this, _NS_PRIVATE_SEL(charactersByApplyingModifiers_), modifiers);
}

_NS_INLINE bool NS::Event::isARepeat() const {
	return sendMessage<bool>(this, _NS_PRIVATE_SEL(isARepeat));
}

_NS_INLINE unsigned short NS::Event::keyCode() const {
	return sendMessage<unsigned short>(this, _NS_PRIVATE_SEL(keyCode));
}

_NS_INLINE NS::Integer NS::Event::trackingNumber() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(trackingNumber));
}

_NS_INLINE void* NS::Event::userData() const {
	return sendMessage<void*>(this, _NS_PRIVATE_SEL(userData));
}

_NS_INLINE NS::EventSubtype NS::Event::subtype() const {
	return sendMessage<EventSubtype>(this, _NS_PRIVATE_SEL(subtype));
}

_NS_INLINE NS::Integer NS::Event::data1() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(data1));
}

_NS_INLINE NS::Integer NS::Event::data2() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(data2));
}

_NS_INLINE CGFloat NS::Event::magnification() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(magnification));
}

_NS_INLINE float NS::Event::rotation() const {
	return sendMessage<float>(this, _NS_PRIVATE_SEL(rotation));
}

_NS_INLINE NS::UInteger NS::Event::deviceID() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(deviceID));
}

_NS_INLINE NS::Integer NS::Event::absoluteX() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(absoluteX));
}

_NS_INLINE NS::Integer NS::Event::absoluteY() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(absoluteY));
}

_NS_INLINE NS::Integer NS::Event::absoluteZ() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(absoluteZ));
}

_NS_INLINE NS::EventButtonMask NS::Event::buttonMask() const {
	return sendMessage<EventButtonMask>(this, _NS_PRIVATE_SEL(buttonMask));
}

_NS_INLINE CGPoint NS::Event::tilt() const {
	return sendMessage<CGPoint>(this, _NS_PRIVATE_SEL(tilt));
}

_NS_INLINE float NS::Event::tangentialPressure() const {
	return sendMessage<float>(this, _NS_PRIVATE_SEL(tangentialPressure));
}

_NS_INLINE NS::UInteger NS::Event::vendorID() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(vendorID));
}

_NS_INLINE NS::UInteger NS::Event::tabletID() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(tabletID));
}

_NS_INLINE NS::UInteger NS::Event::pointingDeviceID() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(pointingDeviceID));
}

_NS_INLINE NS::UInteger NS::Event::systemTabletID() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(systemTabletID));
}

_NS_INLINE NS::UInteger NS::Event::vendorPointingDeviceType() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(vendorPointingDeviceType));
}

_NS_INLINE NS::UInteger NS::Event::pointingDeviceSerialNumber() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(pointingDeviceSerialNumber));
}

_NS_INLINE unsigned long long NS::Event::uniqueID() const {
	return sendMessage<unsigned long long>(this, _NS_PRIVATE_SEL(uniqueID));
}

_NS_INLINE NS::UInteger NS::Event::capabilityMask() const {
	return sendMessage<UInteger>(this, _NS_PRIVATE_SEL(capabilityMask));
}

_NS_INLINE NS::PointingDeviceType NS::Event::pointingDeviceType() const {
	return sendMessage<PointingDeviceType>(this, _NS_PRIVATE_SEL(pointingDeviceType));
}

_NS_INLINE bool NS::Event::isEnteringProximity() const {
	return sendMessage<bool>(this, _NS_PRIVATE_SEL(isEnteringProximity));
}

_NS_INLINE NS::EventPhase NS::Event::phase() const {
	return sendMessage<EventPhase>(this, _NS_PRIVATE_SEL(phase));
}

_NS_INLINE NS::Integer NS::Event::stage() const {
	return sendMessage<Integer>(this, _NS_PRIVATE_SEL(stage));
}

_NS_INLINE CGFloat NS::Event::stageTransition() const {
	return sendMessage<CGFloat>(this, _NS_PRIVATE_SEL(stageTransition));
}

_NS_INLINE NS::EventMask NS::Event::associatedEventsMask() const {
	return sendMessage<EventMask>(this, _NS_PRIVATE_SEL(associatedEventsMask));
}

_NS_INLINE NS::PressureBehavior NS::Event::pressureBehavior() const {
	return sendMessage<PressureBehavior>(this, _NS_PRIVATE_SEL(pressureBehavior));
}

_NS_INLINE CGPoint NS::Event::mouseLocation() {
	return sendMessage<CGPoint>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(mouseLocation));
}

_NS_INLINE NS::EventModifierFlags NS::Event::currentModifierFlags() {
	return sendMessage<EventModifierFlags>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(modifierFlags));
}

_NS_INLINE NS::UInteger NS::Event::pressedMouseButtons() {
	return sendMessage<UInteger>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(pressedMouseButtons));
}

_NS_INLINE NS::TimeInterval NS::Event::doubleClickInterval() {
	return sendMessage<TimeInterval>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(doubleClickInterval));
}

_NS_INLINE NS::TimeInterval NS::Event::keyRepeatDelay() {
	return sendMessage<TimeInterval>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(keyRepeatDelay));
}

_NS_INLINE NS::TimeInterval NS::Event::keyRepeatInterval() {
	return sendMessage<TimeInterval>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(keyRepeatInterval));
}

_NS_INLINE bool NS::Event::isMouseCoalescingEnabled() {
	return sendMessage<bool>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(isMouseCoalescingEnabled));
}

_NS_INLINE void NS::Event::setMouseCoalescingEnabled(const bool enabled) {
	sendMessage<void>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(setMouseCoalescingEnabled_), enabled);
}

_NS_INLINE bool NS::Event::isSwipeTrackingFromScrollEventsEnabled() {
	return sendMessage<bool>(
		_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(isSwipeTrackingFromScrollEventsEnabled));
}

_NS_INLINE NS::Event* NS::Event::otherEventWithType(const EventType type, const CGPoint location, const EventModifierFlags modifierFlags,
													const double timestamp, const Integer windowNumber, void* context, const Integer subtype,
													const Integer data1, const Integer data2) {
	return sendMessage<Event*>(_APPKIT_PRIVATE_CLS(NSEvent), _NS_PRIVATE_SEL(otherEventWithType_location_modifierFlags_timestamp_windowNumber_context_subtype_data1_data2_),
								type, location, modifierFlags, timestamp, windowNumber, context, subtype, data1, data2);
}
