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
// AppKit/NSWindow.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"
#include "NSView.hpp"

#include "MetalCpp/Foundation/NSObject.hpp"
#include "MetalCpp/Metal/MTLEvent.hpp"

namespace NS {
class Screen;
}


namespace NS {

class Window : public Referencing<Window> {
public:
	static Window* alloc();
	void close() const;
	[[nodiscard]] Window* init(const CGRect& contentRect, WindowStyleMask styleMask, BackingStoreType backing, bool defer) const;

	void makeKeyAndOrderFront(const Object* pSender) const;

public:
	void setContentView(const View* pContentView) const;
	void setDelegate(const Object* pDelegate) const;
	void setFrameAutosaveName(const String* pName) const;
	void setFrame(const CGRect& frame, bool display = true) const;
	void setFrame(const CGRect& frame, bool display, bool animate) const;
	void setOpaque(bool flag) const;
	void setTitle(const String* pTitle) const;

public:
	[[nodiscard]] CGFloat backingScaleFactor() const;
	[[nodiscard]] View* contentView() const;
	[[nodiscard]] Object* delegate() const;
	[[nodiscard]] CGRect frame() const;
	[[nodiscard]] Screen* screen() const;
	[[nodiscard]] String* title() const;
};

}


_NS_INLINE NS::Window* NS::Window::alloc() {
	return sendMessage<Window*>(_APPKIT_PRIVATE_CLS(NSWindow), _NS_PRIVATE_SEL(alloc));
}

_NS_INLINE void NS::Window::close() const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(close));
}

_NS_INLINE NS::Window* NS::Window::init(const CGRect& contentRect, const WindowStyleMask styleMask, const BackingStoreType backing, const bool defer) const {
	return sendMessage<Window*>(this, _APPKIT_PRIVATE_SEL(initWithContentRect_styleMask_backing_defer_), contentRect, styleMask, backing, defer);
}

_NS_INLINE void NS::Window::makeKeyAndOrderFront(const Object* pSender) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(makeKeyAndOrderFront_), pSender);
}

_NS_INLINE void NS::Window::setContentView(const View* pContentView) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setContentView_), pContentView);
}

_NS_INLINE void NS::Window::setDelegate(const Object* pDelegate) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setDelegate_), pDelegate);
}

_NS_INLINE void NS::Window::setFrameAutosaveName(const String* pName) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setFrameAutosaveName_), pName);
}

_NS_INLINE void NS::Window::setFrame(const CGRect& frame, const bool display) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setFrame_display_), frame, display);
}

_NS_INLINE void NS::Window::setFrame(const CGRect& frame, const bool display, const bool animate) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setFrame_display_animate_), frame, display, animate);
}

_NS_INLINE void NS::Window::setOpaque(const bool flag) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setOpaque_), flag);
}

_NS_INLINE void NS::Window::setTitle(const String* pTitle) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setTitle_), pTitle);
}

_NS_INLINE CGFloat NS::Window::backingScaleFactor() const {
	return sendMessage<CGFloat>(this, _APPKIT_PRIVATE_SEL(backingScaleFactor));
}

_NS_INLINE NS::View* NS::Window::contentView() const {
	return sendMessage<View*>(this, _APPKIT_PRIVATE_SEL(contentView));
}

_NS_INLINE NS::Object* NS::Window::delegate() const {
	return sendMessage<Object*>(this, _APPKIT_PRIVATE_SEL(delegate));
}

_NS_INLINE CGRect NS::Window::frame() const {
	return sendMessage<CGRect>(this, _APPKIT_PRIVATE_SEL(frame));
}

_NS_INLINE NS::Screen* NS::Window::screen() const {
	return sendMessage<Screen*>(this, _APPKIT_PRIVATE_SEL(screen));
}

_NS_INLINE NS::String* NS::Window::title() const {
	return sendMessage<String*>(this, _APPKIT_PRIVATE_SEL(title_));
}
