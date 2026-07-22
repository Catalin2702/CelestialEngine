//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/MetalKit
// File: MtkViewDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <MetalKit/MetalKit.hpp>

namespace CE::Native {

/**
 * @class MtkViewDelegate
 * @brief Bridges the MTKView delegate callbacks (frame draw, drawable resize) to engine delegates
 * @details Installed as the MetalKit view's delegate: drawInMTKView drives the per-frame render (paced by the view /
 *			display link) and drawableSizeWillChange reports drawable resizes. Each callback forwards to the delegate
 *			set via the matching Set* method.
 */
class CE_API MtkViewDelegate: public MTK::I_ViewDelegate {
public:
	MtkViewDelegate() = default;

	~MtkViewDelegate() override = default;

public:
	/**
	 * @brief Native MTKView per-frame callback: forwards to the draw delegate
	 */
	void drawInMTKView(MTK::View* view) override;
	/**
	 * @brief Native MTKView resize callback: forwards to the resize delegate
	 */
	void drawableSizeWillChange(MTK::View* view, CGSize size) override;

public:
	/**
	 * @brief Sets the delegate invoked on every drawInMTKView callback (the per-frame draw)
	 */
	void SetDrawInMtkViewDelegate(const EventDelegate<MTK::View*>& delegate);

	/**
	 * @brief Sets the delegate invoked when the view's drawable size is about to change
	 */
	void SetDrawableSizeWillChangeDelegate(const EventDelegate<MTK::View*, CGSize>& delegate);

private:
	CallbackDispatcher<void, MTK::View*> _drawInMtkViewDispatcher;					///< Receiver of the per-frame draw callback
	CallbackDispatcher<void, MTK::View*, CGSize> _drawableSizeWillChangeDispatcher;	///< Receiver of the drawable resize callback
};

}

#endif //CE_NATIVE_APPLE_METALCPP_METALKIT_MTKVIEWDELEGATE_HPP
