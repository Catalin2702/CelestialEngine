//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/MetalKit
// File: ViewDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP

#include "MetalKit/MetalKit.hpp"

#include <functional>

namespace MTK {

class ViewDelegate: public I_ViewDelegate {
	using DrawInMtkViewCallback = std::function<void(View*)>;
	using DrawableSizeWillChangeCallback = std::function<void(View*, CGSize)>;
public:
	ViewDelegate() = default;

	~ViewDelegate() override = default;

public:
	void drawInMTKView(View* view) override;
	void drawableSizeWillChange(View* view, CGSize size) override;

public:
	void SetDrawInMtkViewCallback(DrawInMtkViewCallback callback);
	void SetDrawableSizeWillChange(DrawableSizeWillChangeCallback callback);

private:
	DrawInMtkViewCallback _drawInMtkViewCallback;
	DrawableSizeWillChangeCallback _drawableSizeWillChangeCallback;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_METALKIT_VIEWDELEGATE_HPP
