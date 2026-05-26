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
// AppKit/NSView.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

// ReSharper disable once CppUnusedIncludeDirective
#include "AppKitPrivate.hpp"
#include "Apple/MetalCpp/Foundation/NSObject.hpp"

#include <CoreGraphics/CGGeometry.h>

namespace CA {
	class MetalLayer;
}

namespace NS
{
	class View : public NS::Referencing< View >
	{
		public:
			static View* alloc();
			[[nodiscard]] View*	init(const CGRect& frame) const;
			void setLayer(CA::MetalLayer* layer) const;
			void setOpaque(bool opaque) const;
			void setWantsLayer(bool wantsLayer) const;
	};
}

_NS_INLINE NS::View* NS::View::alloc() {
	return Object::sendMessage< View* >( _APPKIT_PRIVATE_CLS( NSView ), _APPKIT_PRIVATE_SEL( alloc ) );
}

_NS_INLINE NS::View* NS::View::init(const CGRect& frame) const {
	return Object::sendMessage< View* >( this, _APPKIT_PRIVATE_SEL( initWithFrame_ ), frame );
}

_NS_INLINE void NS::View::setLayer(CA::MetalLayer* layer) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setLayer_ ), layer );
}

_NS_INLINE void NS::View::setOpaque(const bool opaque) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setOpaque_ ), opaque );
}

_NS_INLINE void NS::View::setWantsLayer(const bool wantsLayer) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setWantsLayer_ ), wantsLayer );
}
