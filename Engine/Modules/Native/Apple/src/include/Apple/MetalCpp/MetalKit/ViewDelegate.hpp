//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/MetalKit
// File: ViewDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP

#include "MetalKit/MetalKit.hpp"

#include <functional>

using ViewDelegateDrawInMtkViewCallback = std::function<void(MTK::View*)>;
using ViewDelegateDrawableSizeWillChangeCallback = std::function<void(MTK::View*, CGSize)>;

namespace CE::Native {

class ViewDelegate: public MTK::I_ViewDelegate {
public:
	ViewDelegate() = default;

	~ViewDelegate() override = default;

public:
	void drawInMTKView(MTK::View* view) override;
	void drawableSizeWillChange(MTK::View* view, CGSize size) override;

public:
	void SetDrawInMtkViewCallback(ViewDelegateDrawInMtkViewCallback callback);
	void SetDrawableSizeWillChange(ViewDelegateDrawableSizeWillChangeCallback callback);

private:
	ViewDelegateDrawInMtkViewCallback _drawInMtkViewCallback;
	ViewDelegateDrawableSizeWillChangeCallback _drawableSizeWillChangeCallback;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP
