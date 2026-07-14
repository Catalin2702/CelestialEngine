//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/MetalKit
// File: MtkViewDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp"

#include <utility>

namespace CE::Native {

void MtkViewDelegate::drawInMTKView(MTK::View* view) {
	if (_drawInMtkViewCallback)
		_drawInMtkViewCallback(view);
}

void MtkViewDelegate::drawableSizeWillChange(MTK::View* view, CGSize size) {
	if (_drawableSizeWillChangeCallback)
		_drawableSizeWillChangeCallback(view, size);
}

void MtkViewDelegate::SetDrawInMtkViewCallback(ViewDelegateDrawInMtkViewCallback callback) {
	_drawInMtkViewCallback = std::move(callback);
}

void MtkViewDelegate::SetDrawableSizeWillChange(ViewDelegateDrawableSizeWillChangeCallback callback) {
	_drawableSizeWillChangeCallback = std::move(callback);
}

}
