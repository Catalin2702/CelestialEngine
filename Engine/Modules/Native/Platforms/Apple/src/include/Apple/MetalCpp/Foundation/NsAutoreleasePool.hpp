//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/Foundation
// File: NsAutoreleasePool.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_FOUNDATION_NSAUTORELEASEPOOL_HPP
#define CE_NATIVE_APPLE_METALCPP_FOUNDATION_NSAUTORELEASEPOOL_HPP

#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>


namespace CE::Native {

/**
 * @class NsAutoreleasePool
 * @brief An autorelease pool for the scope it lives in
 * @details Every Metal object handed back per frame - the command buffer, the drawable, the render pass descriptor
 *                    and its three attachment descriptors - is autoreleased. A frame driven by a display link or by the paced
 *                    run loop runs outside AppKit's own drain, so without a pool in scope none of them is ever released.
 */
class CE_APPLE_API NsAutoreleasePool {
public:
	NsAutoreleasePool(): _pool(NS::AutoreleasePool::alloc()->init()) {}

	NsAutoreleasePool(const NsAutoreleasePool&) = delete;
	NsAutoreleasePool(NsAutoreleasePool&&) noexcept = delete;

	~NsAutoreleasePool() { _pool->release(); }

public:
	NsAutoreleasePool& operator = (const NsAutoreleasePool&) = delete;
	NsAutoreleasePool& operator = (NsAutoreleasePool&&) noexcept = delete;

private:
	NS::AutoreleasePool* _pool;
};

}

#endif //CE_APPLE_NATIVE_METALCPP_FOUNDATION_NSAUTORELEASEPOOL_HPP
