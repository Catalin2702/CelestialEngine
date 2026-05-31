//
// Module: CelestialEngine/Engine/Modules/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderLibrary.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-31
// Updated by: Catalin Chirosca
// Updated: 2026-05-31
//

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"

#include <ranges>

#include "Apple/MetalCpp/Metal/Metal.hpp"
#include "Tools/Log/Log.hpp"

namespace CE::Core::Render::Shader {

std::string defaultLibraryPath = "../Resources/Shaders/Metal/Main.metallib";

MetalShaderLibrary::MetalShaderLibrary(MTL::Device* device, const std::string& path): _device(NS::RetainPtr(device)) {
	if (not path.empty())
		defaultLibraryPath = path;

	_LoadLibrary();

	if (not IsValid()) {
		CE_CORE_ERROR("MetalShaderLibrary::MetalShaderLibrary: Failed to load default shader library from device.");
		throw std::runtime_error("Failed to load default shader library from device.");
	}
}

MetalShaderLibrary::~MetalShaderLibrary() {
	for (const auto& function: _functions | std::views::values) {
		if (function) {
			function->release();
		}
	}
	_functions.clear();
	_library.reset();
	_device.reset();
}

ShaderProgram MetalShaderLibrary::GetShaderProgram(const std::string& vertexName, const std::string& fragmentName) const {
	ShaderProgram program{};
	program.vertexFunction = GetShaderFunction(vertexName);
	program.fragmentFunction = GetShaderFunction(fragmentName);

	if (not program.vertexFunction) {
		CE_CORE_ERROR("MetalShaderLibrary::GetShaderProgram: Failed to get vertex shader function '{}' from library.", vertexName);
	}
	if (not program.fragmentFunction) {
		CE_CORE_ERROR("MetalShaderLibrary::GetShaderProgram: Failed to get fragment shader function '{}' from library.", fragmentName);
	}

	return program;
}

MTL::Function* MetalShaderLibrary::GetShaderFunction(const std::string& name) const {
	if (const auto it = _functions.find(name); it != _functions.end()) {
		return it->second;
	}
	return nullptr;
}

bool MetalShaderLibrary::IsValid() const {
	return _library.get() != nullptr;
}

void MetalShaderLibrary::_LoadLibrary() {
	const auto url = NS::URL::fileURLWithPath(NS::String::string(defaultLibraryPath.c_str(), NS::UTF8StringEncoding));
	NS::Error* error = nullptr;
	const auto library = _device->newLibrary(url, &error);
	if (not library) {
		const auto errorMessage = "Failed to load Metal shader library from path: " + defaultLibraryPath + ". Error: " + (error ? error->localizedDescription()->utf8String() : "Unknown error");
		CE_CORE_ERROR("MetalShaderLibrary::_LoadLibrary: {0}", errorMessage);
		throw std::runtime_error("MetalShaderLibrary::_LoadLibrary: " + errorMessage);
	}
	_library = NS::RetainPtr(library);

	const auto functionNames = _library->functionNames();
	const auto enumerator = functionNames->objectEnumerator();
	const NS::String* functionName = nullptr;
	while ((functionName = reinterpret_cast<NS::String*>(enumerator->nextObject())) != nullptr) {
		const auto functionNameStr = functionName->utf8String();
		if (const auto function = _library->newFunction(functionName)) {
			_functions[functionNameStr] = function;
		}
		else {
			CE_CORE_ERROR("MetalShaderLibrary::_LoadLibrary: Failed to load shader function '{}' from library.", functionNameStr);
		}
	}
}

}
