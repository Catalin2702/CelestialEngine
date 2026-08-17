//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/QuartzCore
// File: CaMetalDisplayLinkDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP

#include "Define/DynamicLinker.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <QuartzCore/CAMetalDisplayLink.hpp>


namespace CE::Native {

/**
 * @class CaMetalDisplayLinkDelegate
 * @brief Bridges the CAMetalDisplayLink update callback to an engine delegate
 * @details Installed on the display link that paces frames while VSync is on: every metalDisplayLinkNeedsUpdate
 *			callback forwards to the delegate set via SetMetalDisplayLinkNeedsUpdateDelegate.
 */
class CE_APPLE_API CaMetalDisplayLinkDelegate: public CA::I_MetalDisplayLinkDelegate {
public:
	CaMetalDisplayLinkDelegate() = default;

	~CaMetalDisplayLinkDelegate() override = default;

public:
	/**
	 * @brief Native display link callback: forwards to the update delegate
	 */
	void metalDisplayLinkNeedsUpdate(CA::MetalDisplayLink* metalDisplayLink, CA::MetalDisplayLinkUpdate* metalDisplayLinkUpdate) override;

public:
	/**
	 * @brief Sets the delegate invoked on every display link update (once per paced frame)
	 */
	void SetMetalDisplayLinkNeedsUpdateDelegate(const EventDelegate<CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*>& delegate);

private:
	CallbackDispatcher<void, CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*> _metalDisplayLinkNeedsUpdateDispatcher;	///< Receiver of the display link update callback
};

}

#endif //CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP
