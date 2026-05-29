
//
// QuartzCore/CAMetalDrawable.hpp
//
// Copyright 2020-2024 Apple Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include "Apple/MetalCpp/Foundation/NSTypes.hpp"
#include "Apple/MetalCpp/Metal/MTLPixelFormat.hpp"
#include "Apple/MetalCpp/Metal/MTLResidencySet.hpp"

#include <CoreGraphics/CGColorSpace.h>

#include "CADefines.hpp"
#include "CAMetalDrawable.hpp"
#include "CAPrivate.hpp"


namespace CA {

class MetalLayer: public NS::Referencing<MetalLayer> {
public:
	void setAllowsNextDrawableTimeout(bool allowsNextDrawableTimeout);

	void setColorspace(CGColorSpaceRef colorspace);

	void setContentsScale(CGFloat contentsScale);

	void setDevice(MTL::Device* device);

	void setDisplaySyncEnabled(bool displaySyncEnabled);

	void setDrawableSize(CGSize drawableSize);

	void setFramebufferOnly(bool framebufferOnly);

	void setMaximumDrawableCount(NS::UInteger maximumDrawableCount);

	void setPixelFormat(MTL::PixelFormat pixelFormat);

public:
	[[nodiscard]] static MetalLayer* layer();

public:
	[[nodiscard]] bool allowsNextDrawableTimeout() const;

	[[nodiscard]] CGColorSpaceRef colorspace() const;

	[[nodiscard]] CGFloat contentsScale() const;

	[[nodiscard]] MTL::Device* device() const;

	[[nodiscard]] bool displaySyncEnabled() const;

	[[nodiscard]] CGSize drawableSize() const;

	[[nodiscard]] bool framebufferOnly() const;

	[[nodiscard]] NS::UInteger maximumDrawableCount() const;

	[[nodiscard]] MetalDrawable* nextDrawable() const;

	[[nodiscard]] MTL::PixelFormat pixelFormat() const;

	[[nodiscard]] MTL::ResidencySet* residencySet() const;
};


// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setAllowsNextDrawableTimeout(bool allowsNextDrawableTimeout) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setAllowsNextDrawableTimeout_), allowsNextDrawableTimeout);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setColorspace(CGColorSpaceRef colorspace) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setColorspace_), colorspace);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setContentsScale(const CGFloat contentsScale) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setContentsScale_), contentsScale);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setDevice(MTL::Device* device) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setDevice_), device);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setDisplaySyncEnabled(bool displaySyncEnabled) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setDisplaySyncEnabled_), displaySyncEnabled);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setDrawableSize(CGSize drawableSize) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setDrawableSize_), drawableSize);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setFramebufferOnly(bool framebufferOnly) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setFramebufferOnly_), framebufferOnly);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setMaximumDrawableCount(NS::UInteger maximumDrawableCount) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setMaximumDrawableCount_), maximumDrawableCount);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_CA_INLINE void MetalLayer::setPixelFormat(MTL::PixelFormat pixelFormat) {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setPixelFormat_), pixelFormat);
}

_CA_INLINE MetalLayer* MetalLayer::layer() {
	return sendMessage<MetalLayer*>(_CA_PRIVATE_CLS(CAMetalLayer), _CA_PRIVATE_SEL(layer));
}

_CA_INLINE bool MetalLayer::allowsNextDrawableTimeout() const {
	return sendMessage<bool>(this, _CA_PRIVATE_SEL(allowsNextDrawableTimeout));
}

_CA_INLINE CGColorSpaceRef MetalLayer::colorspace() const {
	return sendMessage<CGColorSpaceRef>(this, _CA_PRIVATE_SEL(colorspace));
}

_CA_INLINE CGFloat MetalLayer::contentsScale() const {
	return sendMessage<CGFloat>(this, _CA_PRIVATE_SEL(contentsScale));
}

_CA_INLINE MTL::Device* MetalLayer::device() const {
	return sendMessage<MTL::Device*>(this, _CA_PRIVATE_SEL(device));
}

_CA_INLINE bool MetalLayer::displaySyncEnabled() const {
	return sendMessage<bool>(this, _CA_PRIVATE_SEL(displaySyncEnabled));
}

_CA_INLINE CGSize MetalLayer::drawableSize() const {
	return sendMessage<CGSize>(this, _CA_PRIVATE_SEL(drawableSize));
}

_CA_INLINE bool MetalLayer::framebufferOnly() const {
	return sendMessage<bool>(this, _CA_PRIVATE_SEL(framebufferOnly));
}

_CA_INLINE NS::UInteger MetalLayer::maximumDrawableCount() const {
	return sendMessage<NS::UInteger>(this, _CA_PRIVATE_SEL(maximumDrawableCount));
}

_CA_INLINE MetalDrawable* MetalLayer::nextDrawable() const {
	return sendMessage<MetalDrawable*>(this, _CA_PRIVATE_SEL(nextDrawable));
}

_CA_INLINE MTL::PixelFormat MetalLayer::pixelFormat() const {
	return sendMessage<MTL::PixelFormat>(this, _CA_PRIVATE_SEL(pixelFormat));
}

_CA_INLINE MTL::ResidencySet* MetalLayer::residencySet() const {
	return sendMessage<MTL::ResidencySet*>(this, _CA_PRIVATE_SEL(residencySet));
}

} // namespace CA
