//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: CrossCompiler.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "ShaderCompiler/CrossCompiler.hpp"

#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_msl.hpp>


namespace CE::ShaderCompiler {

namespace {

/// The version of the Metal Shading Language the output is written for (macOS 11 and later).
constexpr u32 MslMajorVersion = 2;
constexpr u32 MslMinorVersion = 3;

/// GLSL 4.10: what macOS's OpenGL 4.1 core profile compiles.
constexpr u32 GlslVersion = 410;

spv::ExecutionModel ToExecutionModel(const ShaderStage stage) {
	switch (stage) {
		case ShaderStage::Vertex: return spv::ExecutionModelVertex;
		case ShaderStage::Fragment: return spv::ExecutionModelFragment;
		default: return spv::ExecutionModelVertex;
	}
}


/**
 * @brief Gives every resource of the module the Metal index equal to its binding
 * @details Left alone, SPIRV-Cross numbers Metal resources in the order it meets them: the first uniform buffer becomes
 *			[[buffer(0)]] - the vertex buffer's index - whatever binding the source gave it.
 */
void MapBindingsToMetalIndices(spirv_cross::CompilerMSL& compiler, const spv::ExecutionModel model) {
	const auto resources = compiler.get_shader_resources();

	const auto map = [&](const spirv_cross::Resource& resource) {
		const auto decorationBinding = compiler.get_decoration(resource.id, spirv_cross::DecorationBinding);
		const spirv_cross::MSLResourceBinding binding{
			.stage = model,
			.desc_set = compiler.get_decoration(resource.id, spirv_cross::DecorationDescriptorSet),
			.binding = decorationBinding,
			.msl_buffer = decorationBinding,
			.msl_texture = decorationBinding,
			.msl_sampler = decorationBinding,
		};
		compiler.add_msl_resource_binding(binding);
	};

	for (const auto& resource: resources.uniform_buffers)
		map(resource);
	for (const auto& resource: resources.storage_buffers)
		map(resource);
	for (const auto& resource: resources.sampled_images)
		map(resource);
}

}

std::expected<std::string, std::string> ToMsl(const SpirvBinary& spirv, const ShaderStage stage, const std::string& entryPoint) {
	try {
		spirv_cross::CompilerMSL compiler{spirv};

		auto options = compiler.get_msl_options();
		options.platform = spirv_cross::CompilerMSL::Options::Platform::macOS;
		options.set_msl_version(MslMajorVersion, MslMinorVersion);

		compiler.set_msl_options(options);

		const auto model = ToExecutionModel(stage);
		MapBindingsToMetalIndices(compiler, model);

		// Every GLSL entry point is "main"; MSL needs a unique name per function of the library, and the engine's
		// convention is the shader's name in camelCase plus "Main" (MetalShaderModule::ResolveEntryPoint).
		compiler.rename_entry_point("main", entryPoint, model);

		return compiler.compile();
	}
	catch (const spirv_cross::CompilerError& error) {
		return std::unexpected(std::string(error.what()));
	}
}

std::expected<std::string, std::string> ToGlsl(const SpirvBinary& spirv) {
	try {
		spirv_cross::CompilerGLSL compiler{spirv};

		auto options = compiler.get_common_options();
		options.version = GlslVersion;
		options.es = false;
		// No GL_ARB_shading_language_420pack: macOS does not have it, so bindings are left to the names.
		options.enable_420pack_extension = false;
		options.vulkan_semantics = false;

		compiler.set_common_options(options);

		return compiler.compile();
	}
	catch (const spirv_cross::CompilerError& error) {
		return std::unexpected(std::string(error.what()));
	}
}

}
