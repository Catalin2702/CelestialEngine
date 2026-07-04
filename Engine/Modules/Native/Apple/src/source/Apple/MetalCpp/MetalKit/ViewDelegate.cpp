//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/MetalKit
// File: ViewDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Apple/MetalCpp/MetalKit/ViewDelegate.hpp"

#include <utility>

namespace MTK {

void ViewDelegate::drawInMTKView(View* view) {
	if (_drawInMtkViewCallback)
		_drawInMtkViewCallback(view);
}

void ViewDelegate::drawableSizeWillChange(View* view, CGSize size) {
	if (_drawableSizeWillChangeCallback)
		_drawableSizeWillChangeCallback(view, size);
}

void ViewDelegate::SetDrawInMtkViewCallback(ViewDelegateDrawInMtkViewCallback callback) {
	_drawInMtkViewCallback = std::move(callback);
}

void ViewDelegate::SetDrawableSizeWillChange(ViewDelegateDrawableSizeWillChangeCallback callback) {
	_drawableSizeWillChangeCallback = std::move(callback);
}

}
