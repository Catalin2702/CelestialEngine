//
// Module: CelestialEngine/Engine/Modules/Core/Render/Delegates/Platforms/Mac
// File: RenderViewDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_CORE_RENDER_DELEGATES_PLATFORMS_MAC_METAL_RENDERVIEWDELEGATE_HPP
#define CE_CORE_RENDER_DELEGATES_PLATFORMS_MAC_METAL_RENDERVIEWDELEGATE_HPP

#include "Apple/MetalCpp/MetalKit/RenderView.hpp"

namespace MTK {
class RenderView;

class RenderViewDelegate: public ViewDelegate {
public:
	void drawInMTKView(View* view) override;

	void drawableSizeWillChange(View* view, CGSize size) override;
};

}

#endif //
