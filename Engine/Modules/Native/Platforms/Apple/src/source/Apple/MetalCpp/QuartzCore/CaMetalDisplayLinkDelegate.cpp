//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/QuartzCore
// File: CaMetalDisplayLinkDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp"

#include <cassert>

namespace CE::Native {

void CaMetalDisplayLinkDelegate::metalDisplayLinkNeedsUpdate(CA::MetalDisplayLink* metalDisplayLink, CA::MetalDisplayLinkUpdate* metalDisplayLinkUpdate) {
	assert(_metalDisplayLinkNeedsUpdateDispatcher.IsBound() and "CaMetalDisplayLinkDelegate::metalDisplayLinkNeedsUpdate: The delegate is not bound.");
	_metalDisplayLinkNeedsUpdateDispatcher.Execute(metalDisplayLink, metalDisplayLinkUpdate);
}

void CaMetalDisplayLinkDelegate::SetMetalDisplayLinkNeedsUpdateDelegate(const EventDelegate<CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*>& delegate) {
	assert(delegate.IsValid() and "CaMetalDisplayLinkDelegate::SetMetalDisplayLinkNeedsUpdateDelegate: The delegate is not valid");
	_metalDisplayLinkNeedsUpdateDispatcher.Bind(delegate);
}

}
