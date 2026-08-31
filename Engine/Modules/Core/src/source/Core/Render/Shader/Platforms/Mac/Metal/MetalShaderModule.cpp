//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderModule.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Render.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <stdexcept>
#include <utility>


namespace CE::Core {

MetalShaderModule::MetalShaderModule(const MetalShaderLibrary& library, const ShaderModuleDescriptor& descriptor):
	_entryPoint(descriptor.entryPoint), _stage(descriptor.stage) {
	// Owned, not a view: it feeds std::format calls that outlive the ternary's temporaries.
	const std::string name = descriptor.debugName.empty() ? _entryPoint : std::string(descriptor.debugName);

	if (not Types::IsShaderTypeSupported(Types::GraphicsApi::Metal, descriptor.stage)) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: stage '{}' of module '{}' does not exist in Metal.", Types::ToString(descriptor.stage), name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (_entryPoint.empty()) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: module '{}' has no entry point to look up.", name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	const auto function = library.GetShaderFunction(_entryPoint);
	if (not function) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: the shader library holds no function named '{}'.", _entryPoint);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	// The library hands out non-owning pointers and releases them in its own destructor; retaining decouples the two
	// lifetimes, so a module outliving the library it came from stays usable.
	_function = function->retain();
}

MetalShaderModule::MetalShaderModule(MetalShaderModule&& other) noexcept:
	_entryPoint(std::move(other._entryPoint)), _function(other._function), _stage(other._stage) {
	// The moved-from module must forget the function, otherwise its destructor releases the one we just took.
	other._function = nullptr;
	other._stage = Types::ShaderType::None;
}

MetalShaderModule& MetalShaderModule::operator = (MetalShaderModule&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (_function)
		_function->release();

	_entryPoint = std::move(other._entryPoint);
	_function = other._function;
	_stage = other._stage;

	other._function = nullptr;
	other._stage = Types::ShaderType::None;

	return *this;
}

MetalShaderModule::~MetalShaderModule() {
	if (not _function) [[unlikely]]
		return;

	_function->release();
}

}
