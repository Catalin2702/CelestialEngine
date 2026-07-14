//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/QuartzCore
// File: CaMetalDisplayLinkDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp"

#include <utility>

namespace CE::Native {

void CaMetalDisplayLinkDelegate::metalDisplayLinkNeedsUpdate(CA::MetalDisplayLink* metalDisplayLink, CA::MetalDisplayLinkUpdate* metalDisplayLinkUpdate) {
	if (_callback)
		_callback(metalDisplayLink, metalDisplayLinkUpdate);
}

void CaMetalDisplayLinkDelegate::SetMetalDisplayLinkNeedsUpdateCallback(MetalDisplayLinkDelegateNeedsUpdateCallback callback) {
	_callback = std::move(callback);
}

}
