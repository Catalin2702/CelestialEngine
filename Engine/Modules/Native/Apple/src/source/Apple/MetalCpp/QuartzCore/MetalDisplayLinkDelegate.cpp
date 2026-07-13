//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/QuartzCore
// File: MetalDisplayLinkDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Apple/MetalCpp/QuartzCore/MetalDisplayLinkDelegate.hpp"

#include <utility>

namespace CE::Native {

void MetalDisplayLinkDelegate::metalDisplayLinkNeedsUpdate(CA::MetalDisplayLink* metalDisplayLink, CA::MetalDisplayLinkUpdate* metalDisplayLinkUpdate) {
	if (_callback)
		_callback(metalDisplayLink, metalDisplayLinkUpdate);
}

void MetalDisplayLinkDelegate::SetMetalDisplayLinkNeedsUpdateCallback(MetalDisplayLinkDelegateNeedsUpdateCallback callback) {
	_callback = std::move(callback);
}

}
