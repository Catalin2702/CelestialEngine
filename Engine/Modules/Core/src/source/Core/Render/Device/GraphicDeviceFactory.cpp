//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: GraphicDeviceFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Render/Device/Device.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#if CE_RENDER_METAL
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_GraphicDevice> I_GraphicDevice::MakeDevice(const Types::GraphicsApi api) {
	switch (api) {
#if CE_RENDER_OPENGL
		case Types::GraphicsApi::OpenGL: {
			return std::make_unique<OpenGlGraphicDevice>();
		}
#endif
#if CE_RENDER_METAL
		case Types::GraphicsApi::Metal: {
			return std::make_unique<MetalGraphicDevice>();
		}
#endif
		default: {
			// Either a backend that does not exist yet, or one this build was configured without (CE_RENDER_BACKENDS).
			const auto error = std::format("I_GraphicDevice::MakeDevice: {} is not compiled into this build!", api);
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
