//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/QuartzCore
// File: MetalDisplayLinkDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Apple/MetalCpp/QuartzCore/MetalDisplayLinkDelegate.hpp"

#include <utility>

namespace CA {

void MetalDisplayLinkDelegate::metalDisplayLinkNeedsUpdate(MetalDisplayLink* metalDisplayLink, MetalDisplayLinkUpdate* metalDisplayLinkUpdate) {
	if (_callback)
		_callback(metalDisplayLink, metalDisplayLinkUpdate);
}

void MetalDisplayLinkDelegate::SetMetalDisplayLinkNeedsUpdateCallback(MetalDisplayLinkDelegateNeedsUpdateCallback callback) {
	_callback = std::move(callback);
}

}
