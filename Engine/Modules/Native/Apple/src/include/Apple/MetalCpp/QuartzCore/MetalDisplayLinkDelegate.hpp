//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/QuartzCore
// File: MetalDisplayLinkDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-04
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_QUARTZCORE_METALDISPLAYLINKDELEGATE_HPP
#define CE_NATIVE_APPLE_METALCPP_QUARTZCORE_METALDISPLAYLINKDELEGATE_HPP

#include <QuartzCore/CAMetalDisplayLink.hpp>

#include <functional>

using MetalDisplayLinkDelegateNeedsUpdateCallback = std::function<void(CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate*)>;

namespace CA {

class MetalDisplayLinkDelegate: public I_MetalDisplayLinkDelegate {
public:
	MetalDisplayLinkDelegate() = default;

	~MetalDisplayLinkDelegate() override = default;

public:
	void metalDisplayLinkNeedsUpdate(MetalDisplayLink* metalDisplayLink, MetalDisplayLinkUpdate* metalDisplayLinkUpdate) override;

public:
	void SetMetalDisplayLinkNeedsUpdateCallback(MetalDisplayLinkDelegateNeedsUpdateCallback callback);

private:
	MetalDisplayLinkDelegateNeedsUpdateCallback _callback;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_QUARTZCORE_METALDISPLAYLINKDELEGATE_HPP
