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
#include <Foundation/NSTypes.hpp>
#include <CoreGraphics/CGGeometry.h>

namespace NS
{
	class Window;
	class String;

	// Event Types
	using EventType = UInteger;

	enum : EventType {
		EventTypeLeftMouseDown             = 1,
		EventTypeLeftMouseUp               = 2,
		EventTypeRightMouseDown            = 3,
		EventTypeRightMouseUp              = 4,
		EventTypeMouseMoved                = 5,
		EventTypeLeftMouseDragged          = 6,
		EventTypeRightMouseDragged         = 7,
		EventTypeMouseEntered              = 8,
		EventTypeMouseExited               = 9,
		EventTypeKeyDown                   = 10,
		EventTypeKeyUp                     = 11,
		EventTypeFlagsChanged              = 12,
		EventTypeAppKitDefined             = 13,
		EventTypeSystemDefined             = 14,
		EventTypeApplicationDefined        = 15,
		EventTypePeriodic                  = 16,
		EventTypeCursorUpdate              = 17,
		EventTypeScrollWheel               = 22,
		EventTypeTabletPoint               = 23,
		EventTypeTabletProximity           = 24,
		EventTypeOtherMouseDown            = 25,
		EventTypeOtherMouseUp              = 26,
		EventTypeOtherMouseDragged         = 27,
		EventTypeGesture                   = 29,
		EventTypeMagnify                   = 30,
		EventTypeSwipe                     = 31,
		EventTypeRotate                    = 18,
		EventTypeBeginGesture              = 19,
		EventTypeEndGesture                = 20,
		EventTypeSmartMagnify              = 32,
		EventTypeQuickLook                 = 33,
		EventTypePressure                  = 34,
		EventTypeDirectTouch               = 37,
		EventTypeChangeMode                = 38,
		EventTypeMouseCancelled            = 40,
	};

	// Event Mask
	using EventMask = unsigned long long;

	enum : EventMask {
		EventMaskLeftMouseDown         = 1ULL << EventTypeLeftMouseDown,
		EventMaskLeftMouseUp           = 1ULL << EventTypeLeftMouseUp,
		EventMaskRightMouseDown        = 1ULL << EventTypeRightMouseDown,
		EventMaskRightMouseUp          = 1ULL << EventTypeRightMouseUp,
		EventMaskMouseMoved            = 1ULL << EventTypeMouseMoved,
		EventMaskLeftMouseDragged      = 1ULL << EventTypeLeftMouseDragged,
		EventMaskRightMouseDragged     = 1ULL << EventTypeRightMouseDragged,
		EventMaskMouseEntered          = 1ULL << EventTypeMouseEntered,
		EventMaskMouseExited           = 1ULL << EventTypeMouseExited,
		EventMaskKeyDown               = 1ULL << EventTypeKeyDown,
		EventMaskKeyUp                 = 1ULL << EventTypeKeyUp,
		EventMaskFlagsChanged          = 1ULL << EventTypeFlagsChanged,
		EventMaskAppKitDefined         = 1ULL << EventTypeAppKitDefined,
		EventMaskSystemDefined         = 1ULL << EventTypeSystemDefined,
		EventMaskApplicationDefined    = 1ULL << EventTypeApplicationDefined,
		EventMaskPeriodic              = 1ULL << EventTypePeriodic,
		EventMaskCursorUpdate          = 1ULL << EventTypeCursorUpdate,
		EventMaskScrollWheel           = 1ULL << EventTypeScrollWheel,
		EventMaskTabletPoint           = 1ULL << EventTypeTabletPoint,
		EventMaskTabletProximity       = 1ULL << EventTypeTabletProximity,
		EventMaskOtherMouseDown        = 1ULL << EventTypeOtherMouseDown,
		EventMaskOtherMouseUp          = 1ULL << EventTypeOtherMouseUp,
		EventMaskOtherMouseDragged     = 1ULL << EventTypeOtherMouseDragged,
		EventMaskGesture               = 1ULL << EventTypeGesture,
		EventMaskMagnify               = 1ULL << EventTypeMagnify,
		EventMaskSwipe                 = 1ULL << EventTypeSwipe,
		EventMaskRotate                = 1ULL << EventTypeRotate,
		EventMaskBeginGesture          = 1ULL << EventTypeBeginGesture,
		EventMaskEndGesture            = 1ULL << EventTypeEndGesture,
		EventMaskSmartMagnify          = 1ULL << EventTypeSmartMagnify,
		EventMaskPressure              = 1ULL << EventTypePressure,
		EventMaskDirectTouch           = 1ULL << EventTypeDirectTouch,
		EventMaskChangeMode            = 1ULL << EventTypeChangeMode,
		EventMaskMouseCancelled        = 1ULL << EventTypeMouseCancelled,
		EventMaskAny                   = UIntegerMax,
	};

	// Event Modifier Flags (using existing KeyEquivalentModifierMask from NSMenuItem)
	using EventModifierFlags = UInteger;


	// Pointing Device Type
	using PointingDeviceType = UInteger;

	enum : PointingDeviceType {
		PointingDeviceTypeUnknown     = 0,
		PointingDeviceTypePen         = 1,
		PointingDeviceTypeCursor      = 2,
		PointingDeviceTypeEraser      = 3
	};

	// Event Button Mask
	using EventButtonMask = UInteger;

	enum : EventButtonMask {
		EventButtonMaskPenTip                = 1 << 0,
		EventButtonMaskPenLowerSide          = 1 << 1,
		EventButtonMaskPenUpperSide          = 1 << 2
	};

	// Event Phase
	using EventPhase = UInteger;

	enum : EventPhase {
		EventPhaseNone        = 0,
		EventPhaseBegan       = 0x1 << 0,
		EventPhaseStationary  = 0x1 << 1,
		EventPhaseChanged     = 0x1 << 2,
		EventPhaseEnded       = 0x1 << 3,
		EventPhaseCancelled   = 0x1 << 4,
		EventPhaseMayBegin    = 0x1 << 5,
	};

	// Event Subtype
	using EventSubtype = short;

	enum : EventSubtype {
		EventSubtypeWindowExposed            = 0,
		EventSubtypeApplicationActivated     = 1,
		EventSubtypeApplicationDeactivated   = 2,
		EventSubtypeWindowMoved              = 4,
		EventSubtypeScreenChanged            = 8,
		EventSubtypePowerOff                 = 1,
		EventSubtypeMouseEvent               = 0,
		EventSubtypeTabletPoint              = 1,
		EventSubtypeTabletProximity          = 2,
		EventSubtypeTouch                    = 3
	};

	// Pressure Behavior
	using PressureBehavior = Integer;

	enum : PressureBehavior {
		PressureBehaviorUnknown                = -1,
		PressureBehaviorPrimaryDefault         = 0,
		PressureBehaviorPrimaryClick           = 1,
		PressureBehaviorPrimaryGeneric         = 2,
		PressureBehaviorPrimaryAccelerator     = 3,
		PressureBehaviorPrimaryDeepClick       = 5,
		PressureBehaviorPrimaryDeepDrag        = 6,
	};

	class Event : public NS::Referencing< Event >
	{
		public:
			// Event Type and Properties
			EventType				type() const;
			EventModifierFlags		modifierFlags() const;
			TimeInterval			timestamp() const;
			Window*					window() const;
			Integer					windowNumber() const;

			// Mouse Event Properties
			Integer					clickCount() const;
			Integer					buttonNumber() const;
			Integer					eventNumber() const;
			float					pressure() const;
			CGPoint					locationInWindow() const;

			// Scroll Wheel Properties
			CGFloat					deltaX() const;
			CGFloat					deltaY() const;
			CGFloat					deltaZ() const;
			bool					hasPreciseScrollingDeltas() const;
			CGFloat					scrollingDeltaX() const;
			CGFloat					scrollingDeltaY() const;
			EventPhase				momentumPhase() const;
			bool					isDirectionInvertedFromDevice() const;

			// Key Event Properties
			String*					characters() const;
			String*					charactersIgnoringModifiers() const;
			String*					charactersByApplyingModifiers(EventModifierFlags modifiers) const;
			bool					isARepeat() const;
			unsigned short			keyCode() const;

			// Tracking Event Properties
			Integer					trackingNumber() const;
			void*					userData() const;

			// Generic Event Properties
			EventSubtype			subtype() const;
			Integer					data1() const;
			Integer					data2() const;

			// Magnification and Rotation
			CGFloat					magnification() const;
			float					rotation() const;

			// Tablet Event Properties
			UInteger				deviceID() const;
			Integer					absoluteX() const;
			Integer					absoluteY() const;
			Integer					absoluteZ() const;
			EventButtonMask			buttonMask() const;
			CGPoint					tilt() const;
			float					tangentialPressure() const;
			UInteger				vendorID() const;
			UInteger				tabletID() const;
			UInteger				pointingDeviceID() const;
			UInteger				systemTabletID() const;
			UInteger				vendorPointingDeviceType() const;
			UInteger				pointingDeviceSerialNumber() const;
			unsigned long long		uniqueID() const;
			UInteger				capabilityMask() const;
			PointingDeviceType		pointingDeviceType() const;
			bool					isEnteringProximity() const;

			// Gesture Event Properties
			EventPhase				phase() const;

			// Pressure Event Properties
			Integer					stage() const;
			CGFloat					stageTransition() const;
			EventMask				associatedEventsMask() const;
			PressureBehavior		pressureBehavior() const;

			// Class Methods
			static CGPoint			mouseLocation();
			static EventModifierFlags	currentModifierFlags();
			static UInteger			pressedMouseButtons();
			static TimeInterval		doubleClickInterval();
			static TimeInterval		keyRepeatDelay();
			static TimeInterval		keyRepeatInterval();
			static bool				isMouseCoalescingEnabled();
			static void				setMouseCoalescingEnabled(bool enabled);
			static bool				isSwipeTrackingFromScrollEventsEnabled();
	};
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// Implementation
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::EventType NS::Event::type() const
{
	return Object::sendMessage< EventType >( this, _NS_PRIVATE_SEL( type ) );
}

_NS_INLINE NS::EventModifierFlags NS::Event::modifierFlags() const
{
	return Object::sendMessage< EventModifierFlags >( this, _NS_PRIVATE_SEL( modifierFlags ) );
}

_NS_INLINE NS::TimeInterval NS::Event::timestamp() const
{
	return Object::sendMessage< TimeInterval >( this, _NS_PRIVATE_SEL( timestamp ) );
}

_NS_INLINE NS::Window* NS::Event::window() const
{
	return Object::sendMessage< Window* >( this, _NS_PRIVATE_SEL( window ) );
}

_NS_INLINE NS::Integer NS::Event::windowNumber() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( windowNumber ) );
}

_NS_INLINE NS::Integer NS::Event::clickCount() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( clickCount ) );
}

_NS_INLINE NS::Integer NS::Event::buttonNumber() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( buttonNumber ) );
}

_NS_INLINE NS::Integer NS::Event::eventNumber() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( eventNumber ) );
}

_NS_INLINE float NS::Event::pressure() const
{
	return Object::sendMessage< float >( this, _NS_PRIVATE_SEL( pressure ) );
}

_NS_INLINE CGPoint NS::Event::locationInWindow() const
{
	return Object::sendMessage< CGPoint >( this, _NS_PRIVATE_SEL( locationInWindow ) );
}

_NS_INLINE CGFloat NS::Event::deltaX() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( deltaX ) );
}

_NS_INLINE CGFloat NS::Event::deltaY() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( deltaY ) );
}

_NS_INLINE CGFloat NS::Event::deltaZ() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( deltaZ ) );
}

_NS_INLINE bool NS::Event::hasPreciseScrollingDeltas() const
{
	return Object::sendMessage< bool >( this, _NS_PRIVATE_SEL( hasPreciseScrollingDeltas ) );
}

_NS_INLINE CGFloat NS::Event::scrollingDeltaX() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( scrollingDeltaX ) );
}

_NS_INLINE CGFloat NS::Event::scrollingDeltaY() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( scrollingDeltaY ) );
}

_NS_INLINE NS::EventPhase NS::Event::momentumPhase() const
{
	return Object::sendMessage< EventPhase >( this, _NS_PRIVATE_SEL( momentumPhase ) );
}

_NS_INLINE bool NS::Event::isDirectionInvertedFromDevice() const
{
	return Object::sendMessage< bool >( this, _NS_PRIVATE_SEL( isDirectionInvertedFromDevice ) );
}

_NS_INLINE NS::String* NS::Event::characters() const
{
	return Object::sendMessage< String* >( this, _NS_PRIVATE_SEL( characters ) );
}

_NS_INLINE NS::String* NS::Event::charactersIgnoringModifiers() const
{
	return Object::sendMessage< String* >( this, _NS_PRIVATE_SEL( charactersIgnoringModifiers ) );
}

_NS_INLINE NS::String* NS::Event::charactersByApplyingModifiers(EventModifierFlags modifiers) const
{
	return Object::sendMessage< String* >( this, _NS_PRIVATE_SEL( charactersByApplyingModifiers_ ), modifiers );
}

_NS_INLINE bool NS::Event::isARepeat() const
{
	return Object::sendMessage< bool >( this, _NS_PRIVATE_SEL( isARepeat ) );
}

_NS_INLINE unsigned short NS::Event::keyCode() const
{
	return Object::sendMessage< unsigned short >( this, _NS_PRIVATE_SEL( keyCode ) );
}

_NS_INLINE NS::Integer NS::Event::trackingNumber() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( trackingNumber ) );
}

_NS_INLINE void* NS::Event::userData() const
{
	return Object::sendMessage< void* >( this, _NS_PRIVATE_SEL( userData ) );
}

_NS_INLINE NS::EventSubtype NS::Event::subtype() const
{
	return Object::sendMessage< EventSubtype >( this, _NS_PRIVATE_SEL( subtype ) );
}

_NS_INLINE NS::Integer NS::Event::data1() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( data1 ) );
}

_NS_INLINE NS::Integer NS::Event::data2() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( data2 ) );
}

_NS_INLINE CGFloat NS::Event::magnification() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( magnification ) );
}

_NS_INLINE float NS::Event::rotation() const
{
	return Object::sendMessage< float >( this, _NS_PRIVATE_SEL( rotation ) );
}

_NS_INLINE NS::UInteger NS::Event::deviceID() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( deviceID ) );
}

_NS_INLINE NS::Integer NS::Event::absoluteX() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( absoluteX ) );
}

_NS_INLINE NS::Integer NS::Event::absoluteY() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( absoluteY ) );
}

_NS_INLINE NS::Integer NS::Event::absoluteZ() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( absoluteZ ) );
}

_NS_INLINE NS::EventButtonMask NS::Event::buttonMask() const
{
	return Object::sendMessage< EventButtonMask >( this, _NS_PRIVATE_SEL( buttonMask ) );
}

_NS_INLINE CGPoint NS::Event::tilt() const
{
	return Object::sendMessage< CGPoint >( this, _NS_PRIVATE_SEL( tilt ) );
}

_NS_INLINE float NS::Event::tangentialPressure() const
{
	return Object::sendMessage< float >( this, _NS_PRIVATE_SEL( tangentialPressure ) );
}

_NS_INLINE NS::UInteger NS::Event::vendorID() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( vendorID ) );
}

_NS_INLINE NS::UInteger NS::Event::tabletID() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( tabletID ) );
}

_NS_INLINE NS::UInteger NS::Event::pointingDeviceID() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( pointingDeviceID ) );
}

_NS_INLINE NS::UInteger NS::Event::systemTabletID() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( systemTabletID ) );
}

_NS_INLINE NS::UInteger NS::Event::vendorPointingDeviceType() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( vendorPointingDeviceType ) );
}

_NS_INLINE NS::UInteger NS::Event::pointingDeviceSerialNumber() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( pointingDeviceSerialNumber ) );
}

_NS_INLINE unsigned long long NS::Event::uniqueID() const
{
	return Object::sendMessage< unsigned long long >( this, _NS_PRIVATE_SEL( uniqueID ) );
}

_NS_INLINE NS::UInteger NS::Event::capabilityMask() const
{
	return Object::sendMessage< UInteger >( this, _NS_PRIVATE_SEL( capabilityMask ) );
}

_NS_INLINE NS::PointingDeviceType NS::Event::pointingDeviceType() const
{
	return Object::sendMessage< PointingDeviceType >( this, _NS_PRIVATE_SEL( pointingDeviceType ) );
}

_NS_INLINE bool NS::Event::isEnteringProximity() const
{
	return Object::sendMessage< bool >( this, _NS_PRIVATE_SEL( isEnteringProximity ) );
}

_NS_INLINE NS::EventPhase NS::Event::phase() const
{
	return Object::sendMessage< EventPhase >( this, _NS_PRIVATE_SEL( phase ) );
}

_NS_INLINE NS::Integer NS::Event::stage() const
{
	return Object::sendMessage< Integer >( this, _NS_PRIVATE_SEL( stage ) );
}

_NS_INLINE CGFloat NS::Event::stageTransition() const
{
	return Object::sendMessage< CGFloat >( this, _NS_PRIVATE_SEL( stageTransition ) );
}

_NS_INLINE NS::EventMask NS::Event::associatedEventsMask() const
{
	return Object::sendMessage< EventMask >( this, _NS_PRIVATE_SEL( associatedEventsMask ) );
}

_NS_INLINE NS::PressureBehavior NS::Event::pressureBehavior() const
{
	return Object::sendMessage< PressureBehavior >( this, _NS_PRIVATE_SEL( pressureBehavior ) );
}

_NS_INLINE CGPoint NS::Event::mouseLocation()
{
	return Object::sendMessage< CGPoint >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( mouseLocation ) );
}

_NS_INLINE NS::EventModifierFlags NS::Event::currentModifierFlags()
{
	return Object::sendMessage< EventModifierFlags >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( modifierFlags ) );
}

_NS_INLINE NS::UInteger NS::Event::pressedMouseButtons()
{
	return Object::sendMessage< UInteger >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( pressedMouseButtons ) );
}

_NS_INLINE NS::TimeInterval NS::Event::doubleClickInterval()
{
	return Object::sendMessage< TimeInterval >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( doubleClickInterval ) );
}

_NS_INLINE NS::TimeInterval NS::Event::keyRepeatDelay()
{
	return Object::sendMessage< TimeInterval >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( keyRepeatDelay ) );
}

_NS_INLINE NS::TimeInterval NS::Event::keyRepeatInterval()
{
	return Object::sendMessage< TimeInterval >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( keyRepeatInterval ) );
}

_NS_INLINE bool NS::Event::isMouseCoalescingEnabled()
{
	return Object::sendMessage< bool >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( isMouseCoalescingEnabled ) );
}

_NS_INLINE void NS::Event::setMouseCoalescingEnabled(bool enabled)
{
	Object::sendMessage< void >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( setMouseCoalescingEnabled_ ), enabled );
}

_NS_INLINE bool NS::Event::isSwipeTrackingFromScrollEventsEnabled()
{
	return Object::sendMessage< bool >( _APPKIT_PRIVATE_CLS( NSEvent ), _NS_PRIVATE_SEL( isSwipeTrackingFromScrollEventsEnabled ) );
}

