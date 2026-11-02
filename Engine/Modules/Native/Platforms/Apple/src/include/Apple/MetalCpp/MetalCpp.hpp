//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp
// File: MetalCpp.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file MetalCpp.hpp
 * @brief Umbrella header for the metal-cpp wrappers of the Apple module
 * @details One include for every C++ class that stands in for an Objective-C delegate or dispatcher: AppKit,
 *			Foundation, MetalKit and QuartzCore.
 *
 *			No umbrella per framework folder, on purpose: Apple/MetalCpp/AppKit/AppKit.hpp would share its name with
 *			metal-cpp's own <AppKit/AppKit.hpp>, and which of the two an include finds would depend on the search path.
 */

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_METALCPP_HPP
#define CE_NATIVE_APPLE_METALCPP_METALCPP_HPP

#include "Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp"
#include "Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp"
#include "Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp"

#include "Apple/MetalCpp/Foundation/NsAutoreleasePool.hpp"

#include "Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp"

#include "Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp"

#endif //CE_NATIVE_APPLE_METALCPP_METALCPP_HPP
