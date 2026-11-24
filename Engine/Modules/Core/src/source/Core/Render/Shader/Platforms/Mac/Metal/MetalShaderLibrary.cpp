//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderLibrary.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-31
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "IO/FileSystem/FileSystem.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <ranges>


namespace CE::Core {

namespace {

/// The library the build links every .metal file into (Engine/Shaders), in the engine's own read-only mount.
constexpr auto DefaultLibraryPath = "engine://Shaders/Metal/Main.metallib";

/**
 * @brief Resolves the bundled library to the native path Metal's loader opens itself
 * @details Through the file system, like every other shipped file, rather than through NSBundle: the mount already
 *			knows where the packaging put the engine's files, and it was made absolute at startup, so launching from an IDE
 *			or a debugger with another working directory changes nothing. An unresolvable path comes back empty rather
 *			than guessed at: _LoadLibrary then fails and reports it, which is the message worth reading.
 */
std::string ResolveDefaultMetalLibraryPath() {
	const auto path = IO::FileSystem::ToNativePath(DefaultLibraryPath);
	if (not path) [[unlikely]] {
		CE_CORE_ERROR("ResolveDefaultMetalLibraryPath: Cannot resolve '{}' ({})", DefaultLibraryPath, path.error());
		return {};
	}

	return path->string();
}

}

MetalShaderLibrary::MetalShaderLibrary(MTL::Device* device, const std::string& path):
	_device(NS::RetainPtr(device)), _path(path.empty() ? ResolveDefaultMetalLibraryPath() : path) {
	_LoadLibrary();

	if (not IsValid()) [[unlikely]] {
		CE_CORE_ERROR("MetalShaderLibrary::MetalShaderLibrary: Failed to load default shader library from device.");
		throw std::runtime_error("Failed to load default shader library from device.");
	}
}

MetalShaderLibrary::MetalShaderLibrary(MetalShaderLibrary&& other) noexcept:
	_device(std::move(other._device)), _library(std::move(other._library)), _path(std::move(other._path)),
	_functions(std::move(other._functions)) {
	// A moved-from map is only valid-but-unspecified: emptied explicitly so its destructor releases nothing.
	other._functions.clear();
}

MetalShaderLibrary::~MetalShaderLibrary() {
	_Release();
}

void MetalShaderLibrary::_Release() {
	for (const auto& function: _functions | std::views::values) {
		if (function) [[likely]] {
			function->release();
		}
	}
	_functions.clear();
	_library.reset();
	_device.reset();
}

MetalShaderLibrary& MetalShaderLibrary::operator = (MetalShaderLibrary&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	// The functions held so far are owned: released before the other library's ones take their place.
	_Release();

	_device = std::move(other._device);
	_library = std::move(other._library);
	_path = std::move(other._path);
	_functions = std::move(other._functions);
	other._functions.clear();
	return *this;
}

ShaderProgram MetalShaderLibrary::GetShaderProgram(const std::string& vertexName, const std::string& fragmentName) const {
	ShaderProgram program{};
	program.vertexFunction = GetShaderFunction(vertexName);
	program.fragmentFunction = GetShaderFunction(fragmentName);

	if (not program.vertexFunction) [[unlikely]] {
		CE_CORE_ERROR("MetalShaderLibrary::GetShaderProgram: Failed to get vertex shader function '{}' from library.", vertexName);
	}
	if (not program.fragmentFunction) [[unlikely]] {
		CE_CORE_ERROR("MetalShaderLibrary::GetShaderProgram: Failed to get fragment shader function '{}' from library.", fragmentName);
	}

	return program;
}

MTL::Function* MetalShaderLibrary::GetShaderFunction(const std::string& name) const {
	if (const auto it = _functions.find(name); it != _functions.end()) [[likely]] {
		return it->second;
	}
	return nullptr;
}

bool MetalShaderLibrary::IsValid() const {
	return _library.get() != nullptr;
}

void MetalShaderLibrary::_LoadLibrary() {
	auto* const url = NS::URL::fileURLWithPath(NS::String::string(_path.c_str(), NS::UTF8StringEncoding));
	NS::Error* error = nullptr;
	auto* const library = _device->newLibrary(url, &error);
	if (not library) [[unlikely]] {
		const auto errorMessage = std::format("MetalShaderLibrary::_LoadLibrary: Failed to load Metal shader library from path: {}. Error: {}", _path, error ? error->localizedDescription()->utf8String() : "Unknown error");
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}
	_library = NS::RetainPtr(library);

	auto* const functionNames = _library->functionNames();
	auto* const enumerator = functionNames->objectEnumerator();
	const NS::String* functionName = nullptr;
	while ((functionName = reinterpret_cast<NS::String*>(enumerator->nextObject())) != nullptr) {
		const auto* const functionNameStr = functionName->utf8String();
		if (auto* const function = _library->newFunction(functionName)) [[likely]] {
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
