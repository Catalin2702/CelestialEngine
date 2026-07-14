//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/MetalKit
// File: MtkViewDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP

#include "MetalKit/MetalKit.hpp"

#include <functional>

using ViewDelegateDrawInMtkViewCallback = std::function<void(MTK::View*)>;
using ViewDelegateDrawableSizeWillChangeCallback = std::function<void(MTK::View*, CGSize)>;

namespace CE::Native {

class MtkViewDelegate: public MTK::I_ViewDelegate {
public:
	MtkViewDelegate() = default;

	~MtkViewDelegate() override = default;

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

#endif //CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP
