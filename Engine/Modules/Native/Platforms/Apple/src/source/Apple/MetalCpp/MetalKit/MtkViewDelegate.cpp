//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/MetalKit
// File: MtkViewDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp"

#include <cassert>

namespace CE::Native {

void MtkViewDelegate::drawInMTKView(MTK::View* view) {
	assert(_drawInMtkViewDispatcher.IsBound() and "MtkViewDelegate::drawInMTKView: The delegate is not bound.");
	_drawInMtkViewDispatcher.Execute(view);
}

void MtkViewDelegate::drawableSizeWillChange(MTK::View* view, const CGSize size) {
	assert(_drawableSizeWillChangeDispatcher.IsBound() and "MtkViewDelegate::drawableSizeWillChange: The delegate is not bound.");
	_drawableSizeWillChangeDispatcher.Execute(view, size);
}

void MtkViewDelegate::SetDrawInMtkViewDelegate(const EventDelegate<MTK::View*>& delegate) {
	assert(delegate.IsValid() and "MtkViewDelegate::SetDrawInMtkViewDelegate: The delegate is not valid");
	_drawInMtkViewDispatcher.Bind(delegate);
}

void MtkViewDelegate::SetDrawableSizeWillChangeDelegate(const EventDelegate<MTK::View*, CGSize>& delegate) {
	assert(delegate.IsValid() and "MtkViewDelegate::SetDrawableSizeWillChangeDelegate: The delegate is not valid");
	_drawableSizeWillChangeDispatcher.Bind(delegate);
}

}
