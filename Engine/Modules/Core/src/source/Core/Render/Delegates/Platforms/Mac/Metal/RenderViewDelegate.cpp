//
// Module: CelestialEngine/Engine/Modules/Core/Render/Delegates/Platforms/Mac
// File: RenderViewDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Core/Render/Delegates/Platforms/Mac/Metal/RenderViewDelegate.hpp"

#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"

namespace MTK {

void RenderViewDelegate::drawInMTKView(View*) {
	auto& app = dynamic_cast<CE::Core::Application::CocoaApplication&>(CE::Core::Application::I_Application::StGet());
	app.Tick(app.GetDeltaTime());
}

void RenderViewDelegate::drawableSizeWillChange(View*, CGSize) {}

}
