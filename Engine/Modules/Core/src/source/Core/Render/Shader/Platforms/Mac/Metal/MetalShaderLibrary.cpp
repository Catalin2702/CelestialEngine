//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderLibrary.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Tools/Log/Log.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <ranges>


namespace CE::Core {

// Relative fallback, resolved against the current working directory. Only used
// when the app is not running from a bundle (e.g. some test harnesses).
constexpr auto relativeLibraryPath = "../Resources/Shaders/Metal/Main.metallib";

// Resolves the shader library to an absolute path based on the app bundle's
// Resources directory, so loading is independent of the current working
// directory (fixes launches from IDEs/debuggers such as CLion and Xcode).
static std::string ResolveDefaultLibraryPath() {
	if (const auto bundle = NS::Bundle::mainBundle()) {
		if (const auto resourcePath = bundle->resourcePath()) {
			return std::string(resourcePath->utf8String()) + "/Shaders/Metal/Main.metallib";
		}
	}
	return relativeLibraryPath;
}

static std::string defaultLibraryPath = relativeLibraryPath;

MetalShaderLibrary::MetalShaderLibrary(MTL::Device* device, const std::string& path): _device(NS::RetainPtr(device)) {
	defaultLibraryPath = path.empty() ? ResolveDefaultLibraryPath() : path;

	_LoadLibrary();

	if (not IsValid()) {
		CE_CORE_ERROR("MetalShaderLibrary::MetalShaderLibrary: Failed to load default shader library from device.");
		throw std::runtime_error("Failed to load default shader library from device.");
	}
}

MetalShaderLibrary::MetalShaderLibrary(MetalShaderLibrary&& other) noexcept:
	_device(std::move(other._device)), _library(std::move(other._library)), _functions(std::move(other._functions)) {}

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

MetalShaderLibrary& MetalShaderLibrary::operator=(const MetalShaderLibrary& other) {
	if (this == &other) [[unlikely]]
		return *this;

	_device = other._device;
	_library = other._library;
	_functions = other._functions;
	return *this;
}

MetalShaderLibrary& MetalShaderLibrary::operator=(MetalShaderLibrary&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	_device = std::move(other._device);
	_library = std::move(other._library);
	_functions = std::move(other._functions);
	return *this;
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
		const auto errorMessage = std::format("MetalShaderLibrary::_LoadLibrary: Failed to load Metal shader library from path: {}. Error: {}", defaultLibraryPath, error ? error->localizedDescription()->utf8String() : "Unknown error");
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
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
			const auto errorMessage = std::format("MetalShaderLibrary::_LoadLibrary: Failed to load shader function '{}' from library.", functionNameStr);
			CE_CORE_ERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
	}
}

}
