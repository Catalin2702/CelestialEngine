//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderModule.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Tools/Tools.hpp"
#include "Types/Render/Render.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <stdexcept>
#include <utility>


namespace CE::Core {

namespace {

/// What every entry point of the shader library ends with, so a function reads as one at a glance in a capture.
constexpr auto EntryPointSuffix = "Main";

/**
 * @brief Resolves the engine's name for a shader into the library function that implements it
 * @details The whole convention: the name in the language's own casing, plus the suffix - "CompositeVertex" becomes
 *			"compositeVertexMain". It lives here because it is the Metal answer to "where is this shader"; the caller
 *			states a name and knows nothing of the library or what its functions are called.
 */
std::string ResolveEntryPoint(const std::string_view name) {
	std::string entryPoint(name);
	entryPoint.front() = static_cast<char>(std::tolower(static_cast<unsigned char>(entryPoint.front())));

	return entryPoint + EntryPointSuffix;
}

}

MetalShaderModule::MetalShaderModule(const MetalShaderLibrary& library, const ShaderModuleDescriptor& descriptor):
	_stage(descriptor.stage) {
	// Owned, not a view: it feeds std::format calls that outlive the ternary's temporaries.
	const std::string name(descriptor.name);

	if (not Types::IsShaderTypeSupported(Types::GraphicsApi::Metal, descriptor.stage)) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: stage '{}' of module '{}' does not exist in Metal.", descriptor.stage, name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (name.empty()) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: a '{}' module was asked for under no name, so there is no function to look up.", descriptor.stage);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	_entryPoint = ResolveEntryPoint(name);

	const auto function = library.GetShaderFunction(_entryPoint);
	if (not function) [[unlikely]] {
		const auto message = std::format("MetalShaderModule: module '{}' resolves to '{}', and the shader library holds no function under that name.", name, _entryPoint);
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
