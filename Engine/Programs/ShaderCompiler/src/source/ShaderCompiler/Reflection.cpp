//
// Module: CelestialEngine/Engine/Programs/ShaderCompiler
// File: Reflection.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "ShaderCompiler/Reflection.hpp"
#include "Types/Var/Vars.hpp"

#include <spirv_cross/spirv_cross.hpp>

#include <algorithm>
#include <format>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>


namespace CE::ShaderCompiler {

namespace {

/**
 * @brief Spells a SPIR-V type the way the engine's shaders and Metal do: float3, float4x4, int2...
 * @details columns > 1 is a matrix: float4x4 is four columns of float4.
 */
std::string TypeName(const spirv_cross::SPIRType& type) {
	std::string base;
	switch (type.basetype) {
		case spirv_cross::SPIRType::Float: {
			base = "float";
			break;
		}
		case spirv_cross::SPIRType::Int: {
			base = "int";
			break;
		}
		case spirv_cross::SPIRType::UInt: {
			base = "uint";
			break;
		}
		case spirv_cross::SPIRType::Boolean: {
			base = "bool";
			break;
		}
		default: {
			base = "other";
			break;
		}
	}

	if (type.columns > 1)
		return std::format("{}{}x{}", base, type.columns, type.vecsize);
	if (type.vecsize > 1)
		return std::format("{}{}", base, type.vecsize);

	return base;
}

/**
 * @brief One line per input or output, sorted by location
 * @details SPIRV-Cross lists them in the order of their SPIR-V ids, which follows no rule a reader would expect; sorted,
 *			the file reads like the layout and two builds of the same shader always write the same text.
 */
std::string Interface(const spirv_cross::Compiler& compiler, const std::string_view kind,
const spirv_cross::SmallVector<spirv_cross::Resource>& variables) {
	std::vector<std::pair<u32, std::string>> lines;
	for (const auto& variable: variables) {
		const auto location = compiler.get_decoration(variable.id, spv::DecorationLocation);
		lines.emplace_back(location, std::format(
			"{} location={} name={} type={}\n",
			kind, location, variable.name, TypeName(compiler.get_type(variable.type_id))));
	}
	std::ranges::sort(lines);

	std::string text;
	for (const auto& line: lines | std::views::values)
		text += line;
	return text;
}

}

std::expected<std::string, std::string> Reflect(const SpirvBinary& spirv, const ShaderStage stage) {
	try {
		const spirv_cross::Compiler compiler{spirv};
		const auto resources = compiler.get_shader_resources();

		std::string text = std::format("stage {}\n", format_as(stage));

		// Stage inputs and outputs: what flows between the vertex buffer, the stages and the render target. Built-ins
		// (gl_Position, gl_FragCoord) are not listed: they belong to the pipeline, not to the interface.
		text += Interface(compiler, "input", resources.stage_inputs);
		text += Interface(compiler, "output", resources.stage_outputs);

		// A uniform block is named after its type ("CameraConstants"), not its instance ("camera"): the type name is
		// what OpenGL's glGetUniformBlockIndex looks up, and what the C++ struct is called.
		for (const auto& buffer: resources.uniform_buffers) {
			const auto& type = compiler.get_type(buffer.base_type_id);
			text += std::format("uniform_buffer name={} set={} binding={} size={}\n",
				compiler.get_name(buffer.base_type_id),
				compiler.get_decoration(buffer.id, spv::DecorationDescriptorSet),
				compiler.get_decoration(buffer.id, spv::DecorationBinding),
				compiler.get_declared_struct_size(type)
			);
		}

		for (const auto& image: resources.sampled_images) {
			text += std::format("sampled_image name={} set={} binding={}\n",
				image.name,
				compiler.get_decoration(image.id, spv::DecorationDescriptorSet),
				compiler.get_decoration(image.id, spv::DecorationBinding)
			);
		}

		return text;
	}
	catch (const spirv_cross::CompilerError& error) {
		return std::unexpected(std::string(error.what()));
	}
}

}
