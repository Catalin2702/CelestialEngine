//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/QuartzCore
// File: CaMetalDisplayLinkDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <QuartzCore/CAMetalDisplayLink.hpp>

namespace CE::Native {

class CE_API CaMetalDisplayLinkDelegate: public CA::I_MetalDisplayLinkDelegate {
public:
	CaMetalDisplayLinkDelegate() = default;

	~CaMetalDisplayLinkDelegate() override = default;

public:
	void metalDisplayLinkNeedsUpdate(CA::MetalDisplayLink* metalDisplayLink, CA::MetalDisplayLinkUpdate* metalDisplayLinkUpdate) override;

public:
	void SetMetalDisplayLinkNeedsUpdateDelegate(const EventDelegate<CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*>& delegate);

private:
	CallbackDispatcher<void, CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*> _metalDisplayLinkNeedsUpdateDispatcher;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_QUARTZCORE_CAMETALDISPLAYLINKDELEGATE_HPP
