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

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <MetalKit/MetalKit.hpp>

namespace CE::Native {

class CE_API MtkViewDelegate: public MTK::I_ViewDelegate {
public:
	MtkViewDelegate() = default;

	~MtkViewDelegate() override = default;

public:
	void drawInMTKView(MTK::View* view) override;
	void drawableSizeWillChange(MTK::View* view, CGSize size) override;

public:
	void SetDrawInMtkViewDelegate(const EventDelegate<MTK::View*>& delegate);
	void SetDrawableSizeWillChangeDelegate(const EventDelegate<MTK::View*, CGSize>& delegate);

private:
	CallbackDispatcher<void, MTK::View*> _drawInMtkViewDispatcher;
	CallbackDispatcher<void, MTK::View*, CGSize> _drawableSizeWillChangeDispatcher;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP
