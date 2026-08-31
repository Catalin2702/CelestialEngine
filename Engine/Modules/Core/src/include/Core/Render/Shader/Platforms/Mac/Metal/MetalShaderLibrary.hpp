//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderLibrary.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_METALSHADERLIBRARY_HPP
#define CE_CORE_RENDER_SHADER_METALSHADERLIBRARY_HPP

#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>

#include <string>
#include <unordered_map>


namespace MTL {
	class Device;
	class Function;
	class Library;
}

namespace CE::Core {

/**
 * @struct ShaderProgram
 * @brief Pair of Metal shader entry points forming a render pipeline program
 */
struct ShaderProgram {
	MTL::Function* vertexFunction = nullptr;		///< Vertex stage entry point (non-owning)
	MTL::Function* fragmentFunction = nullptr;		///< Fragment stage entry point (non-owning)
};

/**
 * @class MetalShaderLibrary
 * @brief Loads the compiled .metallib from the app bundle and exposes its shader functions by name
 * @details Wraps an MTL::Library: on construction it resolves the bundled Main.metallib (via FileSystem, unless an
 *			explicit path is provided), loads it on the given device and caches every contained MTL::Function keyed by
 *			its entry-point name. Non-copyable and non-movable: it owns retained Metal objects.
 */
class CE_CORE_API MetalShaderLibrary {
public:
	MetalShaderLibrary() = default;

	/**
	 * @brief Constructor - loads the shader library
	 * @param device Metal device the library is created on (retained)
	 * @param path Optional explicit path to the .metallib; empty resolves the bundled Shaders/Metal/Main.metallib
	 */
	explicit MetalShaderLibrary(MTL::Device* device, const std::string& path = "");

	MetalShaderLibrary(const MetalShaderLibrary&) = default;
	MetalShaderLibrary(MetalShaderLibrary&& other) noexcept;

	~MetalShaderLibrary();

public:
	MetalShaderLibrary& operator = (const MetalShaderLibrary& other);
	MetalShaderLibrary& operator = (MetalShaderLibrary&& other) noexcept;

public:
	/**
	 * @brief Gets a vertex/fragment function pair by entry-point names
	 * @param vertexName Name of the vertex entry point (e.g. "vertexMain")
	 * @param fragmentName Name of the fragment entry point (e.g. "fragmentMain")
	 * @return ShaderProgram The pair of functions; a member is nullptr if the name is not in the library
	 */
	[[nodiscard]] ShaderProgram GetShaderProgram(const std::string& vertexName, const std::string& fragmentName) const;

	/**
	 * @brief Gets a single shader function by entry-point name
	 * @param name Name of the entry point
	 * @return MTL::Function* The cached function, or nullptr if not found (non-owning)
	 */
	[[nodiscard]] MTL::Function* GetShaderFunction(const std::string& name) const;

	/**
	 * @brief Gets every function contained in the library, keyed by entry-point name
	 */
	[[nodiscard]] const std::unordered_map<std::string, MTL::Function*>& GetAllShaderFunctions() const { return _functions; }

	/**
	 * @brief Gets the underlying Metal library
	 */
	[[nodiscard]] const MTL::Library* GetLibrary() const { return _library.get(); }

	/**
	 * @brief Checks whether the library was loaded successfully and contains at least one function
	 */
	[[nodiscard]] bool IsValid() const;

private:
	/**
	 * @brief Loads the .metallib from disk and caches its functions
	 */
	void _LoadLibrary();

private:
	NS::SharedPtr<MTL::Device> _device;				///< Device the library was created on
	NS::SharedPtr<MTL::Library> _library;			///< Loaded Metal library

	std::unordered_map<std::string, MTL::Function*> _functions;	///< Functions cached by entry-point name
};

}

#endif //CE_CORE_RENDER_SHADER_METALSHADERLIBRARY_HPP
