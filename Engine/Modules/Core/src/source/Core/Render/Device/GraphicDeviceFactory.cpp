//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: GraphicDeviceFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-04
//

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#endif

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_GraphicDevice> I_GraphicDevice::MakeDevice(const Types::GraphicsApi api) {
	switch (api) {
		case Types::GraphicsApi::OpenGL: {
			return std::make_unique<OpenGlGraphicDevice>();
		}
#if CE_PLATFORM_MACOS
		case Types::GraphicsApi::Metal: {
			return std::make_unique<MetalGraphicDevice>();
		}
#endif
		default: {
			constexpr auto error = "I_GraphicDevice::MakeDevice: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
